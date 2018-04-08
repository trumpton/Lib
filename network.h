#ifndef NETWORK_H
#define NETWORK_H

#include <QString>

typedef enum {
        PUT = 0,
        POST = 1,
        GET = 2,
        DELETE = 3
} NetworkAction ;

int networkHttp(NetworkAction action, QString url, QString header, QString data, QString& result) ;


#endif // NETWORK_H
