#ifndef ACOMBOBOX_H
#define ACOMBOBOX_H

#include <QComboBox>

// AComboBox: a QComboBox, for the accessibility of ContactManager and Lib.  It has no change of its own yet.  Every change to how
// these controls are read is made here, so that it applies to every ComboBox in the program.  Use it where a QComboBox would be used.
class AComboBox : public QComboBox
{
public:
    using QComboBox::QComboBox ;
} ;

#endif // ACOMBOBOX_H
