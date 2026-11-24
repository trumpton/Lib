#include "dialogfocus.h"

#include <QAccessible>
#include <QAccessibleWidget>
#include <QLabel>
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QFile>
#include <QGuiApplication>
#include <QPointer>
#include <QTextStream>
#include <QTimer>
#include <QWidget>
#include <QWindow>

#ifdef Q_OS_WIN
// Windows does not always let a window be made the foreground window when it is shown (or when another one closes), so
// this asks for it the way Windows allows: by sharing the input queue of the thread that has the foreground.  Not compiled
// on the Linux build; checked by reading only.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
static void forceForeground(QWidget *w)
{
    HWND target = reinterpret_cast<HWND>(w->winId()) ;
    HWND before = GetForegroundWindow() ;
    DWORD fgthread = GetWindowThreadProcessId(before, NULL) ;
    DWORD mythread = GetCurrentThreadId() ;
    bool attached = (fgthread!=0 && fgthread!=mythread) && AttachThreadInput(fgthread, mythread, TRUE) ;
    BOOL set = SetForegroundWindow(target) ;
    BringWindowToTop(target) ;
    if (attached) AttachThreadInput(fgthread, mythread, FALSE) ;
    SwitchToThisWindow(target, TRUE) ;
    HWND after = GetForegroundWindow() ;
    wchar_t title[256] = {0} ;
    GetWindowTextW(after, title, 256) ;
    accessibleTrace(QString("foreground: SetForegroundWindow=%1 attached=%2 dialogIsForeground=%3 foreground window title=%4")
                    .arg(set ? "true" : "false").arg(attached ? "true" : "false").arg(after==target ? "true" : "false")
                    .arg(QString::fromWCharArray(title))) ;
}
#endif

static bool traceOn = false ;
static QString (*traceFolder)() = nullptr ;
static bool guardInstalled = false ;

void setAccessibleTrace(bool on)
{
    traceOn = on ;
}

void setAccessibleTraceFolder(QString (*folder)())
{
    traceFolder = folder ;
}

void accessibleTrace(const QString &line)
{
    if (!traceOn) return ;
    QString text = QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + " " + line ;
    qDebug().noquote() << "AccessibleTrace" << text ;
    QString folder = traceFolder ? traceFolder() : QString() ;
    if (folder.isEmpty()) return ;
    QFile file(folder + "/accessibility-debug.txt") ;
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file) ;
        out << text << "\n" ;
    }
}

static QString nameOfWidget(QObject *o)
{
    if (!o) return QString("none") ;
    QString n = o->objectName() ;
    return n.isEmpty() ? QString(o->metaObject()->className()) : n ;
}

// The state of the windows and the focus, for the trace
static QString focusState()
{
    return QString("focus=%1 activeWindow=%2 modal=%3")
        .arg(nameOfWidget(QApplication::focusWidget()))
        .arg(nameOfWidget(QApplication::activeWindow()))
        .arg(nameOfWidget(QApplication::activeModalWidget())) ;
}

void traceFocus(const QString &where)
{
    if (!traceOn) return ;
    accessibleTrace(where + " " + focusState()) ;
}

// True while a popup is open (a combo box list, a menu, a completer), or the focus is in one.  A popup is a window of its own,
// so the dialog's focus guard must leave the focus alone: moving it back to the dialog closes the popup.
static bool popupInUse(QWidget *w = nullptr)
{
    if (QApplication::activePopupWidget()) return true ;
    if (!w) return false ;
    Qt::WindowType type = w->window()->windowType() ;
    return type==Qt::Popup || type==Qt::ToolTip ;
}

// Tells the screen reader which control has the focus.  Qt reports a focus change to it, but not when the window is not
// active, or when the control already had the focus, so the report is sent here as well.
static void announceFocus(QWidget *w)
{
    if (!w) return ;
    accessibleTrace("announce " + nameOfWidget(w) + " " + focusState()) ;
    QAccessibleEvent event(w, QAccessible::Focus) ;
    QAccessible::updateAccessibility(&event) ;
}

