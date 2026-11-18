#ifndef ACHECKBOX_H
#define ACHECKBOX_H

#include <QCheckBox>

// ACheckBox: a QCheckBox, for the accessibility of ContactManager and Lib.  It has no change of its own yet.  Every change to how
// these controls are read is made here, so that it applies to every CheckBox in the program.  Use it where a QCheckBox would be used.
class ACheckBox : public QCheckBox
{
public:
    using QCheckBox::QCheckBox ;
} ;

#endif // ACHECKBOX_H
