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
#include "ctblmngrbase.h"

class CPlayView;

/**
 * \brief This abstract class implements the declaration of the generic strategy
 * for table manager classes (server and clients - not auto).
 *
 * Table manager strategy classes have a common method interface. The interface is
 * defined partly by means of virtual methods in this class. And partly by means of
 * methods, which are common for all table managers (Server and Client). These
 * methods are implemented in this class.
 */
class CTblMngr : public CTblMngrBase
{
    Q_OBJECT

public:
    CTblMngr(CPlayView *playView, QObject *parent);
    virtual ~CTblMngr();

    /** @name Main menu activated methods.
     * These methods are activated via the applications main menu.
     */
    /*@{*/
    virtual void newDeal();
    virtual void showAllCards() = 0;
    virtual void showDoubleDummyResults() = 0;
    virtual void reBid();
    virtual void rePlay();
    virtual void undo();
    virtual void hint() = 0;
    /*@}*/

signals:
    void sShowScore();

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
    virtual void sShowAuction() = 0;
    virtual void sShowPlay() = 0;
    void sShowBidDialog(bool show);
    void sShowBid(Seat seat, Bids bid, QString features = "", QString alert = "");
    void sBidHint(Bids bid);
    void sShowCenter(Team vulnerable);
    void sShowDummyCards(Seat seat, int cards[13]);
    void sShowDummyOnTable(Seat seat);
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
    void sDisablePlayer(Seat player);
    virtual void sEnableContinueSync(int syncState) = 0;
    virtual void sDisableContinueSync(int syncState) = 0;

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
