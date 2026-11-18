#ifndef ALABEL_H
#define ALABEL_H

#include <QLabel>

// ALabel: a QLabel, for the accessibility of ContactManager and Lib.  It has no change of its own yet.  Every change to how
// these controls are read is made here, so that it applies to every Label in the program.  Use it where a QLabel would be used.
class ALabel : public QLabel
{
public:
    using QLabel::QLabel ;
} ;

#endif // ALABEL_H
