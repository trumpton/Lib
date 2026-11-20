#include "yieldtoevents.h"

#include <QEventLoop>
#include <QTimer>

void yieldToEvents(int ms)
{
    QEventLoop loop ;
    QTimer::singleShot(ms, &loop, &QEventLoop::quit) ;
    loop.exec() ;
}
