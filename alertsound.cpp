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
    QString path = QApplication::applicationDirPath() + QDir::toNativeSeparators("/") + QString("sounds") + QDir::toNativeSeparators("/") ;

    sound_ok = new QSound(path + "ok.wav") ;
    sound_alert = new QSound(path + "alert.wav") ;
    sound_query = new QSound(path + "query.wav") ;
    sound_error = new QSound(path + "error.wav") ;

    sound_disabled = new QSound(path + "disabled.wav") ;

    sound_found = new QSound(path + "found.wav") ;
    sound_notfound = new QSound(path + "notfound.wav") ;
    sound_wrapped = new QSound(path + "wrapped.wav") ;

    sound_fileopen = new QSound(path + "fileopen.wav") ;
    sound_filesave = new QSound(path + "filesave.wav") ;

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
