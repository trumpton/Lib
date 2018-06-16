#include "supportfunctions.h"
#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QRegExp>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QProcess>
#include <QEventLoop>
#include <QTextCodec>
#include <QInputDialog>

#include "warningyesno.h"
#include "warningok.h"
#include "alertsound.h"

#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif


QString supportFunctionLog ;

QString& getLog()
{
    return supportFunctionLog ;
}

void addLog(QString entry)
{
    QDateTime t = QDateTime::currentDateTime() ;
    QString times=t.toString("hh:mm:ss") + ": " ;
    supportFunctionLog = supportFunctionLog + times + entry.trimmed() + "\n" ;
}

//
// File Processing
//
bool parseDirectory(QString& directory, QStringList& into, QString extension, bool reversesort, bool clear)
{
    if (clear) into.clear() ;
    if (extension.isEmpty()) {
        QDir dir(directory) ;
        dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot) ;
        into = into + dir.entryList() ;
    } else {
        QDir dir(directory) ;
        extension = extension.replace(".", "") ;
        QString ext = QString("*.") + extension ;
        QStringList exts(ext) ;
        dir.setFilter(QDir::Files) ;
        if (reversesort) dir.setSorting(QDir::Name|QDir::Reversed) ;
        else dir.setSorting(QDir::Name) ;
        dir.setNameFilters(exts) ;
        into = into + dir.entryList() ;
/* Remove Extensions
        QString regexpstr = "\\." + extension + "$" ;
        for (int i=0, sz=into.size(); i<sz; i++) {
            QString filename = into[i] ;
            filename.replace(QRegExp(regexpstr), "") ;
            into[i] = filename ;
        }
*/
    }
    return true ;
}

bool writeToFile(QString filename, QString data, bool append, QString codec)
{
    QFile file(filename);
    if (append) {
        if (!file.open(QIODevice::Append | QIODevice::Text))
            return false;
    } else {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return false;
    }

    bool ok ;
    QTextStream out(&file);
    out.setCodec(codec.toLatin1().data());
    out.setGenerateByteOrderMark(true) ;
    out << data ;
    ok = file.flush() ;
    file.close() ;
    return ok ;
}

bool writeByteArrayToFile(QString filename, QByteArray data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    bool ok ;
    QTextStream out(&file);
    out << data ;
    ok = file.flush() ;
    file.close() ;
    return ok ;
}


bool readFromFile(QString filename, QString& buffer, QString codec, QString altcodec)
{
    buffer="" ;

    // Open the file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        return false ;

    } else {

        QByteArray filedata=file.readAll() ;
        file.close() ;

        QTextCodec::ConverterState state;
        QTextCodec *c = QTextCodec::codecForName(codec.toLatin1().data());
        buffer = c->toUnicode(filedata.constData(), filedata.size(), &state);
        if (state.invalidChars > 0) {
            if (warningYesNoDialog(0, "File Read Problem",
                  QString("The file does not appear to be in ") + codec +
                  QString(" format, shall I try again, assuming it is ") + altcodec)) {
                c = QTextCodec::codecForName(altcodec.toLatin1().data()) ;
                buffer = c->toUnicode(filedata.constData(), filedata.size(), &state);
            }
        }
    }

    return true ;
}

bool fileExists(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    file.close() ;
    return true ;
}

bool directoryExists(QString directory)
{
    QDir dir ;
    dir.setPath(directory) ;
    return dir.exists() ;
}

bool copyFile(QString from, QString to)
{
    QFile fromfile(from), tofile(to);
    if (!fromfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false ;
    }
    if (!tofile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        fromfile.close() ;
        return false;
    }
    QTextStream in(&fromfile), out(&tofile);
    out << in.readAll() ;
    tofile.close() ;
    fromfile.close() ;
    return true ;
}

bool deleteDirectoryAndFiles(QString directory, QString fileextension)
{
    QStringList files ;
    if (parseDirectory(directory, files, fileextension)) {
        for (int i=0, sz=files.size(); i<sz; i++) {
            QFile::remove(directory + "/" + files.at(i) + "." + fileextension) ;
        }
        QDir dir ;
        dir.rmdir(directory) ;
    }
    bool success = (!directoryExists(directory)) ;
    return success ;
}


bool backupFile(QString& path, QString& filename)
{
    QString src = path + filename ;
    QString dstdir = path + "deleted" ;
    QString dst = dstdir + "/" + filename ;

    if (!QDir(dstdir).exists()) QDir().mkdir(dstdir) ;
    QFile::remove(dst) ;
    QFile::rename(src, dst) ;

    return true ;
}


//
// Conversion to/from DateTime to String
//
QDateTime& isoStringToDateTime(QString str)
{
    static QDateTime t ;
    t.setTimeSpec(Qt::UTC) ;
    t = QDateTime::fromString(str, Qt::ISODate) ;
    return t ;
}

QString& dateTimeToIsoString(QDateTime &t)
{
    static QString response ;
    response = t.toTimeSpec(Qt::UTC).toString(Qt::ISODate) ;
    return response ;
}


