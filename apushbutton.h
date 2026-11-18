#ifndef APUSHBUTTON_H
#define APUSHBUTTON_H

#include <QPushButton>

// APushButton: a QPushButton, for the accessibility of ContactManager and Lib.  It has no change of its own yet.  Every change to how
// these controls are read is made here, so that it applies to every PushButton in the program.  Use it where a QPushButton would be used.
class APushButton : public QPushButton
{
public:
    using QPushButton::QPushButton ;
} ;

#endif // APUSHBUTTON_H
