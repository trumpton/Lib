#ifndef APROGRESSDIALOG_H
#define APROGRESSDIALOG_H

#include <QProgressDialog>

// AProgressDialog: a QProgressDialog, for the accessibility of ContactManager, EasyNotepad, ParseWeb and Lib.  It has no change of
// its own yet.  Every change to how progress dialogs are shown is made here.
class AProgressDialog : public QProgressDialog
{
public:
    using QProgressDialog::QProgressDialog ;
} ;

#endif // APROGRESSDIALOG_H
