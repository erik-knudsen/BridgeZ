/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CRemoteActorClient.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Remote actor client.

  Platforms: Qt.

*/

/**
 * \file
 * Remote actor client (definition).
 */

#include "cremoteactorclient.h"

/**
 * @brief Constructor for remote actor client.
 * @param hostAddress Address of host to connect to.
 * @param port Port to use when connecting.
 * @param parent The parent.
 *
 * The constructor:
 *   - Allocates a socket.
 *   - Sets up signal/slot connections for the communication.
 *   - Connects to the host.
 */
CRemoteActorClient::CRemoteActorClient(QString hostAddress, quint16 port, QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &CRemoteActorClient::connected);
    connect(socket, &QTcpSocket::disconnected, this, &CRemoteActorClient::disConnected);
    connect(socket, &QTcpSocket::readyRead, this, &CRemoteActorClient::readLine);
    connect(socket, &QTcpSocket::errorOccurred, this, &CRemoteActorClient::socketError);
//    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

    socket->connectToHost(hostAddress, port);
}

CRemoteActorClient::~CRemoteActorClient()
{
    delete socket;
}

/**
 * @brief Read a line from the host.
 */
void CRemoteActorClient::readLine()
{
    while (socket->canReadLine())
    {
        char lineData[500];
        socket->readLine(lineData, sizeof(lineData));
        QString line(lineData);
        emit receiveLine(line);
    }
}

/**
 * @brief Send a line to the host.
 * @param line The line to send.
 */
void CRemoteActorClient::sendLine(QString line)
{
    socket->write(line.toLatin1());
}

/**
 * @brief emit clientConnected signal.
 */
void CRemoteActorClient::connected()
{
    emit clientConnected();
}

/**
 * @brief emit clientDisConnected signal.
 */
void CRemoteActorClient::disConnected()
{
    emit clientDisConnected();
}

/**
 * @brief There was a socket error.
 * @param error The error.
 *
 * Report the error and emit sSocketError signal.
 */
void CRemoteActorClient::socketError(QAbstractSocket::SocketError error)
{
    switch (error)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;

    default:
        emit sSocketError(tr("Server connection error."));
        break;
    }
}