QString& nowToIsoString()
{
    static QString response ;
    QDateTime t = QDateTime::currentDateTime() ;
    QDateTime z = t.toTimeSpec(Qt::UTC) ;
    response = z.toString(Qt::ISODate) ;
    return response ;
}

QString& dateTimeStringToZulu(QString str)
{
    static QString zulu ;
    QDateTime t = QDateTime::fromString(str, Qt::ISODate) ;
    QDateTime z = t.toTimeSpec(Qt::UTC) ;
    zulu = z.toString(Qt::ISODate) ;
    return zulu ;
}


//
// Parsing free-text durations to miutes
//
int durationstringToInt(QString str)
{
    static int response ;
    QRegExp rx ;
    rx.setMinimal(true) ;

    response = 0 ;
    str = str.trimmed() ;

    // 1 week
    rx.setPattern("^([0-9]+) *[wW][a-zA-Z]*$") ;
    if (response==0 && rx.indexIn(str)>=0) {
        response = rx.cap(1).toInt()*7*24*60 ;
    }

    // 1 day
    rx.setPattern("^([0-9]+) *[dD][a-zA-Z]*$") ;
    if (response==0 && rx.indexIn(str)>=0) {
        response = rx.cap(1).toInt()*24*60 ;
    }

    // 1.5 days
    rx.setPattern("^([0-9]+)\\.([0-9]+) *[dD][a-zA-Z]*$") ;
    if (response==0 && rx.indexIn(str)>=0) {
        int point = rx.cap(2).toInt() ;
        if (point<10) point*=10 ;
        if (point>99) point=100 ;
        response = rx.cap(1).toInt()*24*60 + (point*24*60)/100 ;
    }

    // 1 day 30 minutes
    rx.setPattern("^([0-9]+) *[dD][a-zA-Z]* *([0-9]+) *[mM][a-zA-Z]*$") ;
    if (response==0 && rx.indexIn(str)>=0) {
        response = rx.cap(1).toInt()*60*24 + rx.cap(2).toInt() ;
    }


    // 1 day 5 hours 30 minutes
    rx.setPattern("^([0-9]+) *[dD][a-zA-Z]* *([0-9]+) *[hH][a-zA-Z]* *([0-9]+).*$") ;
    if (response==0 && rx.indexIn(str)>=0) {
        response = rx.cap(1).toInt()*60*24 + rx.cap(2).toInt()*60 + rx.cap(3).toInt() ;
    }


    // 1 hour
    rx.setPattern("^([0-9]+) *[hH][.*][a-zA-Z]*$") ;
    if (response==0 && rx.indexIn(str)>=0) {
        response = rx.cap(1).toInt()*60 ;
    }

    // 1 hour 30 minutes / 1 hour 30 / 1h30
    rx.setPattern("^([0-9]+) *[hH][a-zA-Z]* *([0-9]+).*$") ;
    if (response==0 && rx.indexIn(str)>=0) {
        response = rx.cap(1).toInt()*60 + rx.cap(2).toInt() ;
    }

    // 1.5 hours
    rx.setPattern("^([0-9]+)\\.([0-9]+) *[hH][a-zA-Z]*$") ;
    if (response==0 && rx.indexIn(str)>=0) {
        int point = rx.cap(2).toInt() ;
        if (point<10) point*=10 ;
        if (point>99) point=100 ;
        response = rx.cap(1).toInt() * 60 + (point*60)/100 ;
    }

    // 30 minutes
    rx.setPattern("^([0-9]+) *[mM][a-zA-Z]*$") ;
    if (response==0 && rx.indexIn(str)>=0) {
        response = rx.cap(1).toInt() ;
    }

    // 25 (minutes)
    rx.setPattern("^([0-9]+)$") ;
    if (response==0 && rx.indexIn(str)>=0) {
        response = rx.cap(1).toInt() ;
    }

    return response ;
}

QString& durationintToString(int mins)
{
    static QString result ;
    result="" ;

    if (mins==0) {

        result="reminder" ;

    } else {

        int d = (mins/(24*60)) ;
        int rh = (mins%(24*60)) ;

        if (d>0) result = result + QString::number(d) + " day" ;
        if (d>1) result = result + "s" ;

        int h = rh/60 ;
        int m = rh%60 ;

        if (d>0 && h>0) result = result + " " ;
        if (h>0) result = result + QString::number(h) + " hour" ;
        if (h>1) result = result + "s" ;

        if ((d>0 || h>0) && m>0) result = result + " " ;
        if (m>0) result = result + QString::number(m) + " minute" ;
        if (m>1) result = result + "s" ;

    }

    return result ;

}

//
// Convert xsdate to and from datetime
QDateTime& xsDateToDateTime(QString xsDate)
{
    static QDateTime response ;
    response = response.fromString("00:00:00 " + xsDate, "hh:mm:ss yyyy-MM-dd") ;
    if (response.isNull()) {
        response = response.fromString("00:00:00 0004" + xsDate, "hh:mm:ss yyyy--MM-dd") ;
    }
    if (response.isNull()) {
        response = response.fromString("00:00:00 0001-01-01", "hh:mm:ss yyyy-MM-dd") ;
    }
    return response ;
}

