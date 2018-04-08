#ifndef OAUTH2_H
#define OAUTH2_H

//
// OAuth2 Access Example
//
//  authcode = getAuthCode("https://server/oauth2grant?client=1234567890", "https://redirectserver/completed.html?code=") ;
//  accesstk = getAccessToken("https://server/accesstoken", "1234567890", "secretsecret", authcode) ;
//

#include <QUrl>
#include <QDialog>

namespace Ui {
class OAuth2;
}

class OAuth2 : public QDialog
{
    Q_OBJECT

public slots:
    void webPageChanged(const QUrl url) ;

public:
    explicit OAuth2(QWidget *parent = 0);
    ~OAuth2();

    QString getAuthCode(QString requesturl, QString completedurl) ;
    QString getAccessToken(QString requesturl, QString clientid, QString clientsecret, QString authcode) ;

private:
    Ui::OAuth2 *ui;
    QString testurl ;
    QString authcode ;
    QString refreshtoken ;
    QString accesstoken ;

};

#endif // OAUTH2_H
