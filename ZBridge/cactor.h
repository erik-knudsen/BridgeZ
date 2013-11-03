#ifndef CACTOR_H
#define CACTOR_H

#include <QObject>

#include "Defines.h"

class CTblMngr;

class CActor : public QObject
{
    Q_OBJECT
public:
    CActor(CTblMngr *tableManager);
    virtual ~CActor();

    virtual void startNewSession() = 0;

    virtual void bidValue(Bids bid) {}
    virtual void playValue(int card) {}
    virtual void continuePlay() {}
    virtual void raiseContinue() {}

    virtual void seated(QString teamName) = 0;
    virtual void teamNames(QString nsTeamName, QString ewTeamName) = 0;
    virtual void startOfBoard() = 0;
    virtual void dealInfo(int boardNumber, Seat dealer, Team vulnerability) = 0;
    virtual void cards(Seat player, int cards[13]) = 0;
    virtual void bidDone(Seat bidder, Bids bid) = 0;
    virtual void playerToLead(Seat player) = 0;
    virtual void dummyToLead() = 0;
    virtual void playerPlays(Seat player, int card) = 0;
    virtual void dummyCards(int cards[13]) = 0;
    virtual void undoBid(bool reBid) = 0;
    virtual void undoTrick(bool rePlay) = 0;
    virtual void endOfSession() = 0;

    virtual Actor getActorType() = 0;
    virtual Seat getSeat() = 0;
    virtual bool isWaiting() = 0;
    virtual void setShowUser(bool showUser) {}

signals:
    void sConnect(QString name, Seat seat, int protocol);
    void sRTNames(Seat seat);
    void sRSBoard(Seat seat);
    void sRDealInfo(Seat seat);
    void sRCards(Seat seat);
    void sRBid(Seat player, Seat bidder);
    void sBid(Seat bidder, Bids bid);
    void sPlayerPlays(Seat player, int card);
    void sReadyForPlayer(Seat seat, Seat player, int trick);
    void sReadyForDummy(Seat seat, int trick);
    void sReadyForDummyCards(Seat seat);

private:
    CTblMngr *tableManager;
};

#endif // CACTOR_H
