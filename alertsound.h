#ifndef ALERTSOUND_H
#define ALERTSOUND_H

#include <QObject>

enum soundType {
    Ok, Alert, Error, Query, Disabled, Found, NotFound, Wrapped, FileOpen, FileSave, Sent,
    SwitchOn, SwitchOff    // a list shows only one contact's entries, or all of them again
};

void initSound(QObject *parent) ;
void closeSound() ;
void play(enum soundType type) ;

#endif // ALERTSOUND_H

