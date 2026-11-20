#ifndef DIALOGFOCUS_H
#define DIALOGFOCUS_H

#include <QString>

class QDialog ;

// Dialogs and the screen reader.  A dialog must be the window that the screen reader follows, with its own control
// focused, and it must stay in front of the program's main window.
//
// execFocused() runs a modal dialog, in place of exec().  It makes the dialog a window of its own (not nested under its
// parent in the accessibility tree), shows it, makes it the active window with its focus control focused, and tells the
// screen reader which control that is.  The first call installs the focus guard, which keeps the dialog in front; later
// calls do nothing more.  installDialogFocusGuard() may also be called at start-up, and is safe to call more than once.
void installDialogFocusGuard() ;

// Runs a modal dialog, in place of exec().  The caller still deletes the dialog if it created it with new.
int execFocused(QDialog *dlg) ;

// Shows a window that stays open while the user works elsewhere, or while a long operation runs (a progress window), as its own
// window, and makes it the active window with its focus control focused, as execFocused does.  The screen reader follows the
// active window, so a progress window shown without this is not followed while it updates.
void showFocused(QDialog *dlg) ;

// A label whose text is already part of its field's name (for example, "From" with the date box after it) is still shown on
// the screen, but is marked invisible in the accessibility tree, so the screen reader does not read it with the dialog.
// Only labels passed to this function are changed. installDialogFocusGuard() installs the factory that does it.
class QWidget ;
void hideLabelFromSpeech(QWidget *label) ;

// Tracing, for finding a fault with a screen reader.  Off until setAccessibleTrace(true).  Each step is written to
// accessibility-debug.txt in the folder given by setAccessibleTraceFolder (if a folder function is set), and to the debug output.
void setAccessibleTrace(bool on) ;
void setAccessibleTraceFolder(QString (*folder)()) ;
void accessibleTrace(const QString &line) ;

#endif // DIALOGFOCUS_H
