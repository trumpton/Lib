#ifndef ARADIOBUTTON_H
#define ARADIOBUTTON_H

#include <QRadioButton>

// ARadioButton: a QRadioButton, for the accessibility of ContactManager and Lib.  It has no change of its own yet.  Every change to how
// these controls are read is made here, so that it applies to every RadioButton in the program.  Use it where a QRadioButton would be used.
class ARadioButton : public QRadioButton
{
public:
    using QRadioButton::QRadioButton ;
} ;

#endif // ARADIOBUTTON_H
