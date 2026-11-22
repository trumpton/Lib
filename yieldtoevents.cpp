#include "yieldtoevents.h"

#include <QAccessible>
#include <QEventLoop>
#include <QTimer>

// TEST: the yield also sends a value change for the widget (the progress bar), and the event loop is not run
void yieldToEvents(int ms, QWidget *widget, int value)
{
    if (widget) {
        QAccessibleValueChangeEvent event(widget, QVariant(value)) ;
        QAccessible::updateAccessibility(&event) ;
    }
    QEventLoop loop ;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit) ;
    // TEST: temporarily disabled
    // loop.exec() ;
}
