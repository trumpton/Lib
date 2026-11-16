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

// Tracing, for finding a fault with a screen reader.  Off until setAccessibleTrace(true).  Each step is written to
// accessibility-debug.txt in the folder given by setAccessibleTraceFolder (if a folder function is set), and to the debug output.
void setAccessibleTrace(bool on) ;
void setAccessibleTraceFolder(QString (*folder)()) ;
void accessibleTrace(const QString &line) ;

#endif // DIALOGFOCUS_H
