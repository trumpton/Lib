#ifndef YIELDTOEVENTS_H
#define YIELDTOEVENTS_H

#include <QElapsedTimer>
#include <QWidget>

// How long a progress window is given to handle events each time it yields, so that the screen reader can read the window, and the
// progress tone plays (documents/gotchas.md 6.14).
const int PROGRESS_YIELD_MS = 10 ;

// A progress window yields after a progress update only when this much time has passed since its last yield (see yieldIfStepDue).
const int PROGRESS_STEP_MS = 100 ;

// The time a progress window is given after its button takes the focus, before the operation writes to it (documents/gotchas.md 6.16).
const int PROGRESS_SETTLE_MS = 250 ;

// Runs the event loop for ms, so that the window handles events while it waits: a sleep that keeps the window working.  Qt timers
// count whole milliseconds, so a shorter time is not possible (documents/gotchas.md 6.16).
void yieldToEvents(int ms = PROGRESS_YIELD_MS, QWidget *widget = nullptr, int value = 0) ;

// Yields for PROGRESS_YIELD_MS when more than PROGRESS_STEP_MS has passed since the last yield, and restarts the clock at each yield.
// A clock that has not been started yields at once.
inline void yieldIfStepDue(QElapsedTimer &clock, QWidget *widget = nullptr, int value = 0)
{
    if (clock.isValid() && clock.elapsed() <= PROGRESS_STEP_MS) return ;
    yieldToEvents(PROGRESS_YIELD_MS, widget, value) ;
    clock.start() ;
}

#endif // YIELDTOEVENTS_H
