#ifndef ASPINBOX_H
#define ASPINBOX_H

#include <QSpinBox>

// ASpinBox: a QSpinBox, for the accessibility of ContactManager and Lib.  It has no change of its own yet.  Every change to how
// these controls are read is made here, so that it applies to every SpinBox in the program.  Use it where a QSpinBox would be used.
class ASpinBox : public QSpinBox
{
public:
    using QSpinBox::QSpinBox ;
} ;

#endif // ASPINBOX_H
