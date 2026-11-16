#ifndef DIALOGFOCUS_H
#define DIALOGFOCUS_H

#include <QString>

class QDialog ;

// Dialogs and the screen reader.  A dialog must be the window that the screen reader follows, with its own control
// focused, and it must stay in front of the program's main window.
//
// This is off until a program calls installDialogFocusGuard() (ContactManager does, from main), so a program that does not
// call it gets the dialogs it always had: exec() with the dialog's own parent.  When it is on, execFocused() makes the dialog
// a window of its own (not nested under its parent in the accessibility tree), shows it, makes it the active window with its
// focus control focused, and tells the screen reader which control that is.
void installDialogFocusGuard() ;
bool dialogFocusEnabled() ;

// Runs a modal dialog, in place of exec().  The caller still deletes the dialog if it created it with new.
int execFocused(QDialog *dlg) ;

// Tracing, for finding a fault with a screen reader.  Off until setAccessibleTrace(true).  Each step is written to
// accessibility-debug.txt in the folder given by setAccessibleTraceFolder (if a folder function is set), and to the debug output.
void setAccessibleTrace(bool on) ;
void setAccessibleTraceFolder(QString (*folder)()) ;
void accessibleTrace(const QString &line) ;

#endif // DIALOGFOCUS_H
