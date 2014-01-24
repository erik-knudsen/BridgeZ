/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CRemoteActorClient.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Remote actor client.

  Platforms: Qt.

*/

/**
 * \file
 * Remote actor client (declaration).
 */

#ifndef CREMOTEACTORCLIENT_H
#define CREMOTEACTORCLIENT_H

#include <QtNetwork>
#include <QObject>
#include <QTcpSocket>

#include "Defines.h"

/**
 * @brief Remote actor client class.
 *
 * This class takes care of tcp/ip communication on the remote actor client side
 */
class CRemoteActorClient : public QObject
{
    Q_OBJECT
public:
    explicit CRemoteActorClient(QString hostAddress, quint16 port, QObject *parent = 0);
    ~CRemoteActorClient();

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
