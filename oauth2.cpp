#include "oauth2.h"
#include "ui_oauth2.h"
#include "../Lib/network.h"

#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonObject>
#include <QJsonParseError>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>

OAuth2::OAuth2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OAuth2)
{
   ui->setupUi(this);
   connect(ui->web, SIGNAL(urlChanged(const QUrl)), this, SLOT(webPageChanged(const QUrl))) ;
}

OAuth2::~OAuth2()
{
    delete ui;
}

//
// Open a web browser, and reurn the authcode, which is extracted
// from the resulting url.

QString OAuth2::getAuthCode(QString url, QString completedurl)
{
    accesstoken.clear() ;
    authcode.clear() ;
    ui->web->setUrl(QUrl(url)) ;
    ui->web->page()->profile()->cookieStore()->deleteAllCookies() ;
    testurl = completedurl ;
    this->exec() ;
    return authcode ;
}

void OAuth2::webPageChanged(const QUrl url)
{
    QString urltxt = url.toString() ;

    int tlen = testurl.length() ;
    if (urltxt.left(tlen).compare(testurl)==0) {
        QStringList dat = urltxt.mid(tlen).split("&") ;
        authcode = dat.at(0) ;
        this->done(0) ;
    }
}

//
// Request the access token, given the authcode
//

QString OAuth2::getAccessToken(QString requesturl, QString clientid, QString clientsecret, QString authcode)
{

    QJsonObject json ;
    json.insert("client_id",clientid) ;
    json.insert("code", authcode) ;
    json.insert("client_secret", clientsecret) ;
    json.insert("grant_type", "authorization_code") ;
    QJsonDocument doc ;
    doc.setObject(json) ;
    QString jsontext = doc.toJson() ;

    QString header = "Content-type: application/json\n" ;
    QString result ;

    int status = networkHttp(POST, requesturl, header, jsontext, result) ;

    if (status<200 || status>299) {
        accesstoken.clear() ;
    } else {
        QJsonParseError err ;
        QJsonDocument resultdoc = QJsonDocument::fromJson(result.toUtf8(), &err) ;
        if (err.error != QJsonParseError::NoError) return "Error" ;
        if (resultdoc.isNull()) return "Error" ;
        if (!resultdoc.isObject()) return "Error" ;
        accesstoken = resultdoc.object()["access_token"].toString() ;
    }

    return accesstoken ;
}

