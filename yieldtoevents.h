#ifndef YIELDTOEVENTS_H
#define YIELDTOEVENTS_H

// The time a progress window is given to handle events after each step of a long operation, so that the screen reader can read the
// window, and the progress tone plays (documents/gotchas.md 6.14).
const int PROGRESS_YIELD_MS = 0 ;

// The time a progress window is given after its button takes the focus, before the operation writes to it (documents/gotchas.md 6.16).
const int PROGRESS_SETTLE_MS = 200 ;

// Runs the event loop for ms, so that the window handles events while it waits: a sleep that keeps the window working.  Qt timers
// count whole milliseconds, so a shorter time is not possible (documents/gotchas.md 6.16).
void yieldToEvents(int ms = PROGRESS_YIELD_MS) ;

#endif // YIELDTOEVENTS_H
