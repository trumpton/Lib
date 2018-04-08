#ifndef SUPPORTFUNCTIONS_H
#define SUPPORTFUNCTIONS_H

#include <QString>
#include <QDateTime>
#include <QWidget>
#include <QLineEdit>

QString& dateTimeStringToZulu(QString str) ;

// Parse a free-text string into a data/time
//QDateTime &dateStringToDateTime(QString str) ;
//QString& datetimeToDateString(QDateTime dt) ;

// Convert DateTime to/from ISO DateTime Strings
QString& dateTimeToIsoString(QDateTime &t) ;
QDateTime& isoStringToDateTime(QString str) ;
QString& nowToIsoString() ;

// Parse DD/MM/YYYY  (date) to and from YYYY-MM-DD (xs)
QString& xsDateToDate(QString xsDate) ;
QString& dateToXsDate(QString date) ;
QDateTime& xsDateToDateTime(QString xsDate) ;
QString& dateTimeToXsDate(QDateTime datetime) ;
QString& parseBackupDate(QString backupdate) ;

// Parse a free-text duration into minutes
int durationstringToInt(QString str) ;
QString& durationintToString(int mins) ;

// Remove accents
QString& deAccent(QString& str) ;

// File Read and Write
bool writeToFile(QString filename, QString data, bool append=false, QString codec=QString("UTF-8")) ;
bool readFromFile(QString filename, QString &buffer, QString codec=QString("UTF-8"), QString altcodec=QString("System")) ;

// File and Directory Processing
bool copyFile(QString from, QString to) ;
bool fileExists(QString filename) ;
bool directoryExists(QString directory) ;
bool parseDirectory(QString& directory, QStringList& into, QString extension, bool reversesort=false) ;
bool deleteDirectoryAndFiles(QString directory, QString fileextension) ;

// Save a copy of the file in the deleted subdirectory of path
bool backupFile(QString& path, QString& filename) ;


// Present a warning form and OK button, or YES/NO buttons
bool warningYesNoDialog(QWidget *parent, QString title, QString message, QString yestext = QString("Yes"), QChar yeskey = QChar('y'), QString notext = QString("No"), QChar nokey = QChar('n')) ;
bool warningOkDialog(QWidget *parent, QString title, QString message, bool iserror=false, bool beep=true) ;
bool errorOkDialog(QWidget *parent, QString title, QString message) ;
QString inputDialog(QWidget *parent, QString title, QString prompt, QLineEdit::EchoMode echo, QString defaulttext, bool *ok) ;

// Manage logging
QString& getLog() ;
void addLog(QString entry) ;

// Zip
bool zip(QString contactmanagerbase, QString destinationfile) ;

// sleep
void qSleep(int ms) ;


#endif // SUPPORTFUNCTIONS_H
