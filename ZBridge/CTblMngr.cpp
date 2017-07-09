/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTblMngr.cpp
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Generic strategy table manager.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the generic strategy for table manager
 * strategy classes.
 */

#include <QApplication>

#include "cplayview.h"
#include "CTblMngr.h"


/**
 * @brief Constructor for table manager.
 * @param parent Pointer to the parent.
 *
 * The constructor creates connections common to all table managers (server and clients).
 */
CTblMngr::CTblMngr(CPlayView *playView, QObject *parent) :
    CTblMngrBase(parent)
{
    this->playView = playView;

    showAll = false;
    showDummy = false;

    createConnections();
}

CTblMngr::~CTblMngr()
{
}

/**
 * @brief Create common table manager connections.
 *
 * These are all from play view to table manager.
 */
void CTblMngr::createConnections()
{
    //From playview to tablemanager.
    connect(playView, &CPlayView::buttonClicked, this, &CTblMngr::buttonClicked);
    connect(playView, &CPlayView::bidValue, this, &CTblMngr::bidValue);
    connect(playView, &CPlayView::bidClose, this, &CTblMngr::bidClose);
    connect(playView, &CPlayView::handClicked, this, &CTblMngr::handClicked);
    connect(playView, &CPlayView::playValue, this, &CTblMngr::playValue);
}

/**
 * @brief Start a new deal in the current session.
 */
void CTblMngr::newDeal()
{
}

/**
 * @brief Rebid the current deal.
 */
void CTblMngr::reBid()
{
}

/**
 * @brief Replay the current deal.
 */
void CTblMngr::rePlay()
{
}

/**
 * @brief Undo bid or trick depending on current mod of play (auction or play).
 */
void CTblMngr::undo()
{
}

//Slots for actor, common for all kinds of table manager.
//-------------------------------------------------------
/**
 * @brief SHow or hide the bid dialog.
 * @param show If true then show the dialog else hide the dialog.
 */
void CTblMngr::sShowBidDialog(bool show)
{
    playView->showBidDialog(show);
}

/**
 * @brief Show bid.
 * @param seat The seat which bids.
 * @param bid The bid.
 */
void CTblMngr::sShowBid(Seat seat, Bids bid, QString features, QString alert)
{
    playView->showBid(seat, bid, features, alert);
}

/**
 * @brief Show bid hint.
 * @param bid The bid hint to set.
 */
void CTblMngr::sBidHint(Bids bid)
{
    playView->setBidHint(bid);
}

/**
 * @brief Show center part of play view.
 * @param vulnerable Indicates vulnerability.
 */
void CTblMngr::sShowCenter(Team vulnerable)
{
    playView->showEWNSTextOnTable();

    if ((vulnerable == NORTH_SOUTH) || (vulnerable == BOTH))
        playView->showNSVulnerableOnTable();
    if ((vulnerable == EAST_WEST) || (vulnerable == BOTH))
        playView->showEWVulnerableOnTable();
}

/**
 * @brief Show dummy's cards in play view.
 * @param seat Dummy's seat
 * @param cards Dummy's cards.
 */
void CTblMngr::sShowDummyCards(Seat seat, int cards[])
{
    showDummy = true;
    dummy = seat;

    playView->setAndShowCards(seat, true, true, cards);
}

/**
 * @brief Indicate who is dummy in play view.
 * @param seat Dummy's seat.
 */
void CTblMngr::sShowDummyOnTable(Seat seat)
{
    playView->showDummyOnTable(seat);
}

/**
 * @brief Indicate who is next to bid/play in play view.
 * @param seat Next bidders/players seat.
 */
void CTblMngr::sShowYourTurnOnTable(Seat seat)
{
    playView->showYourTurnOnTable(seat);
}

/**
 * @brief Clear who is next to bid/play in play view.
 */
void CTblMngr::sClearYourTurnOnTable()
{
    playView->clearYourTurnOnTable();
}

/**
 * @brief Show which card next player plays
 * @param player The player
 * @param card The card.
 */
void CTblMngr::sShowPlayerPlays(Seat player, int card)
{
    playView->showCardOnTable(player, card);
    playView->clearCard(player, card);
}

/**
 * @brief Show play hint.
 * @param player The players seat.
 * @param card The card to show as hint.
 */
void CTblMngr::sPlayHint(Seat player, int card)
{
    playView->setPlayHint(player, card);
}

/**
 * @brief Clear cards played in current trick on play view table.
 */
void CTblMngr::sClearCardsOnTable()
{
    playView->clearCardsOnTable();
}

/**
 * @brief Show number of tricks taken by which side.
 * @param ewTricks East/West tricks.
 * @param nsTricks North/South tricks.
 */
void CTblMngr::sShowTricks(int ewTricks, int nsTricks)
{
    playView->showEWTricks(ewTricks);
    playView->showNSTricks(nsTricks);
}

void CTblMngr::sUndoBid(int noBid)
{
    playView->undoBid(noBid);
}

void CTblMngr::sUndoTrick(int noTrick, int nsTricks, int ewTricks)
{
    bool hideDummy = showDummy && (noTrick == 0) && !showAll;

    if (noTrick == 0)
        showDummy = false;

    playView->undoTrick(noTrick, nsTricks, ewTricks, dummy, hideDummy);
}

/**
 * @brief Enable next bidder tod bid.
 * @param bidder The bidder.
 * @param lastBid The last bid given.
 * @param doubleBid Double/redouble ?
 */
void CTblMngr::sEnableBidder(Seat bidder, Bids lastBid, Bids doubleBid)
{
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_HINT , true));

    playView->enableBidder(bidder, lastBid, doubleBid);
}

/**
 * @brief Disable bidder.
 * @param bidder Current bidder.
 */
void CTblMngr::sDisableBidder(Seat bidder)
{
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_HINT , false));

    playView->disableBidder();
}

/**
 * @brief Enable next player to play.
 * @param player The player.
 */
void CTblMngr::sEnablePlayer(Seat player)
{
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_HINT , true));

    playView->enablePlayer(player);
}

/**
 * @brief  Disableplayer
 * @param player Current player.
 */
void CTblMngr::sDisablePlayer(Seat player)
{
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_HINT , false));

    playView->disablePlayer(player);
}
