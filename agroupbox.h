#ifndef AGROUPBOX_H
#define AGROUPBOX_H

#include <QGroupBox>

// AGroupBox: a QGroupBox, for the accessibility of ContactManager and Lib.  It has no change of its own yet.  Every change to how
// these controls are read is made here, so that it applies to every GroupBox in the program.  Use it where a QGroupBox would be used.
class AGroupBox : public QGroupBox
{
public:
    using QGroupBox::QGroupBox ;
} ;

#endif // AGROUPBOX_H
