/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CPlayView.h
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Bridge table view.

  Platforms: Qt.
  */

/**
 * @file
 * The file implements the declaration of the Bridge table view.
 */

#ifndef CPLAYVIEW_H
#define CPLAYVIEW_H

#include <QGraphicsView>

#include "defines.h"

class QGraphicsScene;
class CCenterCards;
class CCards;
class CMidInfo;
class CTopInfo;
class CBottomInfo;
class CBidDialog;
class CTopInfoAuction;
class CTopInfoPlay;
class CMidInfoAuction;
class CMidInfoPlay;
class CBottomInfoAuction;
class CBottomInfoPlay;
class CBottomInfoButton;

/**
 * @brief This class implements the declaration of the bridge table view.
 *
 * The view divides its scene as shown
 *
 * @image html Scene_PlayView.jpg "Play view scene"
 * \n\n
 * The scene is divided into a number of sub elements.\n
 * Left cards, Top cards, Right cards and Bottom cards are where the bridge players
 * cards are shown (some backside up). It is also here the user interacts with the
 * cards (chooses which to play etc.).\n
 * Center cards is where the cards for the current trick are shown.\n
 * Top info, Mid info and Bottom info are used for displaying different kinds of
 * information for the play (contract, vulnerability, number of tricks taken, trumph suit,
 * bids, cards played etc.).\n\n
 *
 * The structure of the software implementing the play view scene mirrors how the scene
 * is divided:
 *
 * @image html PlayViewDesign_Overview.jpg "Overview of play view"
 * \n\n
 *
 */
class CPlayView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit CPlayView(QWidget *parent = 0);
    ~CPlayView();

public slots:

public:
    /** @name Common group
     * This group of methods are common to all.
     */
    /*@{*/
    void setParams(Seat bottomSeat, int cardBack);
    void resetView();
    /*@}*/

    /** @name Info group
     * This group of methods Handles the Info part of the playview scene.
     */
    /*@{*/
    void setInfoAuction(QString board, Team team, Seat dealer);
    void showInfoAuction(bool show);
    void setInfoPlay(QString board, Team team, Seat dealer, Seat declarer, Bids contract, Bids dbl);
    void showInfoPlay(bool show);
    void showInfoAuctionPlay(bool show);
    void showNSTricks(int tricks);
    void showEWTricks(int tricks);
    void showInfoAuctionButton(bool show, int id = 0);
    void showInfoPlayButton(bool show, int id = 0);
    void showInfoNextButton(bool show, int id = 0);
    void showInfoDDButton(bool show, int id = 0);
    /*@}*/

    /** @name Cards group
     * This group of methods Handles the Cards (Hand) parts of the playview scene.
     */
    /*@{*/
    void setTrumpSuit(Suit trumpSuit);
    void setAndShowAllCards(bool hasWest, bool showWest, int *westCards, bool hasNorth, bool showNorth, int *northCards,
                            bool hasEast, bool showEast, int *eastCards, bool hasSouth, bool showSouth, int *southCards);
    void setAndShowCards(Seat seat, bool hasSeat, bool showSeat, int *cards);
    void showCards(Seat seat, bool showSeat);
    void clearCard(Seat seat, int cardValue);
    void showClearedCard(Seat seat, int noCard);
    void enablePlayer(Seat player);
    void disablePlayer(Seat player);
    void undoTrick(int noTrick, int nsTricks, int ewTricks, Seat dummy, bool hide);
    void undoCard(Seat seat, bool unstack);
    /*@}*/

    /** @name Bid dialog group
     * This group of methods Handles the bid dialog and bid info in the mid info auction display.
     */
    /*@{*/
    void showBidDialog(bool show);
    void showBid(Seat seat, Bids bid, QString features = "", QString alert = "");
    void undoBid(int noBid);
    void enableBidder(Seat bidder, Bids lastBid, Bids doubleBid);
    void disableBidder();
    /*@}*/

    /** @name Center group
     * This group of methods handles the Center part of the playview scene.
     */
    /*@{*/
    void clearCardOnTable(Seat seat);
    void showCardOnTable(Seat seat, int card);
    void clearCardsOnTable();
    void showEWVulnerableOnTable();
    void showNSVulnerableOnTable();
    void clearVulnerableOnTable();
    void showEWNSTextOnTable();
    void clearEWNSTextOnTable();
    void showDummyOnTable(Seat dummy);
    void showYourTurnOnTable(Seat turn);
    void clearYourTurnOnTable();
    void enableLeaderOnTable();
    void disableLeaderOnTable();
    /*@}*/

signals:
    void bidValue(Bids bid);    /**< Next bid to give, as selected by user in the bid dialog. */
    void playValue(int card);   /**< Next card to play, as selected by user. */
    void bidClose();            /**< Close bidding. */
    void buttonClicked(int button); /**< Button clicked. Auction or Play button. */
    void handClicked(Seat seat);    /**< Hand clicked. Requests to display info about hand. */
    void bidClicked(Seat seat, Bids bid);   /**< Bid clicked. Requests to display info about bid. */

private:
    virtual void customEvent(QEvent *event);
    void createSceneAndWidgetsAndLayout();
    void createChildren();

    QGraphicsScene *scene;
    CCenterCards *centerCards;
    CCards *actorCards[4];
    CMidInfo *midInfo;
    CTopInfo *topInfo;
    CBottomInfo *bottomInfo;

    CTopInfoAuction *topInfoAuction;
    CTopInfoPlay *topInfoPlay;
    CMidInfoAuction *midInfoAuction;
    CMidInfoPlay *midInfoPlay;
    CBottomInfoAuction *bottomInfoAuction;
    CBottomInfoPlay *bottomInfoPlay;
    CBottomInfoButton *bottomInfoAuctionButton;
    CBottomInfoButton *bottomInfoPlayButton;
    CBottomInfoButton *bottomInfoNextButton;
    CBottomInfoButton *bottomInfoDDButton;

    CBidDialog *m_pBidDlg;
    bool firstBidDialogShow;

    int cardBack;
    Seat posToSeat[4];
    Position seatToPos[4];
    Suit trumpSuit;
};

#endif // CPLAYVIEW_H
