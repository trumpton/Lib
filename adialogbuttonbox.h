#ifndef ADIALOGBUTTONBOX_H
#define ADIALOGBUTTONBOX_H

#include <QDialogButtonBox>

// ADialogButtonBox: a QDialogButtonBox, for the accessibility of ContactManager and Lib.  It has no change of its own yet.  Every change to how
// these controls are read is made here, so that it applies to every DialogButtonBox in the program.  Use it where a QDialogButtonBox would be used.
class ADialogButtonBox : public QDialogButtonBox
{
public:
    using QDialogButtonBox::QDialogButtonBox ;
} ;

#endif // ADIALOGBUTTONBOX_H
