#ifndef INICONFIG_H
#define INICONFIG_H

#include <QString>
#include <QSettings>
#include <QMap>
#include <QStringList>

#define DISABLED QString("DISABLED")

// Reads an ini file.  Every value is expanded once, when the file is loaded.
//
// Variables are written $(NAME).  The built-in names HOME, INI and APPDIR are parsed first, and cannot be overridden in
// the [var] section.  LIBREOFFICE is also set automatically (the LibreOffice install folder, found on Windows from the
// registry), but a [var] entry called LIBREOFFICE overrides it.  The [var] entries are then expanded, and every other
// value is expanded.  Each expansion goes through
// at most four passes (IniConfig::PASSES), or until nothing changes.  Anything still unresolved after the fourth pass is
// kept as it was at that point.
class IniConfig
{
public:
    static const int PASSES = 4 ;

private:
    QSettings *settings ;
    QString selectedSection ;
    QString path ;
    QMap<QString,QString> values ;      // every ini value, expanded, keyed "section/key"
    QMap<QString,QString> variables ;   // the built-in names and the [var] entries, expanded
    QStringList problems ;              // [var] entries that were ignored, for the log

    void parseOnLoad() ;

public:
    IniConfig() ;
    ~IniConfig() ;

private:
    // Unused copy construct
    IniConfig(const IniConfig &other) ;

public:
    bool init(QString inifile);
    QString& canonicalPath() ;
    QString get(QString key) ;
    bool isset(QString key) ;
    QString get(QString section, QString key) ;
    bool isset(QString section, QString key) ;

    bool selectSection(QString section) ;
    int numSections() ;
    QString getSection(int num) ;

    // The expanded value of a variable (a built-in name or a [var] entry), or "" if there is none
    QString variable(QString name) const ;

    // The text with its variables replaced: the built-in names and the [var] entries, up to PASSES passes
    QString expandVariables(QString text) const ;

    // [var] entries that were not used (for example, a built-in name)
    QStringList ignoredVariables() const { return problems ; }

signals:

public slots:
};

#endif // INICONFIG_H
