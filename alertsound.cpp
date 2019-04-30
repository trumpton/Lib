#include "alertsound.h"

#include <QSound>
#include <QApplication>
#include <QDir>

QSound *sound_ok ;
QSound *sound_alert ;
QSound *sound_error ;
QSound *sound_query ;

QSound *sound_disabled ;

QSound *sound_notfound ;
QSound *sound_found ;
QSound *sound_wrapped ;

QSound *sound_fileopen ;
QSound *sound_filesave ;

static int sound_initialised = 0 ;

void initSound()
{
    sound_ok = new QSound(":/sounds/ok.wav") ;
    sound_alert = new QSound(":/sounds/alert.wav") ;
    sound_query = new QSound(":/sounds/query.wav") ;
    sound_error = new QSound(":/sounds/error.wav") ;

    sound_disabled = new QSound(":/sounds/disabled.wav") ;

    sound_found = new QSound(":/sounds/found.wav") ;
    sound_notfound = new QSound(":/sounds/notfound.wav") ;
    sound_wrapped = new QSound(":/sounds/wrapped.wav") ;

    sound_fileopen = new QSound(":/sounds/fileopen.wav") ;
    sound_filesave = new QSound(":/sounds/filesave.wav") ;

    sound_initialised = 1 ;
}

void play(enum soundType type)
{
    if (sound_initialised==0) initSound() ;

    QSound *sound ;
    switch (type) {
    case Alert: sound = sound_alert ; break ;
    case Error: sound = sound_error ; break ;
    case Query: sound = sound_query ; break ;
    case Disabled: sound = sound_disabled ; break ;
    case Found: sound = sound_found ; break ;
    case NotFound: sound = sound_notfound ; break ;
    case Wrapped: sound = sound_wrapped ; break ;
    case FileOpen: sound = sound_fileopen ; break ;
    case FileSave: sound = sound_filesave ; break ;
    default:
    case Ok:
        sound = sound_ok ; break ;
    }
    sound->play() ;
}

void closeSound()
{
    if (sound_initialised!=0) {
        if (sound_alert) delete sound_alert ;
        if (sound_error) delete sound_error ;
        if (sound_query) delete sound_query ;
        if (sound_disabled) delete sound_disabled ;
        if (sound_found) delete sound_found ;
        if (sound_notfound) delete sound_notfound ;
        if (sound_wrapped) delete sound_wrapped ;
        if (sound_fileopen) delete sound_fileopen ;
        if (sound_filesave) delete sound_filesave ;
        sound_initialised=0 ;
    }
}
