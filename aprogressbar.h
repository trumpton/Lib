#ifndef APROGRESSBAR_H
#define APROGRESSBAR_H

#include <QProgressBar>

// AProgressBar: a QProgressBar, for the accessibility of ContactManager and Lib.  It has no change of its own yet.  Every change to how
// these controls are read is made here, so that it applies to every ProgressBar in the program.  Use it where a QProgressBar would be used.
class AProgressBar : public QProgressBar
{
public:
    using QProgressBar::QProgressBar ;
} ;

#endif // APROGRESSBAR_H
