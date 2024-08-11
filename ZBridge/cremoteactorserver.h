/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CRemoteActorServer.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Remote actor server.

  Platforms: Qt.

*/

/**
 * \file
 * Remote actor server (declaration).
 */

#ifndef CREMOTEACTORSERVER_H
#define CREMOTEACTORSERVER_H

#include <QtNetwork>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QString>
#include <QMessageBox>

#include "ZBridgeException.h"
#include "Defines.h"

/**
 * @brief The remote actor front end.
 *
 * The front end runs in a thread of its own. It takes care of most of the tcp/ip communication from
 * a remote actor client.
 */
class CRemoteActorFrontEnd : public QObject
{
    Q_OBJECT

public:
    explicit CRemoteActorFrontEnd(Seat seat, QString connectLine, QTcpSocket *socket);
    ~CRemoteActorFrontEnd();

signals:
    void receiveLine(QString line, bool connected);
    void disConnectSeat(Seat seat);

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


/**
 * @brief The remote actor server.
 *
 * The remote actor server listens for remote actor clients and when one connects it takes care of
 * the initial communication and then starts a front end thread for further communication with the client.
 * The remote actor server can handle up to 4 remote actors.
 */
class CRemoteActorServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CRemoteActorServer(Protocol protocol, QHostAddress hostAddress, quint16 port,
                                QObject *parent = 0);
    ~CRemoteActorServer();

    void incomingConnection(qintptr socketDescriptor);

    void stopAllClients();
    bool isConnected(Seat seat) { return remoteConnects[seat].isConnected; }
    CRemoteActorFrontEnd *getFrontend(Seat seat) { return remoteConnects[seat].frontEnd; }

signals:
    void stopFrontEnds();
    void clientDisconnected();
    void connectError(QString text);
    void connectWarning(QString text);
    void connectInfo(QString text);

public slots:
    void disConnectSeat(Seat seat);

private:
    class CRemoteConnects
    {
    public:
        QString teamName;
        Protocol protocol;
        QThread *thread;
        CRemoteActorFrontEnd *frontEnd;
        bool isConnected;
    } remoteConnects[4];

    Protocol protocol;
};

#endif // CREMOTEACTORSERVER_H
