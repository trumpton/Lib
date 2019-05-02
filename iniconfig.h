#ifndef INICONFIG_H
#define INICONFIG_H

#include <QString>
#include <QSettings>

#define DISABLED QString("DISABLED")

class IniConfig
{
private:
    QSettings *settings ;
    QString selectedSection ;
    QString path ;
    QString ExpandVars(QString src) ;

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

signals:

public slots:
};

#endif // INICONFIG_H