QString& dateTimeToXsDate(QDateTime datetime)
{
    static QString response ;
    if (datetime.date().year()==1) {
        response = "" ;
    } else if (datetime.date().year()==4) {
        response = datetime.toString("--MM-dd") ;
    } else {
        response = datetime.toString("yyyy-MM-dd") ;
    }
    return response ;
}

//
// Converting XS Format date to / from
//
QString& xsDateToDate(QString xsDate)
{
    static QString response ;
    int day=0, month=0, year=0 ;
    QRegExp rx ;
    rx.setMinimal(true) ;

    response = "" ;
    QString str = xsDate.trimmed() ;

    // YYYY-MM-DD
    rx.setPattern("^([0-9]{2,4})-([0-9]{1,2})-([0-9]{1,2})$") ;
    if (rx.indexIn(str)>=0) {
        year = rx.cap(1).toInt() ;
        month = rx.cap(2).toInt() ;
        day = rx.cap(3).toInt() ;
    }

    // MM-DD
    rx.setPattern("^([0-9]{1,2})-([0-9]{1,2})$") ;
    if (day==0) {
        year=0 ;
        month=rx.cap(1).toInt() ;
        day=rx.cap(2).toInt() ;
    }

    if (day!=0 && month!=0 && year!=0 && year!=1900) {
        response = response.sprintf("%02d/%02d/%04d", day, month, year) ;
    } else if (day!=0 && month!=0) {
        response = response.sprintf("%02d/%02d", day, month) ;
    } else {
        response = "" ;
    }

    return response ;
}

QString& dateToXsDate(QString date)
{
    static QString response ;
    int day=0, month=0, year=0 ;
    QRegExp rx ;
    rx.setMinimal(false) ;

    response = "" ;
    QString str = date.trimmed() ;

    // DD/MM/YYYY
    rx.setPattern("^([0-9]{1,2})[\\/\\- ]([0-9]{1,2})[\\/\\- ]([0-9]{2,4})$") ;
    if (rx.indexIn(str)>=0) {
        day = rx.cap(1).toInt() ;
        month = rx.cap(2).toInt() ;
        year = rx.cap(3).toInt() ;
    }

    // DD/MM
    rx.setPattern("^([0-9]{1,2})[\\/\\- ]([0-9]{1,2})$") ;
    if (rx.indexIn(str)>=0) {
        day = rx.cap(1).toInt() ;
        month = rx.cap(2).toInt() ;
        year = 1900 ;
    }

    if (day!=0 && month!=0 && year!=0) {
        response = response.sprintf("%04d-%02d-%02d", year, month, day) ;
    } else {
        response = "" ;
    }

    return response ;
}

//
// Zip
//
bool zip(QString contactmanagerbase, QString destinationfile)
{
    QString command = "zip" ;
    QStringList args ;
    args << "-r" <<  "-q" <<
            destinationfile <<
            contactmanagerbase + "/contact" <<
            contactmanagerbase + "/calendar" ;
    QProcess *myProcess = new QProcess(NULL) ;
    myProcess->start(command, args) ;
    return true ;
}


//
// Accessible pop-up dialog boxes
//
bool warningYesNoDialog(QWidget *parent, QString title, QString message, QString yestext, QChar yeskey, QString notext, QChar nokey)
{
    WarningYesNo yn(parent) ;
    yn.setWindowTitle(title) ;
    yn.setText(message) ;
    yn.setButtons(yestext, yeskey, notext, nokey);
    play(Query) ;
    return yn.exec()==QDialog::Accepted ;
}


bool warningOkDialog(QWidget *parent, QString title, QString message, bool iserror, bool beep)
{
    WarningOk yn(parent) ;
    yn.setWindowTitle(title) ;
    yn.setText(message) ;
    if (beep) {
        if (iserror) play(Error) ;
        else play(Query) ;
    }
    return yn.exec()==QDialog::Accepted ;
}

bool errorOkDialog(QWidget *parent, QString title, QString message)
{
    return warningOkDialog(parent, title, message, true) ;
}

QString inputDialog(QWidget *parent, QString title, QString prompt, QLineEdit::EchoMode echo, QString defaulttext, bool *ok)
{
    play(Query) ;
    return QInputDialog::getText(parent, title,  prompt, echo, defaulttext, ok) ;
}

QString& deAccent(QString& str)
{
    str.replace(QRegExp("[áàâä]"), "a");
    str.replace(QRegExp("[éèëê]"), "e");
    str.replace(QRegExp("[îï]"), "i");
    str.replace(QRegExp("[óôö]"), "o");
    str.replace(QRegExp("[œ]"), "oe");
    str.replace(QRegExp("[ùúûü]"), "u");
    str.replace(QRegExp("[ÿ]"), "y");
    str.replace(QRegExp("[ç]"), "c");
    str.replace(QRegExp("[ñ]"), "n");
    str.replace(QRegExp("[ß]"), "ss");
    return str ;
}



void qSleep(int ms)
{
#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}
