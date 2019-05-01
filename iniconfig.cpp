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

IniConfig::IniConfig(IniConfig &other)
{
    qFatal("FATAL: IniConfig copied or not passed to function by ref") ;
}

IniConfig::~IniConfig()
{
    if (settings) delete settings ;
}

bool IniConfig::init(QString inifile)
{
    settings = new QSettings(inifile, QSettings::IniFormat);
    if (!settings) return false ;
    if (numSections()!=0) {
        path = QFileInfo(inifile).canonicalPath() ;
        return true ;
    } else {
        delete settings ;
        settings=NULL ;
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
    if (!settings || num<0 || num>=settings->childGroups().length()) {
        return QString("") ;
    } else {
        return settings->childGroups().at(num) ;
    }
}

int IniConfig::numSections()
{
    if (!settings) return 0 ;
    else return settings->childGroups().length() ;
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
    QString result = QString("") ;
    QString path = section + QString("/") + key ;
    if (!settings) {
        result = "ERROR" ;
    } else {
        QVariant var = settings->value(path) ;
        if (var.isValid()) result = var.toString() ;
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
    if (settings) {
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
    }
    return src ;

}

// TODO: GetIcon
