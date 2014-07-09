/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTblMngr.h
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Generic strategy table manager.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the generic strategy for table manager
 * strategy classes.
 */

#ifndef CTBLMNGR_H
#define CTBLMNGR_H

#include <QObject>

#include "Defines.h"

class CPlayView;

/**
 * \brief This abstract class implements the declaration of the generic strategy
 * for table manager classes (server and clients).
 *
 * Table manager strategy classes have a common method interface. The interface is
 * defined by means of the virtual methods in this class. And by means of the
 * methods, which are common for all table managers (Server and Client). These
 * methods are implemented in this class.
 */
class CTblMngr : public QObject
{
    Q_OBJECT

public:
    CTblMngr(CPlayView *playView, QObject *parent);
    virtual ~CTblMngr();

    /** @name Main menu activated methods.
     * These methods are activated via the applications main menu.
     */
    /*@{*/
    virtual void newSession() = 0;
    virtual void newDeal();
    virtual void showAllCards() = 0;
    virtual void reBid();
    virtual void rePlay();
    virtual void undo();
    /*@}*/

signals:

public slots:
    /** @name Play view slots.
     * These slot methods are signalled from the play view.
     */
    /*@{*/
    virtual void buttonClicked(int button) = 0;
    virtual void bidValue(Bids bid) = 0;
    virtual void playValue(int card) = 0;
    virtual void bidClose() = 0;
    virtual void handClicked(Seat seat) = 0;
    /*@}*/

    /** @name Actor slots.
     * These slot methods are signalled from the actors.
     */
    /*@{*/
    virtual void sConnect(QString name, Seat seat, int protocol) = 0;
    virtual void sRTNames(Seat seat) = 0;
    virtual void sRSBoard(Seat seat) = 0;
    virtual void sRDealInfo(Seat seat) = 0;
    virtual void sRCards(Seat seat) = 0;
    virtual void sRBid(Seat player, Seat bidder) = 0;
    virtual void sBid(Seat player, Bids bid) = 0;
    virtual void sPlayerPlays(Seat player, int card) = 0;
    virtual void sReadyForPlayer(Seat seat, Seat player, int trick) = 0;
    virtual void sReadyForDummy(Seat seat, int trick) = 0;
    virtual void sReadyForDummyCards(Seat seat) = 0;
    virtual void sAttemptSyncFromClientToServer(Seat syncher) = 0;
    virtual void sConfirmSyncFromClientToServer(Seat syncher) = 0;

    virtual void sShowAuction() = 0;
    virtual void sShowPlay() = 0;
    void sShowBidDialog(bool show);
    void sShowBid(Seat seat, Bids bid);
    void sShowCenter(Team vulnerable);
    void sShowDummyCards(Seat seat, int cards[13]);
    void sShowDummyOnTable(Seat seat);
    void sShowYourTurnOnTable(Seat seat);
    void sClearYourTurnOnTable();
    void sShowPlayerPlays(Seat player, int card);
    void sClearCardsOnTable();
    void sShowTricks(int ewTricks, int nsTricks);
    void sUndoBid(int noBid);
    void sUndoTrick(int noTrick, int nsTricks, int ewTricks);

    void sEnableBidder(Seat bidder, Bids lastBid, Bids doubleBid);
    void sDisableBidder(Seat bidder);
    void sEnablePlayer(Seat player);
    void sDisablePlayer(Seat player);
    virtual void sEnableContinueLeader() = 0;
    virtual void sDisableContinueLeader() = 0;
    virtual void sEnableContinueSync(int syncState) = 0;
    virtual void sDisableContinueSync(int syncState) = 0;

    virtual void sContinueLeader() = 0;
    virtual void sContinueSync() = 0;
    /*@}*/

protected:
    bool showAll;
    bool showDummy;
    Seat dummy;

private:
    void createConnections();

    CPlayView *playView;
};

#endif // CTBLMNGR_H
