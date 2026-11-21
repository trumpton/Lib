#ifndef APROGRESSDIALOG_H
#define APROGRESSDIALOG_H

#include <QProgressDialog>

// AProgressDialog: a QProgressDialog, for the accessibility of ContactManager, EasyNotepad, ParseWeb and Lib.  Every change to
// how progress dialogs are shown is made here.
class AProgressDialog : public QProgressDialog
{
public:
    using QProgressDialog::QProgressDialog ;

    // Shows the dialog with its cancel button focused, as AProgressInfo does (its button is focused once it is shown)
    void showProgress() ;

    // Sets the bar, then lets the window handle events, so that the screen reader can read the new value.  See
    // documents/gotchas.md 6.14.  The operation calls this for each step.
    void setValue(int value) ;

private:
    int lastYieldValue = 0 ;   // the value at the last yield, for the step rule (see setValue)
} ;

#endif // APROGRESSDIALOG_H
