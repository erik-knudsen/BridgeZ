#ifndef CACTORAUTO_H
#define CACTORAUTO_H

#include <QObject>

#include "../src-gen/ZBridgeClient.h"

#include "Defines.h"
#include "cbidandplay.h"
#include "cactor.h"

class CBidOptionDoc;

class CActorLocal : public CActor
{
    Q_OBJECT
public:
    CActorLocal(bool manual, QString teamName, Seat seat, int protocol, CBidOptionDoc &bidOptionDocOwn,
               CBidOptionDoc &bidOptionDocOpp, CTblMngr *tableManager);

    void startNewSession();
    void clientRunCycle();

    void bidValue(Bids bid);
    void playValue(int card);
    void continuePlay();

    void setShowUser(bool showUser) { this->showUser = showUser; }

    void seated(QString teamName);
    void teamNames(QString nsTeamName, QString ewTeamName);
    void startOfBoard();
    void dealInfo(int boardNumber, Seat dealer, Team vulnerability);
    void cards(int cards[4][13]);
    void bidDone(Seat bidder, Bids bid);
    void playerToLead(Seat player);
    void dummyToLead();
    void playerPlays(Seat player, int card);
    void dummyCards(int cards[13]);
    void undoBid(bool reBid);
    void undoTrick(bool rePlay);
    void endOfSession();

    Actor getActorType() { return (manual ? MANUAL_ACTOR : AUTO_ACTOR); }
    Seat getSeat() {return (Seat)zBridgeClientIface_get_client(&handle); }
    bool isWaiting() { return waiting; }

    ZBridgeClient *getHandle() { return &handle; }

public slots:
    void bidValue();
    void playValue();

signals:
    void sShowAuction();
    void sShowPlay();
    void sShowBidDialog(bool show);
    void sShowBid(Seat seat, Bids bid);
    void sShowCenter(Team vulnerable);
    void sShowDummyOnTable(Seat seat);
    void sShowDummyCards(Seat seat, int cards[13]);
    void sShowYourTurnOnTable(Seat seat);
    void sClearYourTurnOnTable();
    void sShowPlayerPlays(Seat player, int card);
    void sClearCardsOnTable();
    void sShowTricks(int ewTricks, int nsTricks);

    void sEnableBidder(Seat bidder, Bids lastBid, Bids doubleBid);
    void sDisableBidder(Seat bidder);
    void sEnablePlayer(Seat player);
    void sDisablePlayer(Seat Player);
    void sEnableContinue();
    void sDisableContinue();


private:
    void clientActions();

    bool manual;
    bool showUser;
    bool waiting;

    QString teamName;
    int protocol;

    CBidAndPlay bidAndPlay;

    CTblMngr *tableManager;

    ZBridgeClient handle;
};

#endif // CACTORLOCAL_H
