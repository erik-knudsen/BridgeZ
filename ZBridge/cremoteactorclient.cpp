#include "cremoteactorclient.h"

CRemoteActorClient::CRemoteActorClient(QString hostAddress, quint16 port, QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &CRemoteActorClient::connected);
    connect(socket, &QTcpSocket::disconnected, this, &CRemoteActorClient::disConnected);
    connect(socket, &QTcpSocket::readyRead, this, &CRemoteActorClient::readLine);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

    socket->connectToHost(hostAddress, port);
}

void CRemoteActorClient::readLine()
{
    QByteArray lineData;
    lineData += socket->readAll();
    while (socket->waitForReadyRead(100))
        lineData += socket->readAll();
    QString line(lineData);
    emit receiveLine(line);
}

void CRemoteActorClient::sendLine(QString line)
{
    socket->write(line.toLatin1());
}

void CRemoteActorClient::connected()
{
    emit clientConnected();
}

void CRemoteActorClient::disConnected()
{
    emit clientDisConnected();
}

void CRemoteActorClient::socketError(QAbstractSocket::SocketError error)
{
    QString err;
    QString str;
    switch (error)
    {
    case QAbstractSocket::SocketTimeoutError:
        err = tr("Server connection timed out. ") + str.setNum(error);
        break;

    default:
        err = tr("Server connection error. ") + str.setNum(error);
        break;
    }

    emit sSocketError(err);
}
