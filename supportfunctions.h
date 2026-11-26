#ifndef SUPPORTFUNCTIONS_H
#define SUPPORTFUNCTIONS_H

#include <QString>
#include <QDateTime>
#include <QWidget>
#include <QLineEdit>
#include <QDebug>

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

// Parse a free-text duration into minutes
int durationstringToInt(QString str) ;
QString& durationintToString(int mins) ;

// Remove accents
QString& deAccent(QString& str) ;

// File Read and Write
bool writeToFile(QString filename, QString data, bool append=false) ;
bool readFromFile(QString filename, QString &buffer, QString codec=QString("UTF-8"), QString altcodec=QString("System")) ;

// File and Directory Processing
bool copyFile(QString from, QString to) ;
bool fileExists(QString filename) ;
bool directoryExists(QString directory) ;
bool parseDirectory(QString& directory, QStringList& into, QString extension, bool reversesort=false, bool clear=true) ;
bool deleteDirectoryAndFiles(QString directory, QString fileextension) ;

// Save a copy of the file in the deleted subdirectory of path
bool backupFile(QString& path, QString& filename) ;


// Present a warning form and OK button, or YES/NO buttons
// The application's name starts the title of every dialog, as "Contact Manager - Setup".  The application sets it at start-up
// with setDialogAppName(); until then the application's name is used.
void setDialogAppName(const QString &name) ;

// The name line of a results list: the application's name and the query type, as "Contact Manager: Global Find", right-padded
// with spaces to 40 characters.
QString resultsName(const QString &query) ;
// The title of a dialog for a task, as "Contact Manager - Setup".  An empty task gives the application's name alone.  The Lib
// dialogs below take the task as their title, and add the application's name.
QString dialogTitle(const QString &task) ;
// The accessible name of a field, from its visible label: "Start:" from "Start", "Start:" or "Start?".  Accelerator marks are removed.
QString fieldName(const QString &label) ;

bool warningYesNoDialog(QWidget *parent, QString title, QString message, QString yestext = QString("Yes"), QChar yeskey = QChar('y'), QString notext = QString("No"), QChar nokey = QChar('n')) ;

// As above, with a checkbox to the left of the buttons.  checked is set to the state of the checkbox when the
// dialog closes.  Returns true for yes, as above.
bool warningYesNoDialog(QWidget *parent, QString title, QString message, QString yestext, QChar yeskey, QString notext, QChar nokey, QString checktext, bool checkdefault, bool &checked) ;
bool warningOkDialog(QWidget *parent, QString title, QString message, bool iserror=false, bool beep=true) ;
bool errorOkDialog(QWidget *parent, QString title, QString message) ;
// An error message, as QMessageBox::critical with its OK button, run with the focus handling (dialogfocus.h)
void criticalDialog(QWidget *parent, QString title, QString message) ;
// A question that must be answered on purpose: the focus starts on the Yes button (so Space answers it), Enter accepts and
// Escape cancels.  spokenMessage is what the screen reader says for the message (empty: the message itself).
bool deliberateYesNoDialog(QWidget *parent, QString title, QString message, QString spokenMessage, QString yestext, QChar yeskey,
                           QString notext, QChar nokey, QString yesdescription = QString(), QString nodescription = QString()) ;
// A single line entry, with OK and Cancel.  The entry is the Lib line edit: the text is not selected, the cursor is at the end, and
// typing does not replace a selection.  accessibleName is the prompt when it is empty.
QString inputDialog(QWidget *parent, QString title, QString prompt, QLineEdit::EchoMode echo, QString defaulttext, bool *ok,
                    QString accessibleName = QString(), QString accessibleDescription = QString()) ;
// The text box of a Find or Global Find: the same dialog, but defaulttext (the last search) is selected when it opens, with the
// cursor at its end, and typing replaces it (ASearchLineEdit).  The name and description are those of inputDialog.
QString searchInputDialog(QWidget *parent, QString title, QString prompt, QString defaulttext, bool *ok) ;

// Manage logging
QString& getLog() ;
void addLog(QString entry) ;

// Zip
bool zip(QString contactmanagerbase, QString destinationfile) ;

// sleep
void qSleep(int ms) ;

// debug to console
void setupdebug() ;

#ifndef QT_NO_DEBUG_OUTPUT
#define dbg(s) qDebug() << s ; fflush(stderr) ;
#else
#define dbg(s)
#endif

#endif // SUPPORTFUNCTIONS_H
