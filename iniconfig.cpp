#include "iniconfig.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QApplication>

IniConfig::IniConfig()
{
    settings = NULL ;
    selectedSection = "" ;
    path = "" ;
}

IniConfig::~IniConfig()
{
    if (settings) delete settings ;
}

bool IniConfig::init(QString inifile)
{
    settings = new QSettings(inifile, QSettings::IniFormat);
    if (numSections()!=0) {
        path = QFileInfo(inifile).canonicalPath() ;
        return true ;
    } else {
        path="" ;
        return false ;
    }
}

QString& IniConfig::canonicalPath()
{
    return path ;
}

bool IniConfig::selectSection(QString section)
{
    selectedSection=section ;
    return true ;
}

QString IniConfig::getSection(int num)
{
    if (num<0 || num>=settings->childGroups().length()) {
        return QString("") ;
    } else {
        return settings->childGroups().at(num) ;
    }
}

int IniConfig::numSections()
{
    return settings->childGroups().length() ;
}

QString IniConfig::get(QString key)
{
    QString result ;
    if (settings==NULL) result = "ERROR" ;
    else result = settings->value(selectedSection + "/" + key).toString() ;
    return ExpandVars(result) ;
}

QString IniConfig::get(QString section, QString key)
{
    QString result ;
    if (settings==NULL) result = "ERROR" ;
    else result = settings->value(section + "/" + key).toString() ;
    return ExpandVars(result) ;
}


bool IniConfig::isset(QString section, QString key)
{
    if (get(section, key).toLower().contains("true") || get(key).toLower().contains("yes")) return true ;
    else return false ;
}

bool IniConfig::isset(QString key)
{
    if (get(key).toLower().contains("true") || get(key).toLower().contains("yes")) return true ;
    else return false ;
}

QString IniConfig::ExpandVars(QString src)
{
    // Expand entries from 'var' section - loop until nothing more to change
    // And include count to trap VAR=$VAR

    bool changed ;
    int count=0 ;
    do {
        changed=false ;
        count++ ;
        settings->beginGroup("var") ;
        const QStringList childKeys = settings->childKeys();
        foreach (const QString &childKey, childKeys) {
            // TODO: Get childKey to the right of the slash
            QString var = QString("$") + childKey.toLatin1().data() ;
            QString value = settings->value(childKey).toString() ;
            QString oldsrc = src ;
            src = src.replace(var, value) ;
            if (oldsrc.compare(src)!=0) changed=true ;
        }
        settings->endGroup() ;
    } while (changed && count<100) ;

    // Now, include the standard entries

    src = src.replace("$CONFIG", QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/TrumptonApps") ;
    src = src.replace("$APPDIR", QApplication::applicationDirPath()) ;
    src = src.replace("$HOME", QStandardPaths::writableLocation(QStandardPaths::HomeLocation)) ;
    src = src.replace("$INI", canonicalPath()) ;

    return src ;
}

// TODO: GetIcon

/*
#include "iniconfig.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QApplication>

IniConfig::IniConfig()
{
    settings = NULL ;
    selectedSection = "" ;
    path = "" ;
}

IniConfig::~IniConfig()
{
    if (settings) delete settings ;
}

bool IniConfig::init(QString inifile)
{
    settings = new QSettings(inifile, QSettings::IniFormat);
    if (!settings) {
        path="" ;
        return false ;
    } else {
        if (numSections()!=0) {
            path = QFileInfo(inifile).canonicalPath() ;
            return true ;
        } else {
            path="" ;
            if (settings) delete settings ;
            return false ;
        }
    }
}

QString& IniConfig::canonicalPath()
{
    return path ;
}

bool IniConfig::selectSection(QString section)
{
    selectedSection=section ;
    return true ;
}

QString IniConfig::getSection(int num)
{
    if (!settings) {
        return "ERROR" ;
    } else {
        if (num<0 || num>=settings->childGroups().length()) {
            return QString("") ;
        } else {
            return settings->childGroups().at(num) ;
        }
    }
}

int IniConfig::numSections()
{
    return settings->childGroups().length() ;
}

QString IniConfig::get(QString key)
{
    return get(selectedSection, key) ;
}

QString IniConfig::get(QString section, QString key)
{
    QString result ;
    if (!settings) {
        result = "ERROR" ;
    } else {
        if (settings->contains(section + "/" + key)) {
            result = settings->value(section + "/" + key, QVariant(QString(""))).toString() ;
        } else {
            result = "" ;
        }
    }
    return ExpandVars(result) ;
}


bool IniConfig::isset(QString section, QString key)
{
    if (get(section, key).toLower().contains("true") || get(key).toLower().contains("yes")) return true ;
    else return false ;
}

bool IniConfig::isset(QString key)
{
    if (get(key).toLower().contains("true") || get(key).toLower().contains("yes")) return true ;
    else return false ;
}

QString IniConfig::ExpandVars(QString src)
{
    // Expand entries from 'var' section - loop until nothing more to change
    // And include count to trap VAR=$VAR

    bool changed ;
    int count=0 ;
    do {
        changed=false ;
        count++ ;
        settings->beginGroup("var") ;
        const QStringList childKeys = settings->childKeys();
        foreach (const QString &childKey, childKeys) {
            // TODO: Get childKey to the right of the slash
            QString var = QString("$") + childKey.toLatin1().data() ;
            QString value = settings->value(childKey).toString() ;
            QString oldsrc = src ;
            src = src.replace(var, value) ;
            if (oldsrc.compare(src)!=0) changed=true ;
        }
        settings->endGroup() ;
    } while (changed && count<100) ;

    // Now, include the standard entries

    src = src.replace("$CONFIG", QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/TrumptonApps") ;
    src = src.replace("$APPDIR", QApplication::applicationDirPath()) ;
    src = src.replace("$HOME", QStandardPaths::writableLocation(QStandardPaths::HomeLocation)) ;
    src = src.replace("$INI", canonicalPath()) ;

    return src ;
}

// TODO: GetIcon
*/
