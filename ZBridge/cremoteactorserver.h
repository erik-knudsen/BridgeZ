#ifndef CREMOTEACTORSERVER_H
#define CREMOTEACTORSERVER_H

#include <QtNetwork>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QString>

#include "Defines.h"

class CRemoteActorFrontEnd : public QObject
{
    Q_OBJECT

public:
    explicit CRemoteActorFrontEnd(QString connectLine, QTcpSocket *socket);

signals:
    void receiveLine(QString line);
    void newSession();

public slots:
    void start();
    void sendLine(QString line);
    void stopFrontEnd();
    void disConnect();

private:
    void readLine();

    QTcpSocket *socket;
    QString connectLine;
};


class CRemoteActorServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CRemoteActorServer(QHostAddress hostAddress, quint16 port, QObject *parent = 0);
    ~CRemoteActorServer();

    void incomingConnection(qintptr socketDescriptor);

    void stopAllClients();
    bool isConnected(Seat seat) { return remoteConnects[seat].isConnected; }
    CRemoteActorFrontEnd *getFrontend(Seat seat) { return remoteConnects[seat].frontEnd; }

signals:
    void stopFrontEnds();

private:
    void errMsg(QString msg);

    class CRemoteConnects
    {
    public:
        QString teamName;
        int protocol;
        QThread *thread;
        CRemoteActorFrontEnd *frontEnd;
        bool isConnected;
    } remoteConnects[4];
};

#endif // CREMOTEACTORSERVER_H
