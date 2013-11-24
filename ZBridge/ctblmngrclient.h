#ifndef CTBLMNGRCLIENT_H
#define CTBLMNGRCLIENT_H

#include <QTimer>

#include "CTblMngr.h"
#include "../src-gen/ZBridgeClient.h"

#include "Defines.h"

class CZBridgeDoc;
class CActorLocal;
class CBidEngine;
class CPlayEngine;
class CPlayView;
class CRemoteActorClient;

class CTblMngrClient : public CTblMngr
{
    Q_OBJECT

public:
    CTblMngrClient(CZBridgeDoc *doc, CPlayView *playView, QObject *parent = 0);
    ~CTblMngrClient();

    void newSession();

public slots:
    void clientConnected();
    void clientDisConnected();
    void receiveLine(QString line);

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

private:
    void cleanTableManager();

    CActorLocal *actor;
    CRemoteActorClient *remoteActorClient;

    int protocol;

    int noHands;
    int currentCards[4][13];

    QTimer *continueButton;

    CZBridgeDoc *doc;
    CPlayView *playView;

    ZBridgeClient *handle;
};

#endif // CTBLMNGRCLIENT_H
