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
    explicit CRemoteActorClient(QHostAddress hostAddress, quint16 port, QObject *parent = 0);
    
signals:
    void clientConnected();
    void clientDisConnected();
    void receiveLine(QString line);

public slots:
    void sendLine(QString line);
    void connected();
    void disConnected();

private:
    void readLine();

    QTcpSocket *socket;
};

#endif // CREMOTEACTORCLIENT_H
