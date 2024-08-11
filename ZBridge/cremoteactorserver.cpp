/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CRemoteActorServer.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Remote actor server.

  Platforms: Qt.

*/

/**
 * \file
 * Remote actor server (definition).
 */

#include "cremoteactorserver.h"

/**
 * @brief The front end sets up signal/slot connections.
 * @param seat The seat for the front end actor.
 * @param connectLine The connect line received in the initial communication with the remote actor.
 * @param socket The socket to use.
 */
CRemoteActorFrontEnd::CRemoteActorFrontEnd(Seat seat, QString connectLine, QTcpSocket *socket)
{
    this->seat = seat;
    this->connectLine = connectLine;
    this->socket = socket;
    socket->setParent(this);

    connect(socket, &QTcpSocket::readyRead, this, &CRemoteActorFrontEnd::readLine);
    connect(socket, &QTcpSocket::disconnected, this, &CRemoteActorFrontEnd::disConnect);
}

CRemoteActorFrontEnd::~CRemoteActorFrontEnd()
{
    delete socket;
}

/**
 * @brief Starts the front end.
 *
 * emits a receiveLine signal with the connect line (to the table manager).
 */
void CRemoteActorFrontEnd::start()
{
    emit receiveLine(connectLine, false);
}

/**
 * @brief Reads a line from the remote actor and emits it to the table manager.
 */
void CRemoteActorFrontEnd::readLine()
{
    while (socket->canReadLine())
    {
        char lineData[500];
        socket->readLine(lineData, sizeof(lineData));
        QString line(lineData);
        emit receiveLine(line, true);
    }
}


/**
 * @brief Send a line to the remote actor.
 * @param line The line to send.
 */
void CRemoteActorFrontEnd::sendLine(QString line)
{
    socket->write(line.toLatin1());
}

/**
 * @brief Stop the front end.
 *
 * Disconnects from the remote actor, makes sure things are cleaned up and quits the front end thread.
 */
void CRemoteActorFrontEnd::stopFrontEnd()
{
    if (socket->state() == QTcpSocket::ConnectedState)
        socket->disconnectFromHost();
    deleteLater();
    thread()->quit();
}

/**
 * @brief Makes sure things are cleaned up and quits the front end thread.
 */
void CRemoteActorFrontEnd::disConnect()
{
    emit disConnectSeat(seat);
    deleteLater();
    thread()->quit();
}


/**
 * @brief Constructor for remote actor server.
 * @param protocol The protocol to use in the communication.
 * @param hostAddress The host address.
 * @param port The port.
 * @param parent The parent.
 *
 * Starts listening for a connect from a remote actor.
 */
CRemoteActorServer::CRemoteActorServer(Protocol protocol, QHostAddress hostAddress,
              quint16 port, QObject *parent) :
    QTcpServer(parent)
{
    this->protocol = protocol;

    for (int i = 0; i < 4; i++)
        remoteConnects[i].isConnected = false;

    if (!listen(hostAddress, port))
        throw NetProtocolException(errorString().toStdString());
}

CRemoteActorServer::~CRemoteActorServer()
{
    close();

    emit stopFrontEnds();
}

/**
 * @brief A remote actor is connecting.
 *
 * After connecting the remote actor server expects to receive the first message from the remote
 * actor client. It is a connect message. This message is checked and if it is found ok then a
 * remote actor front end thread is started to take care of further communication.
 */
void CRemoteActorServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket();

    socket->setSocketDescriptor(socketDescriptor);

    //Check for reading (connect message).
    if (!socket->waitForReadyRead(10000))
    {
        emit connectWarning(tr("Timeout on client connection."));
        delete socket;
        return;
    }

    //Check size of connect message.
    char buf[200];
    int length = socket->read(buf, 200);
    if (length > 175)
    {
        emit connectWarning(tr("Connecting message from client is too long."));
        delete socket;
        return;
    }
    buf[length] = 0;
    QString connectLine(buf);

    //Determine the protocol.
    int assumedProtocol = 0;
    int i = connectLine.size() - 1;
    while ((i >= 0) && !connectLine[i].isDigit()) i--;
    int j = i;
    while ((j >= 0) && connectLine[j].isDigit()) j--;
    i++; j++;

    if (i != j)
        assumedProtocol = connectLine.mid(j, i - j).toInt();

    //Check protocol and syntax of connect line.
    if ((assumedProtocol != protocol) ||
            !connectLine.contains("Connecting", Qt::CaseInsensitive) ||
            !connectLine.contains("as", Qt::CaseInsensitive) ||
            !connectLine.contains("using protocol version", Qt::CaseInsensitive) ||
            (!connectLine.contains("West", Qt::CaseInsensitive) &&
             !connectLine.contains("North", Qt::CaseInsensitive) &&
             !connectLine.contains("East", Qt::CaseInsensitive) &&
             !connectLine.contains("South", Qt::CaseInsensitive)) ||
            (connectLine.count(QChar('"')) != 2))
    {
        emit connectWarning(tr("Syntax error on connecting message from client."));
        delete socket;
        return;
    }

    Seat seat = connectLine.contains("West", Qt::CaseInsensitive) ? (WEST_SEAT) :
                connectLine.contains("North", Qt::CaseInsensitive) ? (NORTH_SEAT) :
                connectLine.contains("East", Qt::CaseInsensitive) ? (EAST_SEAT) :
                                                            (SOUTH_SEAT);

    QString teamName = connectLine.section('"', 1, 1);

    //Check that the seat is not already connected.
    if (remoteConnects[seat].isConnected)
    {
        emit connectWarning(tr("Client tries to connect as already connected hand."));
        delete socket;
        return;
    }

    //Everything is ok. Now start a front end thread.
    QThread *thread = new QThread();

    CRemoteActorFrontEnd *frontEnd = new CRemoteActorFrontEnd(seat, connectLine, socket);

    frontEnd->moveToThread(thread);

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(this, &CRemoteActorServer::stopFrontEnds, frontEnd, &CRemoteActorFrontEnd::stopFrontEnd);
    connect(frontEnd, &CRemoteActorFrontEnd::disConnectSeat, this, &CRemoteActorServer::disConnectSeat);

    remoteConnects[seat].teamName = teamName;
    remoteConnects[seat].protocol = protocol;
    remoteConnects[seat].thread = thread;
    remoteConnects[seat].frontEnd = frontEnd;
    remoteConnects[seat].isConnected = true;

    emit connectInfo(connectLine);

    thread->start();
}

/**
 * @brief Stop all remote actor clients.
 */
void CRemoteActorServer::stopAllClients()
{
    for (int i = 0; i < 4; i++)
        remoteConnects[i].isConnected = false;

    emit stopFrontEnds();
}

/**
 * @brief Remote actor has disconnected.
 * @param seat The remote actors seat.
 */
void CRemoteActorServer::disConnectSeat(Seat seat)
{
    remoteConnects[seat].isConnected = false;

    emit connectError(QCoreApplication::translate("defines", SEAT_NAMES[seat]) + tr(" has disconnected."));
    emit clientDisconnected();
}
