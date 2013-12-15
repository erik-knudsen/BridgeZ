#ifndef CTBLMNGRSERVER_H
#define CTBLMNGRSERVER_H

#include <QTimer>

#include "CTblMngr.h"
#include "../src-gen/ZBridgeServer.h"

#include "Defines.h"
#include "cbidhistory.h"
#include "cplayhistory.h"

class CZBridgeDoc;
class CActor;
class CBidEngine;
class CPlayEngine;
class CPlayView;
class CRemoteActorServer;

class CTblMngrServer : public CTblMngr
{
    Q_OBJECT

public:
    CTblMngrServer(CZBridgeDoc *doc, CPlayView *playView, QObject *parent = 0);
    ~CTblMngrServer();

    void serverRunCycle();
    void newSession();
    void newDeal();
    void reBid();
    void rePlay();
    void undo();

public slots:
    //From actors.
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

    void sShowAuction();
    void sShowPlay();
    void sEnableContinue();
    void sDisableContinue();

    void sContinuePlay();

    //From play view.
    void buttonClicked(int button);
    void bidValue(Bids bid);
    void playValue(int card);
    void bidBackup();
    void bidHint();
    void bidRestart();
    void bidClose();
    void handClicked(Seat seat);

    //Delay slots.
    void startOfBoard();
    void playerToLead();
    void dummyToLead();

    //Reset table manager.
    void cleanTableManager();

private:
    void giveNewDeal();

    void serverActions();
    void setShowUser(bool showAll);

    ZBridgeServer handle;

    CActor *actors[4];
    CRemoteActorServer *remoteActorServer;
    CBidHistory bidHistory;
    CPlayHistory playHistory;

    int protocol;

    int currentBoardNo;
    Team currentVulnerable;

    int boardNo;
    Team vulnerable[4] = { NEITHER, NORTH_SOUTH, EAST_WEST, BOTH };
    Seat dealer[4] = { NORTH_SEAT, EAST_SEAT, SOUTH_SEAT, WEST_SEAT };

    int currentCards[4][13];
    bool showUser;
    bool waiting;

    QTimer *continueButton;

    CZBridgeDoc *doc;
    CPlayView *playView;
};

#endif // CTBLMNGRSERVER_H
