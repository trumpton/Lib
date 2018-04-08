#include "network.h"

#include <QVariant>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QString>
#include <QList>

int networkHttp(NetworkAction action, QString url, QString header, QString data, QString& result)
{

    QNetworkAccessManager manager ;
    QNetworkReply *reply = NULL ;
    QNetworkRequest request(url) ;
    QEventLoop eventLoop ;

    result.clear() ;

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    QByteArray submitdata = data.toStdString().c_str() ;

    QList<QString> headers = header.split("\n") ;

    for (int i=0; i<headers.size(); i++) {
        QList<QString> bits = headers.at(i).split(":") ;
        if (bits.size()==2) {
            QByteArray h, d ;
            h.append(bits.at(0).trimmed().toLatin1()) ;
            d.append(bits.at(1).trimmed().toLatin1()) ;
            request.setRawHeader(h, d) ;
        }
    }

    request.setHeader(QNetworkRequest::ContentLengthHeader, submitdata.length());

    switch (action) {
    case GET:
        reply = manager.get(request) ;
        break ;
     case POST:
         reply = manager.post(request, submitdata) ;
         break ;
     case PUT:
         reply = manager.put(request, submitdata) ;
         break ;
     case DELETE:
         reply = manager.deleteResource(request) ;
         break ;
     }

     eventLoop.exec() ;
     QVariant replycode=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) ;
     int errorcode=replycode.toInt() ;
     result = reply->readAll() ;

     return errorcode ;
}
