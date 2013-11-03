#include <QMessageBox>

#include "cremoteactorserver.h"

CRemoteActorFrontEnd::CRemoteActorFrontEnd(QString connectLine, QTcpSocket *socket)
{
    this->connectLine = connectLine;
    this->socket = socket;
    socket->setParent(this);

    connect(socket, &QTcpSocket::readyRead, this, &CRemoteActorFrontEnd::readLine);
    connect(socket, &QTcpSocket::disconnected, this, &CRemoteActorFrontEnd::disConnect);
}

void CRemoteActorFrontEnd::start()
{
    emit receiveLine(connectLine);
}

void CRemoteActorFrontEnd::readLine()
{
    QByteArray lineData;
    while (socket->waitForReadyRead(10))
        lineData += socket->readAll();
    QString line(lineData);
    emit receiveLine(line);
}


void CRemoteActorFrontEnd::sendLine(QString line)
{
    socket->write(line.toLatin1());
}

void CRemoteActorFrontEnd::stopFrontEnd()
{
    if (socket->state() == QTcpSocket::ConnectedState)
        socket->disconnectFromHost();
    deleteLater();
    thread()->quit();
}

void CRemoteActorFrontEnd::disConnect()
{
    emit newSession();
}


CRemoteActorServer::CRemoteActorServer(QHostAddress hostAddress, quint16 port, QObject *parent) :
    QTcpServer(parent)
{
    for (int i = 0; i < 4; i++)
        remoteConnects[i].isConnected = false;

    if (!listen(hostAddress, port))
        errMsg(errorString());
}

CRemoteActorServer::~CRemoteActorServer()
{
    close();

    emit stopFrontEnds();
}

void CRemoteActorServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket();

    socket->setSocketDescriptor(socketDescriptor);
    if (!socket->waitForReadyRead(1000))
    {
        errMsg("Timeout on client connection.");
        delete socket;
        return;
    }

    char buf[200];
    int length = socket->read(buf, 200);
    if (length > 175)
    {
        errMsg("Connecting message from client is too long.");
        delete socket;
        return;
    }
    buf[length] = 0;
    QString connectLine(buf);

    int protocol = 0;
    int i = connectLine.size() - 1;
    while (connectLine[i].isDigit()) i--;
    if (i != (connectLine.size() - 1))
        protocol = connectLine.right(connectLine.size() - 1 - i).toInt();

    if ((protocol != NET_PROTOCOL) ||
            !connectLine.contains("Connecting", Qt::CaseInsensitive) ||
            !connectLine.contains("as", Qt::CaseInsensitive) ||
            !connectLine.contains("using protocol version", Qt::CaseInsensitive) ||
            (!connectLine.contains("West", Qt::CaseInsensitive) &&
             !connectLine.contains("North", Qt::CaseInsensitive) &&
             !connectLine.contains("East", Qt::CaseInsensitive) &&
             !connectLine.contains("South", Qt::CaseInsensitive)) ||
            (connectLine.count(QChar('"')) != 2))
    {
        errMsg("Syntax error on connecting message from client.");
        delete socket;
        return;
    }

    Seat seat = connectLine.contains("West", Qt::CaseInsensitive) ? (WEST_SEAT) :
                connectLine.contains("North", Qt::CaseInsensitive) ? (NORTH_SEAT) :
                connectLine.contains("East", Qt::CaseInsensitive) ? (EAST_SEAT) :
                                                            (SOUTH_SEAT);

    QString teamName = connectLine.section('"', 1, 1);

    if (remoteConnects[seat].isConnected)
    {
        errMsg("Client tries to connect as already connected hand.");
        delete socket;
        return;
    }
    if ((remoteConnects[(seat + 2) & 3].isConnected) &&
            (remoteConnects[(seat +2) & 3].teamName.compare(teamName, Qt::CaseInsensitive)) != 0)
    {
        errMsg("Team clients do not agree on team name.");
        delete socket;
        return;
    }

    QThread *thread = new QThread();
    socket->moveToThread(thread);

    CRemoteActorFrontEnd *frontEnd = new CRemoteActorFrontEnd(connectLine, socket);

    frontEnd->moveToThread(thread);

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(this, &CRemoteActorServer::stopFrontEnds, frontEnd, &CRemoteActorFrontEnd::stopFrontEnd);

    remoteConnects[seat].teamName = teamName;
    remoteConnects[seat].protocol = protocol;
    remoteConnects[seat].thread = thread;
    remoteConnects[seat].frontEnd = frontEnd;
    remoteConnects[seat].isConnected = true;

    thread->start();
}

void CRemoteActorServer::stopAllClients()
{
    for (int i = 0; i < 4; i++)
        remoteConnects[i].isConnected = false;

    emit stopFrontEnds();
}

void CRemoteActorServer::errMsg(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}
