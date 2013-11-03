#ifndef CACTORREMOTE_H
#define CACTORREMOTE_H

#include <QObject>

#include "Defines.h"
#include "cactor.h"

class CRemoteActorFrontEnd;

class CActorRemote : public CActor
{
    Q_OBJECT
public:
    CActorRemote(Seat seat, CRemoteActorFrontEnd *frontEnd, CTblMngr *tableManager);

    void startNewSession();

    void seated(QString teamName);
    void teamNames(QString nsTeamName, QString ewTeamName);
    void startOfBoard();
    void dealInfo(int boardNumber, Seat dealer, Team vulnerability);
    void cards(Seat player, int cards[13]);
    void bidDone(Seat bidder, Bids bid);
    void playerToLead(Seat player);
    void dummyToLead();
    void playerPlays(Seat player, int card);
    void dummyCards(int cards[13]);
    void undoBid(bool reBid);
    void undoTrick(bool rePlay);
    void endOfSession();

    Actor getActorType() { return REMOTE_ACTOR; }
    Seat getSeat() { return seat; }
    bool isWaiting() { return false; }

public slots:
    void receiveLine(QString line);

signals:
    void start();
    void sendLine(QString line);

private:
    Seat seat;
    CRemoteActorFrontEnd *frontEnd;
    CTblMngr * tableManager;
};

#endif // CACTORREMOTE_H
