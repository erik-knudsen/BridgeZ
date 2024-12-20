/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CActorLocal.h
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Actor local class.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the actor local class.
 */

#ifndef CACTORLOCAL_H
#define CACTORLOCAL_H

#include <QObject>
#include <QTextStream>

#include "../src-gen/ZBridgeClient.h"
#include "../src-gen/ZBridgeClientSync.h"

#include "Defines.h"
#include "cbiddb.h"
#include "cbiddesc.h"
#include "cbidandplay.h"
#include "cactor.h"

class CBidAndPlayEngines;
class CTblMngr;

/**
 * @brief This class implements the actor local class.
 *
 * The class implements a local actor (player). This is done by means of a statechart generated by
 * the Yakindu Statechart Tools. The statechart takes the actor through the states needed by the actor.
 * The statechart implements the @ref protocol. The protocol is used for both local and remote actors.
 * The protocol is best suited for automatic actors. I have added a few extensions to make it better
 * suited for manual actors. I have called this extended protocol the advanced protocol.
 */
class CActorLocal : public CActor
{
    Q_OBJECT
public:
    CActorLocal(Protocol protocol, int bidDelay, int playDelay, bool manual, QString teamName,
            Seat seat, CBidAndPlayEngines *bidAndPlayEngines, CTblMngr *tableManager);

    void startNewSession();
    void clientRunCycle();
    void clientSyncRunCycle();

    void bidValue(Bids bid);
    void playValue(int card);

    void setShowUser(bool showUser) { this->showUser = showUser; }
    void setUpdateGameInfo(bool updateGameInfo) { this->updateGameInfo = updateGameInfo; }

    CBidHistory &getBidHistory() { return bidAndPlay.getBidHistory(); }
    CPlayHistory &getPlayHistory() { return bidAndPlay.getPlayHistory(); }

    void seated(QString teamName);
    void teamNames(QString nsTeamName, QString ewTeamName);
    void startOfBoard();
    void dealInfo(int boardNumber, Seat dealer, Team vulnerability);
    void cards(int cards[4][13]);
    void bidDone(Seat bidder, Bids bid);
    void playerToLead(Seat player);
    void dummyToLead();
    void playerPlays(Seat player, int card);
    void getHint();
    void dummyCards(int cards[13]);
    void undoBid(bool reBid);
    void undoTrick(bool rePlay);
    void endOfSession();
    void reStart();

    void attemptSyncFromServerToClient();
    void confirmSyncFromServerToClient();
    void allSyncFromServerToClient();

    Actor getActorType() { return (manual ? MANUAL_ACTOR : AUTO_ACTOR); }
    Seat getSeat() {return (Seat)zBridgeClientIface_get_client(&handle); }
    void setManual(bool manual) { this->manual = (manual) ? manual : defManual; }

    ZBridgeClient *getHandle() { return &handle; }

public slots:
    void bidValue();
    void playValue();
    void continueSync();
signals:
    void sShowAuction(bool afterReplay, Seat dummy);
    void sShowPlay(Suit trump);
    void sShowBidDialog(bool show);
    void sShowBid(Seat seat, Bids bid, QString features = "", QString alert = "");
    void sBidHint(Bids bid);
    void sShowCenter(Team vulnerable);
    void sShowDummyOnTable(Seat seat);
    void sShowDummyCards(Seat seat, int cards[13]);
    void sShowYourTurnOnTable(Seat seat);
    void sClearYourTurnOnTable();
    void sShowPlayerPlays(Seat player, int card);
    void sPlayHint(Seat player, int card);
    void sClearCardsOnTable();
    void sShowTricks(int ewTricks, int nsTricks);
    void sUndoBid(int noBid);
    void sUndoTrick(int noTrick, int nsTricks, int ewTricks);

    void sEnableBidder(Seat bidder, Bids lastBid, Bids doubleBid);
    void sDisableBidder(Seat bidder);
    void sEnablePlayer(Seat player);
    void sDisablePlayer(Seat Player);
    void sEnableContinueSync(int syncState);
    void sDisableContinueSync(int syncState);
private:
    void clientActions();
    void clientSyncActions();

    bool manual;        /**< Automatic or Manual actor ? */
    bool defManual;
    bool showUser;      /**< Control what is shown to the user? */
    bool updateGameInfo;/**< Control who updates game info? */
    bool updateCurrentGameInfo;

    Protocol protocol;  /**< Protocol. */

    QString teamName;   /**< Team name. */

    CBidAndPlay bidAndPlay; /**< Bid and play handling. */

    CTblMngr *tableManager; /**< Controlling table manager (Server or Client). */

    ZBridgeClient handle;   /**< Handle to client Yakindu state chart. */
    ZBridgeClientSync syncHandle; /**< Handle to client Yakindu state chart synchronization. */

    bool synchronizing;
    int bidDelay;
    int playDelay;
};

#endif // CACTORLOCAL_H
