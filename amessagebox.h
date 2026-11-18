#ifndef AMESSAGEBOX_H
#define AMESSAGEBOX_H

#include <QMessageBox>

// AMessageBox: a QMessageBox, for the accessibility of ContactManager and Lib.  It has no change of its own yet.  Every change to how
// these controls are read is made here, so that it applies to every MessageBox in the program.  Use it where a QMessageBox would be used.
class AMessageBox : public QMessageBox
{
public:
    using QMessageBox::QMessageBox ;
} ;

#endif // AMESSAGEBOX_H
