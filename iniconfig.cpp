#include "iniconfig.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QApplication>

// The built-in variable names.  They are parsed first, and a [var] entry with one of these names is ignored.
static const QStringList builtinNames = { "HOME", "INI", "APPDIR" } ;

// The folder of the LibreOffice install, the one with program\python.exe in it, with forward slashes and no slash at the
// end (for example C:/Program Files (x86)/LibreOffice 5).  On Windows it comes from the UNO InstallPath registry key, which
// the 32-bit install writes under WOW6432Node, so both places are looked at.  On Linux it is /usr/lib/libreoffice when that
// is installed.  Empty when none is found.  A [var] entry called LIBREOFFICE overrides it.
static QString libreOfficeFolder()
{
    QString folder ;
#ifdef Q_OS_WIN
    const QStringList keys = { "HKEY_LOCAL_MACHINE\\SOFTWARE\\LibreOffice\\UNO\\InstallPath",
                               "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\LibreOffice\\UNO\\InstallPath" } ;
    for (const QString &key : keys) {
        QSettings reg(key, QSettings::NativeFormat) ;
        folder = reg.value(".").toString() ;
        if (!folder.isEmpty()) break ;
    }
    folder = folder.replace("\\", "/") ;
    while (folder.endsWith("/")) folder.chop(1) ;
    if (folder.endsWith("/program", Qt::CaseInsensitive)) folder.chop(QString("/program").length()) ;
#else
    if (QDir("/usr/lib/libreoffice/program").exists()) folder = "/usr/lib/libreoffice" ;
#endif
    return folder ;
}

// Replaces each $(NAME) in the text with its value, for every name in the map, once
static QString replaceVariables(QString text, const QMap<QString,QString> &vars)
{
    for (auto it = vars.constBegin(); it != vars.constEnd(); ++it)
        text.replace("$(" + it.key() + ")", it.value()) ;
    return text ;
}

// Replaces variables for at most IniConfig::PASSES passes, stopping early when a pass changes nothing
static QString expandPasses(QString text, const QMap<QString,QString> &vars)
{
    for (int pass=0 ; pass<IniConfig::PASSES ; pass++) {
        QString next = replaceVariables(text, vars) ;
        if (next==text) break ;
        text = next ;
    }
    return text ;
}

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
    if (!settings) return false ;
    if (numSections()!=0) {
        path = QFileInfo(inifile).canonicalPath() ;
        parseOnLoad() ;
        return true ;
    } else {
        delete settings ;
        settings=NULL ;
        path="" ;
        return false ;
    }
}

// Expands every variable once, in this order: the built-in names, then the [var] entries (up to PASSES passes), then every
// other value (up to PASSES passes, with the expanded [var] entries).
void IniConfig::parseOnLoad()
{
    values.clear() ;
    variables.clear() ;
    problems.clear() ;
    if (!settings) return ;

    QMap<QString,QString> builtin ;
    builtin["HOME"] = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) ;
    builtin["INI"] = path ;
    builtin["APPDIR"] = QApplication::applicationDirPath() ;
    // Automatic, but a [var] entry with this name overrides it (see below, where [var] entries replace these)
    builtin["LIBREOFFICE"] = libreOfficeFolder() ;

    // The [var] entries, without the built-in names
    QMap<QString,QString> raw ;
    settings->beginGroup("var") ;
    for (const QString &key : settings->childKeys()) {
        if (builtinNames.contains(key)) {
            problems << QString("[var] %1 is a built-in variable and is not overridden").arg(key) ;
            continue ;
        }
        raw[key] = settings->value(key).toString() ;
    }
    settings->endGroup() ;

    // Expand the [var] entries, with the built-in names, up to PASSES passes
    QMap<QString,QString> resolved = raw ;
    for (int pass=0 ; pass<PASSES ; pass++) {
        bool changed = false ;
        QMap<QString,QString> now = builtin ;
        for (auto it = resolved.constBegin() ; it != resolved.constEnd() ; ++it) now[it.key()] = it.value() ;
        for (auto it = resolved.begin() ; it != resolved.end() ; ++it) {
            QString next = replaceVariables(it.value(), now) ;
            if (next != it.value()) { it.value() = next ; changed = true ; }
        }
        if (!changed) break ;
    }

    // The variables the other values and the templates can use
    variables = builtin ;
    for (auto it = resolved.constBegin() ; it != resolved.constEnd() ; ++it) variables[it.key()] = it.value() ;

    // Every value: the groups in the file, then its keys
    for (const QString &group : settings->childGroups()) {
        settings->beginGroup(group) ;
        for (const QString &key : settings->childKeys()) {
            QString raw_value = settings->value(key).toString() ;
            values[group + "/" + key] = expandPasses(raw_value, variables) ;
        }
        settings->endGroup() ;
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
    return values.value(selectedSection + "/" + key) ;
}

QString IniConfig::get(QString section, QString key)
{
    return values.value(section + "/" + key) ;
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

QString IniConfig::variable(QString name) const
{
    return variables.value(name) ;
}

QString IniConfig::expandVariables(QString text) const
{
    return expandPasses(text, variables) ;
}

// TODO: GetIcon
