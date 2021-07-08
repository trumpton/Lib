#include "alertsound.h"

#include <QSoundEffect>
#include <QUrl>

#include <QApplication>
#include <QDir>

QSoundEffect *sound_ok ;
QSoundEffect *sound_alert ;
QSoundEffect *sound_error ;
QSoundEffect *sound_query ;

QSoundEffect *sound_disabled ;

QSoundEffect *sound_notfound ;
QSoundEffect *sound_found ;
QSoundEffect *sound_wrapped ;

QSoundEffect *sound_fileopen ;
QSoundEffect *sound_filesave ;

QSoundEffect *sound_sent ;

static int sound_initialised = 0 ;

void initSound(QObject *parent = nullptr)
{
    sound_ok = new QSoundEffect(parent) ;
    sound_ok->setSource(QUrl("qrc:/sounds/ok.wav")) ;

    sound_alert = new QSoundEffect(parent) ;
    sound_alert->setSource(QUrl("qrc:/sounds/alert.wav")) ;

    sound_query = new QSoundEffect(parent) ;
    sound_query->setSource(QUrl("qrc:/sounds/query.wav")) ;

    sound_error = new QSoundEffect(parent) ;
    sound_error->setSource(QUrl("qrc:/sounds/error.wav")) ;

    sound_disabled = new QSoundEffect(parent) ;
    sound_disabled->setSource(QUrl("qrc:/sounds/disabled.wav")) ;

    sound_found = new QSoundEffect(parent) ;
    sound_found->setSource(QUrl("qrc:/sounds/found.wav")) ;

    sound_notfound = new QSoundEffect(parent) ;
    sound_notfound->setSource(QUrl("qrc:/sounds/notfound.wav")) ;

    sound_wrapped = new QSoundEffect(parent) ;
    sound_wrapped->setSource(QUrl("qrc:/sounds/wrapped.wav")) ;

    sound_fileopen = new QSoundEffect(parent) ;
    sound_fileopen->setSource(QUrl("qrc:/sounds/fileopen.wav")) ;

    sound_filesave = new QSoundEffect(parent) ;
    sound_filesave->setSource(QUrl("qrc:/sounds/filesave.wav")) ;

    sound_sent = new QSoundEffect(parent) ;
    sound_sent->setSource(QUrl("qrc:/sounds/sent.wav")) ;

    sound_initialised = 1 ;
}

void play(enum soundType type)
{
    if (sound_initialised==0) initSound() ;

    QSoundEffect *sound ;
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
    case Sent: sound = sound_sent ; break ;
    default:
    case Ok:
        sound = sound_ok ; break ;
    }
    sound->setVolume(1) ;
    sound->setMuted(false) ;
    sound->play() ;
    QSoundEffect::Status status2 = sound->status() ;
    return ;
}

void closeSound()
{
  if (sound_initialised==1) {
      delete sound_alert ;
      delete sound_error ;
      delete sound_query ;
      delete sound_disabled ;
      delete sound_found ;
      delete sound_notfound ;
      delete sound_wrapped ;
      delete sound_fileopen ;
      delete sound_filesave ;
      delete sound_sent ;
      sound_initialised=0 ;
  }
}
