#include "cremoteactorclient.h"

CRemoteActorClient::CRemoteActorClient(QString hostAddress, quint16 port, QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &CRemoteActorClient::connected);
    connect(socket, &QTcpSocket::disconnected, this, &CRemoteActorClient::disConnected);
    connect(socket, &QTcpSocket::readyRead, this, &CRemoteActorClient::readLine);

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
