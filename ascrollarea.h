#ifndef ASCROLLAREA_H
#define ASCROLLAREA_H

#include <QScrollArea>

// AScrollArea: a QScrollArea, for the accessibility of ContactManager and Lib.  It has no change of its own yet.  Every change to how
// these controls are read is made here, so that it applies to every ScrollArea in the program.  Use it where a QScrollArea would be used.
class AScrollArea : public QScrollArea
{
public:
    using QScrollArea::QScrollArea ;
} ;

#endif // ASCROLLAREA_H