// Makes an open modal dialog the active window, and gives the focus to its focus control (the one it had, or the first)
static void activateDialog(QDialog *dlg)
{
    accessibleTrace("activate " + nameOfWidget(dlg) + " " + focusState()) ;
    if (QApplication::activeModalWidget()!=dlg) return ;
    if (popupInUse()) { accessibleTrace("popup open: the dialog is not activated") ; return ; }
#ifdef Q_OS_WIN
    forceForeground(dlg) ;
#endif
    dlg->activateWindow() ;
    // Qt's own active window is what a screen reader follows.  The trace showed it still the main window after the dialog was
    // made the foreground window, so it is set here as well.
    QApplication::setActiveWindow(dlg) ;
    dlg->raise() ;
    QWidget *target = dlg->focusWidget() ;
    if (!target || !dlg->isAncestorOf(target)) {
        // No focus control yet: the first one that can take the focus
        target = dlg ;
        for (QWidget *w : dlg->findChildren<QWidget *>()) {
            if (w->isVisible() && w->isEnabled() && w->focusPolicy()!=Qt::NoFocus) { target = w ; break ; }
        }
    }
    target->setFocus(Qt::ActiveWindowFocusReason) ;
    accessibleTrace("activated " + nameOfWidget(dlg) + ": activeWindow now " + nameOfWidget(QApplication::activeWindow()) + ", focus " + nameOfWidget(target)) ;
    announceFocus(target) ;

    // Qt may still have focus events queued for the window it left.  The announcement is repeated once they have run, so the
    // last one the screen reader hears is this dialog's control.
    QPointer<QWidget> control(target) ;
    QTimer::singleShot(0, dlg, [dlg, control]() {
        if (!control || QApplication::activeModalWidget()!=dlg || QApplication::focusWidget()!=control) return ;
        accessibleTrace("announce again after queued events: " + nameOfWidget(control) + " " + focusState()) ;
        announceFocus(control) ;
    }) ;
}

// When a dialog closes, Qt gives the focus back to the window that was active before it.  That can happen after the next
// dialog has opened, so the focus, and the screen reader with it, moves out of the new dialog (to the calendar list, for
// example).  This moves the focus back into the open modal dialog, to the widget that had it, or to the dialog itself.
// It runs after Qt has finished moving the focus, so it has the last word.
static void keepFocusInModalDialog(QWidget *old, QWidget *now)
{
    accessibleTrace("focusChanged " + nameOfWidget(old) + " -> " + nameOfWidget(now) + " " + focusState()) ;
    QWidget *modal = QApplication::activeModalWidget() ;
    if (!modal || !now) return ;
    if (now==modal || modal->isAncestorOf(now)) return ;
    if (popupInUse(now)) { accessibleTrace("focus went to the popup " + nameOfWidget(now) + ": left there") ; return ; }
    accessibleTrace("focus went to " + nameOfWidget(now) + " outside the dialog " + nameOfWidget(modal) + " " + focusState()) ;

    QTimer::singleShot(0, modal, [modal]() {
        if (QApplication::activeModalWidget()!=modal || popupInUse()) return ;
        if (QDialog *dlg = qobject_cast<QDialog *>(modal)) activateDialog(dlg) ;
        else {
            modal->activateWindow() ;
            announceFocus(modal) ;
        }
    }) ;
}

// Windows can make the window behind the dialog active after the dialog has been shown (when the previous dialog closes,
// for example), so the focus goes to the calendar list and the screen reader follows it.  For a short time after the dialog
// is shown, this checks that the dialog is still the active window, and makes it active again if it is not.  After that
// the user is left in charge, so that switching to another program is not undone.
static const int SETTLE_MS = 1500 ;
static const int SETTLE_STEP_MS = 50 ;

void showFocused(QDialog *dlg)
{
    installDialogFocusGuard() ;

    // A window with a Qt parent is nested under that window in the accessibility tree, so it becomes a window of its own
    if (dlg->parentWidget()) dlg->setParent(nullptr, dlg->windowFlags()) ;

    accessibleTrace("show " + nameOfWidget(dlg) + " " + focusState()) ;
    dlg->show() ;
    activateDialog(dlg) ;
}

void showWindow(QWidget *w)
{
    installDialogFocusGuard() ;

    // A window with a Qt parent belongs to that window: Alt+Tab does not give it, and the parent brings it back.  It is made a
    // window of its own, as a top-level window, before it is shown.
    if (w->parentWidget() || w->windowType()!=Qt::Window) w->setParent(nullptr, Qt::Window) ;

    accessibleTrace("show window " + nameOfWidget(w) + " " + focusState()) ;
    w->show() ;
    w->raise() ;
    w->activateWindow() ;
    QApplication::setActiveWindow(w) ;

    QWidget *target = w->focusWidget() ;
    if (!target || !w->isAncestorOf(target)) {
        // No focus control yet: the first one that can take the focus
        target = w ;
        for (QWidget *c : w->findChildren<QWidget *>()) {
            if (c->isVisible() && c->isEnabled() && c->focusPolicy()!=Qt::NoFocus) { target = c ; break ; }
        }
    }
    target->setFocus(Qt::ActiveWindowFocusReason) ;
    announceFocus(target) ;
}

