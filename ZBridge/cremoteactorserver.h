#ifndef CREMOTEACTORSERVER_H
#define CREMOTEACTORSERVER_H

#include <QtNetwork>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QString>
#include <QMessageBox>

#include "Defines.h"

class CRemoteActorFrontEnd : public QObject
{
    Q_OBJECT

public:
    explicit CRemoteActorFrontEnd(Seat seat, QString connectLine, QTcpSocket *socket);

signals:
    void receiveLine(QString line);
    void disConnectSeat(int seat);

public slots:
    void start();
    void sendLine(QString line);
    void stopFrontEnd();
    void disConnect();

private:
    void readLine();

    Seat seat;
    QTcpSocket *socket;
    QString connectLine;
};


class CRemoteActorServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CRemoteActorServer(int protocol, QHostAddress hostAddress, quint16 port, QObject *parent = 0);
    ~CRemoteActorServer();

    void incomingConnection(qintptr socketDescriptor);

    void stopAllClients();
    bool isConnected(Seat seat) { return remoteConnects[seat].isConnected; }
    CRemoteActorFrontEnd *getFrontend(Seat seat) { return remoteConnects[seat].frontEnd; }

signals:
    void stopFrontEnds();
    void clientDisconnected();

public slots:
    void disConnectSeat(int seat);

private:
    class CRemoteConnects
    {
    public:
        QString teamName;
        int protocol;
        QThread *thread;
        CRemoteActorFrontEnd *frontEnd;
        bool isConnected;
    } remoteConnects[4];

    int protocol;
};

#endif // CREMOTEACTORSERVER_H
