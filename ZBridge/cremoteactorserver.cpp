#include "misc.h"
#include "cremoteactorserver.h"

CRemoteActorFrontEnd::CRemoteActorFrontEnd(Seat seat, QString connectLine, QTcpSocket *socket)
{
    this->seat = seat;
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
    lineData += socket->readAll();
    while (socket->waitForReadyRead(100))
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
    emit disConnectSeat(seat);
    deleteLater();
    thread()->quit();
}


CRemoteActorServer::CRemoteActorServer(int protocol, QHostAddress hostAddress, quint16 port, QObject *parent) :
    QTcpServer(parent)
{
    this->protocol = protocol;

    for (int i = 0; i < 4; i++)
        remoteConnects[i].isConnected = false;

    if (!listen(hostAddress, port))
        ::message(QMessageBox::Warning, errorString());
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
    if (!socket->waitForReadyRead(10000))
    {
        ::message(QMessageBox::Warning, tr("Timeout on client connection."));
        delete socket;
        return;
    }

    char buf[200];
    int length = socket->read(buf, 200);
    if (length > 175)
    {
        ::message(QMessageBox::Warning, tr("Connecting message from client is too long."));
        delete socket;
        return;
    }
    buf[length] = 0;
    QString connectLine(buf);

    int assumedProtocol = 0;
    int i = connectLine.size() - 1;
    while ((i >= 0) && !connectLine[i].isDigit()) i--;
    int j = i;
    while ((j >= 0) && connectLine[j].isDigit()) j--;
    i++; j++;

    if (i != j)
        assumedProtocol = connectLine.mid(j, i - j).toInt();

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
        ::message(QMessageBox::Warning, tr("Syntax error on connecting message from client."));
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
        ::message(QMessageBox::Warning, tr("Client tries to connect as already connected hand."));
        delete socket;
        return;
    }
    if ((remoteConnects[(seat + 2) & 3].isConnected) &&
            (remoteConnects[(seat +2) & 3].teamName.compare(teamName, Qt::CaseInsensitive)) != 0)
    {
        ::message(QMessageBox::Warning, tr("Team clients do not agree on team name."));
        delete socket;
        return;
    }

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

    ::message(QMessageBox::Information, connectLine);

    thread->start();
}

void CRemoteActorServer::stopAllClients()
{
    for (int i = 0; i < 4; i++)
        remoteConnects[i].isConnected = false;

    emit stopFrontEnds();
}


void CRemoteActorServer::disConnectSeat(int seat)
{
    int i;

    remoteConnects[seat].isConnected = false;

    for (i = 0; i < 4; i++)
        if (SEATS[i] == seat)
            break;

    ::message(QMessageBox::Critical, SEAT_NAMES[i] + tr(" has disconnected."));

    emit clientDisconnected();
}