int execFocused(QDialog *dlg)
{
    // QDialog::exec, not exec, so that a dialog which overrides exec() (ItemSelect) does not come back here.
    installDialogFocusGuard() ;

    // A dialog with a Qt parent is nested under that window in the accessibility tree, so it becomes a window of its own.
    // Its parent does not delete it after this (the caller deletes it, as it always did).
    if (dlg->parentWidget()) dlg->setParent(nullptr, dlg->windowFlags()) ;

    accessibleTrace("open " + nameOfWidget(dlg) + " " + focusState()) ;
    QTimer::singleShot(0, dlg, [dlg]() { activateDialog(dlg) ; }) ;

    QElapsedTimer clock ;
    clock.start() ;
    QTimer *settle = new QTimer(dlg) ;
    settle->setInterval(SETTLE_STEP_MS) ;
    QObject::connect(settle, &QTimer::timeout, dlg, [dlg, settle, clock]() {
        if (QApplication::activeModalWidget()!=dlg || clock.elapsed()>SETTLE_MS) {
            settle->stop() ;
            return ;
        }
        if (popupInUse()) return ;
        if (QApplication::activeWindow()!=dlg) activateDialog(dlg) ;
    }) ;
    settle->start() ;

    int result = dlg->QDialog::exec() ;
    accessibleTrace("closed " + nameOfWidget(dlg) + " " + focusState()) ;
    return result ;
}

// A window of ours other than the open modal dialog has become the focused window (the main window, for example): the dialog
// is brought back in front.  Popups and other programs are left alone, so switching to another program still works.
static void keepModalInFront(QWindow *window)
{
    if (!window || (window->type()!=Qt::Window && window->type()!=Qt::Dialog)) return ;

    // The open modal dialog comes back to the front when another window of ours has the focus
    QDialog *dlg = qobject_cast<QDialog *>(QApplication::activeModalWidget()) ;
    if (dlg && window!=dlg->windowHandle()) {
        accessibleTrace("window " + window->objectName() + " came to the front; bringing " + nameOfWidget(dlg) + " back") ;
        QTimer::singleShot(0, dlg, [dlg]() {
            if (QApplication::activeModalWidget()==dlg) activateDialog(dlg) ;
        }) ;
    }

    // The child dialogs of the window that has the focus (the main window, for example) come to the front with it
    for (QWidget *top : QApplication::topLevelWidgets()) {
        if (!top->isVisible() || top->windowHandle()!=window) continue ;
        for (QDialog *child : top->findChildren<QDialog *>(QString(), Qt::FindDirectChildrenOnly)) {
            if (child->isVisible() && child!=dlg) child->raise() ;
        }
    }
}

// A label: the same as its widget, except that a label marked by hideLabelFromSpeech is invisible in the tree.  The mark is read
// each time the state is asked for, not when the interface is made, because Qt makes the interface while the dialog is built
// and keeps it, and a label can be marked after that.
class SpeechHiddenLabel : public QAccessibleWidget
{
public:
    // A label is static text, and its name is its text, as Qt's own label interface gives it
    explicit SpeechHiddenLabel(QWidget *widget) : QAccessibleWidget(widget, QAccessible::StaticText) {}
    QString text(QAccessible::Text t) const override {
        if (t==QAccessible::Name) {
            if (QLabel *label = qobject_cast<QLabel *>(widget())) {
                QString name = label->text() ;
                name.remove(QLatin1Char('&')) ;
                return name ;
            }
        }
        return QAccessibleWidget::text(t) ;
    }
    QAccessible::State state() const override {
        QAccessible::State s = QAccessibleWidget::state() ;
        if (widget() && widget()->property("speechHidden").toBool()) s.invisible = true ;
        return s ;
    }
};

// The accessibility factory: the label interface for every label
static QAccessibleInterface *speechFactory(const QString &classname, QObject *object)
{
    Q_UNUSED(classname) ;
    // Any label, including the A-prefixed labels (ALabel), which are QLabels
    if (qobject_cast<QLabel *>(object))
        return new SpeechHiddenLabel(qobject_cast<QWidget *>(object)) ;
    return nullptr ;
}

void hideLabelFromSpeech(QWidget *label)
{
    if (label) label->setProperty("speechHidden", true) ;
}

void installDialogFocusGuard()
{
    if (guardInstalled) return ;
    guardInstalled = true ;
    QAccessible::installFactory(speechFactory) ;
    QObject::connect(qApp, &QApplication::focusChanged, keepFocusInModalDialog) ;
    QObject::connect(qGuiApp, &QGuiApplication::focusWindowChanged, keepModalInFront) ;
}

