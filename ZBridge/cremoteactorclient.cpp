#include "misc.h"
#include "cremoteactorclient.h"

CRemoteActorClient::CRemoteActorClient(QHostAddress hostAddress, quint16 port, QObject *parent) :
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
    while (socket->waitForReadyRead(10))
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
    ::message(QMessageBox::Information, tr("Client connected."));

    emit clientConnected();
}

void CRemoteActorClient::disConnected()
{
    ::message(QMessageBox::Information, tr("Client disconnected."));

    emit clientDisConnected();
}
