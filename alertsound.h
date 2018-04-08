#ifndef ALERTSOUND_H
#define ALERTSOUND_H

enum soundType {
    Ok, Alert, Error, Query, Disabled, Found, NotFound, Wrapped, FileOpen, FileSave
};

void initSound() ;
void play(enum soundType type) ;

#endif // ALERTSOUND_H

