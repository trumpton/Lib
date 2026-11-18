#ifndef ADATEEDIT_H
#define ADATEEDIT_H

#include <QDateEdit>

// ADateEdit: a QDateEdit, for the accessibility of ContactManager and Lib.  It has no change of its own yet.  Every change to how
// these controls are read is made here, so that it applies to every DateEdit in the program.  Use it where a QDateEdit would be used.
class ADateEdit : public QDateEdit
{
public:
    using QDateEdit::QDateEdit ;
} ;

#endif // ADATEEDIT_H
