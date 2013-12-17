#ifndef CREMOTEACTORCLIENT_H
#define CREMOTEACTORCLIENT_H

#include <QtNetwork>
#include <QObject>
#include <QTcpSocket>

#include "Defines.h"

class CRemoteActorClient : public QObject
{
    Q_OBJECT
public:
    explicit CRemoteActorClient(QString hostAddress, quint16 port, QObject *parent = 0);
    
signals:
    void clientConnected();
    void clientDisConnected();
    void receiveLine(QString line);
    void sSocketError(QString err);

public slots:
    void sendLine(QString line);
    void connected();
    void disConnected();
    void socketError(QAbstractSocket::SocketError error);

private:
    void readLine();

    QTcpSocket *socket;
};

#endif // CREMOTEACTORCLIENT_H
