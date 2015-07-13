/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CPlayView.cpp
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Bridge table view.

  Platforms: Qt.
  */

/**
 * @file
 * The file implements the definition of the Bridge table view.
 */

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsAnchorLayout>
#include <QGraphicsWidget>
#include <QGraphicsAnchor>
#include <cassert>

#include "cplayview.h"
#include "ccentercards.h"
#include "clrcards.h"
#include "ctbcards.h"
#include "cmidinfo.h"
#include "ctopinfo.h"
#include "cbottominfo.h"
#include "ccard.h"
#include "cbiddialog.h"
#include "ctopinfoauction.h"
#include "ctopinfoplay.h"
#include "cmidinfoauction.h"
#include "cmidinfoplay.h"
#include "cbottominfoauction.h"
#include "cbottominfoplay.h"
#include "cbottominfobutton.h"

/**
 * @brief Constructor for playview.
 * @param parent Pointer to the parent.
 *
 * The constructor:
 *   - Creates the scene, the widgets for the scene and the layout of the widgets
 *     in the scene.
 *   - Creates the children for the widgets in the scene.
 *   - Sets initial parameters for bottom seat and for card backs.
 *   - Allocates the bid dialog and hides it initially.
 *   - Connects bid dialog signals to the play view.
 */
CPlayView::CPlayView(QWidget *parent) :
    QGraphicsView(parent)
{
    createSceneAndWidgetsAndLayout();
    createChildren();
    setParams(SOUTH_SEAT, 0);

    m_pBidDlg = new CBidDialog(this);
    m_pBidDlg->hide();
    firstBidDialogShow = true;

    connect(m_pBidDlg, &CBidDialog::bidValue, this, &CPlayView::bidValue);
    connect(m_pBidDlg, &CBidDialog::bidClose, this, &CPlayView::bidClose);
}

CPlayView::~CPlayView()
{
}

/**
 * @brief Create the scene for the play view.
 *
 * The scene is allocated. The widgets for the scene are allocated and initialized
 * and the layout is set.
 */
void CPlayView::createSceneAndWidgetsAndLayout()
{
    //Allocate and initializethe scene.
    scene = new QGraphicsScene(0, 0, SCENE_HOR_SIZE, SCENE_VER_SIZE, this);
    setBackgroundBrush(Qt::gray);
    setScene(scene);

    //Allocate and initialize widget for the center of the scene.
    centerCards = new CCenterCards();
    centerCards->setPreferredSize(QSizeF(CENTER_HOR_SIZE, CENTER_VER_SIZE));
    centerCards->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    centerCards->connectButton(this);

    //Allocate and initialize widgets for the four hands.
    actorCards[LEFT_POS] = new CLRCards(LEFT_POS);
    actorCards[TOP_POS] = new CTBCards(TOP_POS);
    actorCards[RIGHT_POS] = new CLRCards(RIGHT_POS);
    actorCards[BOTTOM_POS] = new CTBCards(BOTTOM_POS);

    actorCards[LEFT_POS]->setPreferredSize(QSizeF(LR_CARD_HOR_SIZE, CENTER_VER_SIZE));
    actorCards[RIGHT_POS]->setPreferredSize(QSizeF(LR_CARD_HOR_SIZE, LR_CARD_VER_SIZE));
    actorCards[TOP_POS]->setPreferredSize(QSizeF(TB_CARD_HOR_SIZE, TB_CARD_VER_SIZE));
    actorCards[BOTTOM_POS]->setPreferredSize(QSizeF(TB_CARD_HOR_SIZE, TB_CARD_VER_SIZE));

    for (int i = 0; i < 4; i++)
    {
        actorCards[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        actorCards[i]->connectCards(this);
    }

    //Allocate and initialize the mid infor widget.
    midInfo = new CMidInfo();
    midInfo->setPreferredSize(QSizeF(INF_HOR_SIZE, INF_VER_SIZE));
    midInfo->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    //Allocate and initialize the top info widget.
    topInfo = new CTopInfo();
    topInfo->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    topInfo->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    //Allocat and initialize the bottom info widget.
    bottomInfo = new CBottomInfo();
    bottomInfo->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    bottomInfo->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    //Layout the widgets in the scene.
    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setSpacing(0);

    QGraphicsWidget *w = new QGraphicsWidget;
    w->setPos(0, 0);
    w->setLayout(l);

    l->addAnchor(actorCards[TOP_POS], Qt::AnchorTop, l, Qt::AnchorTop);
    l->addAnchor(actorCards[LEFT_POS], Qt::AnchorLeft, l, Qt::AnchorLeft);
    l->addAnchor(actorCards[BOTTOM_POS], Qt::AnchorBottom, l, Qt::AnchorBottom);
    l->addAnchor(actorCards[RIGHT_POS], Qt::AnchorRight, l, Qt::AnchorRight);
    l->addAnchor(actorCards[LEFT_POS], Qt::AnchorRight, centerCards, Qt::AnchorLeft);
    l->addAnchor(actorCards[LEFT_POS], Qt::AnchorVerticalCenter, centerCards, Qt::AnchorVerticalCenter);
    l->addAnchor(actorCards[TOP_POS], Qt::AnchorBottom, centerCards, Qt::AnchorTop);
    l->addAnchor(actorCards[TOP_POS], Qt::AnchorHorizontalCenter, centerCards, Qt::AnchorHorizontalCenter);
    l->addAnchor(actorCards[RIGHT_POS], Qt::AnchorLeft, centerCards, Qt::AnchorRight);
    l->addAnchor(actorCards[RIGHT_POS], Qt::AnchorVerticalCenter, centerCards, Qt::AnchorVerticalCenter);
    l->addAnchor(actorCards[BOTTOM_POS], Qt::AnchorTop, centerCards, Qt::AnchorBottom);
    l->addAnchor(actorCards[BOTTOM_POS], Qt::AnchorHorizontalCenter, centerCards, Qt::AnchorHorizontalCenter);
    l->addAnchor(midInfo, Qt::AnchorLeft, actorCards[RIGHT_POS], Qt::AnchorRight);
    l->addAnchor(midInfo, Qt::AnchorTop, actorCards[RIGHT_POS], Qt::AnchorTop);
    l->addAnchor(topInfo, Qt::AnchorBottom, midInfo, Qt::AnchorTop);
    l->addAnchor(topInfo, Qt::AnchorLeft, midInfo, Qt::AnchorLeft);
    l->addAnchor(bottomInfo, Qt::AnchorTop, midInfo, Qt::AnchorBottom);
    l->addAnchor(bottomInfo, Qt::AnchorLeft, midInfo, Qt::AnchorLeft);

    scene->addItem(w);
}

/**
 * @brief Create children for the info widgets in the scene.
 */
void CPlayView::createChildren()
{
    //Top info for auction part of bridge play.
    topInfoAuction = new CTopInfoAuction();
    topInfoAuction->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    topInfoAuction->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    topInfoAuction->setParentItem(topInfo);
    topInfoAuction->hide();

    //Top info for play part of bridge play.
    topInfoPlay = new CTopInfoPlay();
    topInfoPlay->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    topInfoPlay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    topInfoPlay->setParentItem(topInfo);
    topInfoPlay->hide();

    //Mid info for auction part of bridge play.
    midInfoAuction = new CMidInfoAuction();
    midInfoAuction->setPreferredSize(QSizeF(INF_HOR_SIZE, INF_VER_SIZE));
    midInfoAuction->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    midInfoAuction->setParentItem(midInfo);
    midInfoAuction->hide();
    midInfoAuction->connectBids(this);

    //Mid info for play part of bridge play.
    midInfoPlay = new CMidInfoPlay();
    midInfoPlay->setPreferredSize(QSizeF(INF_HOR_SIZE, INF_VER_SIZE));
    midInfoPlay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    midInfoPlay->setParentItem(midInfo);
    midInfoPlay->hide();

    //Bottom info for auction part of bridge play.
    bottomInfoAuction = new CBottomInfoAuction();
    bottomInfoAuction->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    bottomInfoAuction->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    bottomInfoAuction->setParentItem(bottomInfo);
    bottomInfoAuction->hide();

    //Bottom info for play part of bridge play.
    bottomInfoPlay = new CBottomInfoPlay();
    bottomInfoPlay->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    bottomInfoPlay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    bottomInfoPlay->setParentItem(bottomInfo);
    bottomInfoPlay->hide();

    //Auction  button to display in bottom info.
    bottomInfoAuctionButton = new CBottomInfoButton(INF_HOR_SIZE, TB_CARD_VER_SIZE, "Start\nAuction");
    bottomInfoAuctionButton->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    bottomInfoAuctionButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    bottomInfoAuctionButton->connectButton(this);
    bottomInfoAuctionButton->setParentItem(bottomInfo);
    bottomInfoAuctionButton->hide();

    //Play button to display in bottom info.
    bottomInfoPlayButton = new CBottomInfoButton(INF_HOR_SIZE, TB_CARD_VER_SIZE, "Start\nPlay");
    bottomInfoPlayButton->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    bottomInfoPlayButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    bottomInfoPlayButton->connectButton(this);
    bottomInfoPlayButton->setParentItem(bottomInfo);
    bottomInfoPlayButton->hide();

    //Next deal button to display in bottom info.
    bottomInfoNextButton = new CBottomInfoButton(INF_HOR_SIZE, TB_CARD_VER_SIZE, "Start\nNext Deal");
    bottomInfoNextButton->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    bottomInfoNextButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    bottomInfoNextButton->connectButton(this);
    bottomInfoNextButton->setParentItem(bottomInfo);
    bottomInfoNextButton->hide();
}

/**
 * @brief Custom events for play view.
 * @param event A custom event.
 *
 * The play view uses custom events for some events. This seemed to give a simpler
 * implementation for these events instead of using signals and slots. These events
 * are converted to signals here. The following events are handled:
 *   - WMS_CARD_CLICKED
 *     This event is generated when a card is clicked in one of the cards view
 *     (Left, Top, Right or Bottom hand). It is converted to a playvalue signal.
 *   - WMS_HAND_CLICKED
 *     This event is generated when the hand is clicked in one of the cards view
 *     (Left, Top, Right or bottom hand). It is converted to a handClicked signal.
 *   - WMS_BUTTON_CLICKED
 *     This event is generated when one of the buttons (Auction or Play or Continue)
 *     is clicked. It is converted to a buttonClicked signal.
 *   - WMS_BID_CLICKED
 *     This event is generated when a bid is clicked in the mid info auction widget.
 *     It is converted to a bidClicked signal.
 */
void CPlayView::customEvent(QEvent *event)
{
    if (event->type() == WMS_CARD_CLICKED)
    {
        CARD_CLICKED_Event * ev = static_cast<CARD_CLICKED_Event *>(event);

        int cardValue = ev->getCardValue();

        emit playValue(cardValue);
    }
    else if (event->type() == WMS_HAND_CLICKED)
    {
        HAND_CLICKED_Event * ev = static_cast<HAND_CLICKED_Event *>(event);

        Position pos = ev->getPos();

        emit handClicked(posToSeat[pos]);
    }
    else if (event->type() == WMS_BUTTON_CLICKED)
    {
        BUTTON_CLICKED_Event * ev = static_cast<BUTTON_CLICKED_Event *>(event);

        int button = ev->getButton();

        emit buttonClicked(button);
    }
    else if (event->type() == WMS_BID_CLICKED)
    {
        BID_CLICKED_Event *ev = static_cast<BID_CLICKED_Event *>(event);
        Seat seat = ev->getSeat();
        Bids bid = ev->getBid();

        emit bidClicked(seat, bid);
    }
}

/**
 * @brief Set parameters for play view.
 * @param bottomSeat Identifies which seat to display as the bottom seat.
 * @param cardBack Identifies cardback picture.
 *
 * Saves cardback information and creates mapping arrays for seat to position
 * and for position to seat.
 */
void CPlayView::setParams(Seat bottomSeat, int cardBack)
{
    this->cardBack = cardBack;

    //Mapping of position to seat.
    posToSeat[LEFT_POS] =   (bottomSeat == WEST_SEAT) ? NORTH_SEAT :
                            (bottomSeat == NORTH_SEAT) ? EAST_SEAT :
                            (bottomSeat == EAST_SEAT) ? SOUTH_SEAT :
                            WEST_SEAT;
    posToSeat[TOP_POS] =    (bottomSeat == WEST_SEAT) ? EAST_SEAT :
                            (bottomSeat == NORTH_SEAT) ? SOUTH_SEAT :
                            (bottomSeat == EAST_SEAT) ? WEST_SEAT :
                            NORTH_SEAT;
    posToSeat[RIGHT_POS] =  (bottomSeat == WEST_SEAT) ? SOUTH_SEAT :
                            (bottomSeat == NORTH_SEAT) ? WEST_SEAT :
                            (bottomSeat == EAST_SEAT) ? NORTH_SEAT :
                            EAST_SEAT;
    posToSeat[BOTTOM_POS] = (bottomSeat == WEST_SEAT) ? WEST_SEAT :
                            (bottomSeat == NORTH_SEAT) ? NORTH_SEAT :
                            (bottomSeat == EAST_SEAT) ? EAST_SEAT :
                            SOUTH_SEAT;

    //Mapping of seat to position.
    seatToPos[WEST_SEAT] =  (bottomSeat == WEST_SEAT) ? BOTTOM_POS :
                            (bottomSeat == NORTH_SEAT) ? RIGHT_POS :
                            (bottomSeat == EAST_SEAT) ? TOP_POS :
                            LEFT_POS;
    seatToPos[NORTH_SEAT] = (bottomSeat == WEST_SEAT) ? LEFT_POS :
                            (bottomSeat == NORTH_SEAT) ? BOTTOM_POS :
                            (bottomSeat == EAST_SEAT) ? RIGHT_POS :
                            TOP_POS;
    seatToPos[EAST_SEAT] =  (bottomSeat == WEST_SEAT) ? TOP_POS :
                            (bottomSeat == NORTH_SEAT) ? LEFT_POS :
                            (bottomSeat == EAST_SEAT) ? BOTTOM_POS :
                            RIGHT_POS;
    seatToPos[SOUTH_SEAT] = (bottomSeat == WEST_SEAT) ? RIGHT_POS :
                            (bottomSeat == NORTH_SEAT) ? TOP_POS :
                            (bottomSeat == EAST_SEAT) ? LEFT_POS :
                                                        BOTTOM_POS;
}

/**
 * @brief Reset the play view.
 *
 * Does a complete reset and clean up of the play view.
 */
void CPlayView::resetView()
{
    //Reset and clean up cards (Left, Top, Right, Bottom hand).
    for (int i = 0; i < 4; i++)
    {
        actorCards[i]->setEnabled(false);
        actorCards[i]->clearCards();
        actorCards[i]->setTrumpSuit(ANY);
    }

    //Reset and clean up the center.
    centerCards->setEnabled(false);

    //Reset and clean up Info part of scene.
    topInfoAuction->hide();
    topInfoPlay->hide();
    midInfoAuction->hide();
    midInfoAuction->reset();
    midInfoPlay->hide();
    bottomInfoAuction->hide();
    bottomInfoPlay->hide();
    bottomInfoAuctionButton->hide();
    bottomInfoPlayButton->hide();
    bottomInfoNextButton->hide();

    //Hide bid dialog.
    m_pBidDlg->hide();
    disableBidder();

    //Clean up table.
    clearCardsOnTable();
    clearVulnerableOnTable();
    clearEWNSTextOnTable();
}

/**
 * @brief Set info for the info auction widget.
 * @param board Identification of the board.
 * @param team The vulnerabilities.
 * @param dealer The dealer.
 *
 * Initialize the parameters for the info auction widget. The widget belongs
 * to the top info part of the scene. The widget is meant to be shown during
 * the bidding auction part of the bridge play.
 */
void CPlayView::setInfoAuction(QString board, Team team, Seat dealer)
{
    topInfoAuction->setBoardId(board);
    topInfoAuction->setVulnerability(team);
    topInfoAuction->setDealer(dealer);
}

/**
 * @brief Show or hide the info auction widget.
 * @param show If (true) then show the widget else hide the widget.
 */
void CPlayView::showInfoAuction(bool show)
{
    if (show)
    {
        topInfoAuction->show();
        midInfoAuction->show();
        bottomInfoAuction->show();
    }
    else
    {
        topInfoAuction->hide();
        midInfoAuction->hide();
        bottomInfoAuction->hide();
    }
}

/**
 * @brief Set info for the info play widget.
 * @param board Identification of the board.
 * @param team The vulnerabilities.
 * @param dealer The dealer.
 * @param declarer The declarer.
 * @param contract The contract.
 * @param dbl Indicates if the contract is doubled (or redoubled).
 *
 * Initialize the parameters for the info play widget. The widget belongs
 * to the top info part of the scene. The widget is meant to be shown during the play
 * part of the bridge play.
 */
void CPlayView::setInfoPlay(QString board, Team team, Seat dealer, Seat declarer, Bids contract, Bids dbl)
{
    topInfoPlay->setBoardId(board);
    topInfoPlay->setVulnerability(team);
    topInfoPlay->setDealer(dealer);
    topInfoPlay->setDeclarer(declarer);
    topInfoPlay->setContract(contract, dbl);
}

/**
 * @brief Show or hide the info play widget.
 * @param show If (true) then show the widget else hide the widget.
 */
void CPlayView::showInfoPlay(bool show)
{
    if (show)
    {
        topInfoPlay->show();
        midInfoPlay->show();
        bottomInfoPlay->show();
    }
    else
    {
        topInfoPlay->hide();
        midInfoPlay->hide();
        bottomInfoPlay->hide();
    }
}

/**
 * @brief Show or hide the info auction andplay widget.
 * @param show If (true) then show the widget else hide the widget.
 */
void CPlayView::showInfoAuctionPlay(bool show)
{
    if (show)
    {
        topInfoPlay->show();
        midInfoAuction->show();
        bottomInfoPlay->show();
    }
    else
    {
        topInfoPlay->hide();
        midInfoAuction->hide();
        bottomInfoPlay->hide();
    }
}

/**
 * @brief Show number of tricks taken by the North/South partners.
 * @param tricks Nuber of tricks.
 *
 * The number of tricks are shown in the bottom play info part of the scene.
 */
void CPlayView::showNSTricks(int tricks)
{
    bottomInfoPlay->showNSTricks(tricks);
}

/**
 * @brief Show number of tricks taken by the East/West partners.
 * @param tricks Nuber of tricks.
 *
 * The number of tricks are shown in the bottom play info part of the scene.
 */
void CPlayView::showEWTricks(int tricks)
{
    bottomInfoPlay->showEWTricks(tricks);
}

/**
 * @brief Activate and show/hide the auction button.
 * @param show if (true) then show the button else hide the button.
 * @param id Identification of the button.
 *
 * The button is shown in the bottom info widget of the scene.
 */
void CPlayView::showInfoAuctionButton(bool show, int id)
{
    if (show)
    {
        bottomInfoAuction->hide();
        bottomInfoPlay->hide();
        bottomInfoAuctionButton->setButtonId(id);
        bottomInfoAuctionButton->show();
    }
    else
        bottomInfoAuctionButton->hide();
}

/**
 * @brief Activate and show/hide the play button.
 * @param show if (true) then show the button else hide the button.
 * @param id Identification of the button.
 *
 * The button is shown in the bottom info widget of the scene.
 */
void CPlayView::showInfoPlayButton(bool show, int id)
{
    if (show)
    {
        bottomInfoAuction->hide();
        bottomInfoPlay->hide();
        bottomInfoPlayButton->setButtonId(id);
        bottomInfoPlayButton->show();
    }
    else
        bottomInfoPlayButton->hide();
}

void CPlayView::showInfoNextButton(bool show, int id)
{
    if (show)
    {
        bottomInfoAuction->hide();
        bottomInfoPlay->hide();
        bottomInfoAuctionButton->hide();
        bottomInfoPlayButton->hide();
        bottomInfoNextButton->setButtonId(id);
        bottomInfoNextButton->show();
    }
    else
        bottomInfoNextButton->hide();
}

/**
 * @brief Set the trump suit of the bridge play
 * @param trumpSuit The trumpsuit.
 *
 * Sets the trumpsuit and rearranges display of cards according to this.
 */
void CPlayView::setTrumpSuit(Suit trumpSuit)
{
    this->trumpSuit = trumpSuit;

    for (int i = 0; i < 4; i++)
    {
        actorCards[i]->setTrumpSuit(trumpSuit);
        actorCards[i]->showCards(true);
    }
}

/**
 * @brief Set and show all cards.
 * @param hasWest If true then the West cards are present.
 * @param showWest If true then the West card faces should be shown in the scene.
 * @param westCards The West cards.
 * @param hasNorth If true then the North cards are present.
 * @param showNorth If true then the North card faces should be shown in the scene.
 * @param northCards The North cards.
 * @param hasEast If true then the East cards are present.
 * @param showEast If true then the East card faces should be shown in the scene.
 * @param eastCards The East cards.
 * @param hasSouth If true then the South cards are present.
 * @param showSouth If true then the South card faces should be shown in the scene.
 * @param southCards The South cards.
 *
 * The cards are set if they are present and shown in the cards part of the scene
 * either with their faces or with their backs. Arranged according to the trumpsuit.
 */
void CPlayView::setAndShowAllCards(bool hasWest, bool showWest, int *westCards, bool hasNorth, bool showNorth, int *northCards, bool hasEast, bool showEast, int *eastCards, bool hasSouth, bool showSouth, int *southCards)
{
    trumpSuit = ANY;

    setAndShowCards(WEST_SEAT, hasWest, showWest, westCards);
    setAndShowCards(NORTH_SEAT, hasNorth, showNorth, northCards);
    setAndShowCards(EAST_SEAT, hasEast, showEast, eastCards);
    setAndShowCards(SOUTH_SEAT, hasSouth, showSouth, southCards);
}

/**
 * @brief Set and show cards for one seat.
 * @param seat The seat to show cards for.
 * @param hasSeat If true then the cards are present.
 * @param showSeat If true then the cards faces should be shown in the scene.
 * @param cards The cards.
 *
 * The cards are set for the given seat if they are present and shown in this seats
 * part of the scene. Arranged according to the trumpsuit.
 */
void CPlayView::setAndShowCards(Seat seat, bool hasSeat, bool showSeat, int *cards)
{
    Position pos = seatToPos[seat];

    actorCards[pos]->clearCards();
    actorCards[pos]->setBackValues(cardBack);
    if (hasSeat)
        for (int i = 0; i < 13; i++)
            actorCards[pos]->setCardValue(cards[i]);
    actorCards[pos]->setShowBack(!(hasSeat && showSeat));
    actorCards[pos]->setTrumpSuit(trumpSuit);
    actorCards[pos]->showCards(true);
}

void CPlayView::showCards(Seat seat, bool showSeat)
{
    Position pos = seatToPos[seat];

    actorCards[pos]->setShowBack(!showSeat);
    actorCards[pos]->showCards(true);
}

/**
 * @brief Hide card from cards display of scene.
 * @param seat The seat with the card to hide.
 * @param cardValue The card to hide.
 *
 * This is meant to be used when the user has played a card to remove it from the hand.
 */
void CPlayView::clearCard(Seat seat, int cardValue)
{
    assert((cardValue >= 0) && (cardValue <= 51));

    actorCards[seatToPos[seat]]->clearCard(cardValue);
}

/**
 * @brief Show hidden card again in cards display of scene.
 * @param seat The seat with the card to shoe.
 * @param noCard Number of cards to still not show.
 *
 * The cards are shown in opposite sequence of which they were hidden. This
 * is meant to be used when a trick is undone.
 */
void CPlayView::showClearedCard(Seat seat, int noCard)
{
    actorCards[seatToPos[seat]]->showClearedCard(noCard);
}

/**
 * @brief Enable player.
 * @param player The player to enable.
 */
void CPlayView::enablePlayer(Seat player)
{
    actorCards[seatToPos[player]]->setEnabled(true);
}

/**
 * @brief Disable player.
 * @param player The player to disable.
 */
void CPlayView::disablePlayer(Seat player)
{
    actorCards[seatToPos[player]]->setEnabled(false);
}

/**
 * @brief Undo trick(s).
 * @param noTrick Number of (complete) tricks to keep.
 * @param nsTricks Number of NS tricks after undo.
 * @param ewTricks Number of EW tricks after undo.
 * @param dummy Dummy seat.
 * @param hide If true then dummy should be hidden on replay.
 *
 * Step back tricks in play view. Tricks are unstacked and card(s) are
 * returned to the hand from where they came. After undo the top level
 * trick (4 cards) in the stack is shown in the center display. In case
 * undo unstacks all tricks then dummy hand will be hidden if required.
 *
 */
void CPlayView::undoTrick(int noTrick, int nsTricks, int ewTricks, Seat dummy, bool hide)
{
    assert(noTrick == (nsTricks + ewTricks));

    //Clear cards in center  display.
    clearCardsOnTable();

    //Unstack trick(s) and return to hands.
    for (int i = 0; i < 4; i++)
        actorCards[i]->showClearedCard(noTrick);

    //Replay?
    if (noTrick > 0)
        //Not replay. Show cards for previous trick in center display.
        for (int i = 0; i < 4; i++)
        {
            int card = actorCards[i]->getTopCard();
            centerCards->showCardOnTable((Position)i, card);
        }
    else if (hide)
        //Replay. Hide dummy hand.
        showCards(dummy, false);

    //Show number of tricks after undo.
    showNSTricks(nsTricks);
    showEWTricks(ewTricks);
}

/**
 * @brief Undo card.
 * @param seat Player seat to undo one card from.
 * @param unstack if true unstack. Otherwise just show top four cards in center display.
 *
 * Step back one card in play view. The card is unstacked and
 * returned to the hand from where it came. The card is also cleared
 * from the center display. In case the stack size now is the same for
 * all seats (all cards cleared from the center display), then the four
 * top cards are shown in the center  display.
 *
 */
void CPlayView::undoCard(Seat seat, bool unstack)
{
    if (unstack)
    {
        Position pos = seatToPos[seat];

        //Unstack trick and return to hand.
        actorCards[pos]->showTopClearedCard();

        //Clear card in center  display.
        centerCards->clearCardOnTable(pos);
    }

    //Show previous trick in center display?
    int size_0 = actorCards[0]->getNotVisibleValuesSize();
    int size_1 = actorCards[1]->getNotVisibleValuesSize();
    int size_2 = actorCards[2]->getNotVisibleValuesSize();
    int size_3 = actorCards[3]->getNotVisibleValuesSize();
    if ((size_0 != 0) &&  ((size_0 == size_1) && (size_1 == size_2) && (size_2 == size_3)))
    {
        for (int i = 0; i < 4; i++)
        {
            int card = actorCards[i]->getTopCard();
            centerCards->showCardOnTable((Position)i, card);
        }
    }
}

/**
 * @brief Show/hide the bid dialog.
 * @param show if (true) then show the dialog else hide the dialog.
 */
void CPlayView::showBidDialog(bool show)
{
    if (show)
    {
        m_pBidDlg->show();
        if (firstBidDialogShow)
        {
            QPoint point(m_pBidDlg->pos());
            m_pBidDlg->move(point.x() - 350, point.y() - 100);
            firstBidDialogShow = false;
        }
    }
    else
        m_pBidDlg->hide();
}

/**
 * @brief Show bid in mid info auction widget.
 * @param seat The seat to show the bid for.
 * @param bid The bid to show.
 */
void CPlayView::showBid(Seat seat, Bids bid)
{
    midInfoAuction->showBid(seat, bid);
}

/**
 * @brief Undo bid.
 * @param noBid Number of bids to keep.
 */
void CPlayView::undoBid(int noBid)
{
    midInfoAuction->undoBid(noBid);
}

/**
 * @brief Enable bidder.
 * @param bidder The bidder to enable.
 * @param lastBid The last bid given.
 * @param doubleBid Double/redouble if any.
 */
void CPlayView::enableBidder(Seat bidder, Bids lastBid, Bids doubleBid)
{
    m_pBidDlg->enableBidder(bidder, lastBid, doubleBid);
}

/**
 * @brief Disable bidder.
 */
void CPlayView::disableBidder()
{
    m_pBidDlg->disableBidder();
}

/**
 * @brief Clear card in center widget of scene.
 * @param seat The seat to clear the card for.
 *
 * This is meant to be used during the play to update the display of the current trick.
 */
void CPlayView::clearCardOnTable(Seat seat)
{
    Position pos = seatToPos[seat];
    centerCards->clearCardOnTable(pos);
}

/**
 * @brief Show card in center widget of scene.
 * @param seat The seat to show the card for.
 * @param card The card to show.
 *
 * This is meant to be used during the play to update the display of the current trick.
 */
void CPlayView::showCardOnTable(Seat seat, int card)
{
    Position pos = seatToPos[seat];
    centerCards->showCardOnTable(pos, card);
}

/**
 * @brief Clear all cards in center widget of the scene.
 *
 * This is meant to be done during the play to prepare the display for the next trick.
 */
void CPlayView::clearCardsOnTable()
{
    centerCards->clearCardOnTable(LEFT_POS);
    centerCards->clearCardOnTable(TOP_POS);
    centerCards->clearCardOnTable(RIGHT_POS);
    centerCards->clearCardOnTable(BOTTOM_POS);
}

/**
 * @brief Show East/West vulnerability in the center widget of the scene.
 */
void CPlayView::showEWVulnerableOnTable()
{
    Position pos = seatToPos[WEST_SEAT];
    centerCards->showVulnerable(pos);
    pos = seatToPos[EAST_SEAT];
    centerCards->showVulnerable(pos);
}

/**
 * @brief Show North/South vulnerability in the center widget of the scene.
 */
void CPlayView::showNSVulnerableOnTable()
{
    Position pos = seatToPos[NORTH_SEAT];
    centerCards->showVulnerable(pos);
    pos = seatToPos[SOUTH_SEAT];
    centerCards->showVulnerable(pos);
}

/**
 * @brief Clear vulnerability indications in the center widget of the scene.
 */
void CPlayView::clearVulnerableOnTable()
{
    centerCards->clearVulnerable(LEFT_POS);
    centerCards->clearVulnerable(TOP_POS);
    centerCards->clearVulnerable(RIGHT_POS);
    centerCards->clearVulnerable(BOTTOM_POS);
}

/**
 * @brief Label East, West, North, South positions in the center widget of the scene.
 */
void CPlayView::showEWNSTextOnTable()
{
    Position pos;

    pos = seatToPos[WEST_SEAT];
    centerCards->showText(pos, 'W');

    pos = seatToPos[NORTH_SEAT];
    centerCards->showText(pos, 'N');

    pos = seatToPos[EAST_SEAT];
    centerCards->showText(pos, 'E');

    pos = seatToPos[SOUTH_SEAT];
    centerCards->showText(pos, 'S');
}
/**
 * @brief Clear East, West, North, South position labels in the center widget of the scene.
 */

void CPlayView::clearEWNSTextOnTable()
{
    centerCards->clearText(LEFT_POS);
    centerCards->clearText(TOP_POS);
    centerCards->clearText(RIGHT_POS);
    centerCards->clearText(BOTTOM_POS);
}

/**
 * @brief Indicate dummy's position in the center widget of the scene.
 * @param dummy Dummy's seat.
 */
void CPlayView::showDummyOnTable(Seat dummy)
{
    Position pos = seatToPos[dummy];
    centerCards->showText(pos, 'D');
}

/**
 * @brief Indicate who is next to bid or play in the center widget of the scene.
 * @param turn The seat which is next.
 */
void CPlayView::showYourTurnOnTable(Seat turn)
{
    Position pos = seatToPos[turn];
    centerCards->showYourTurn(pos);
}

/**
 * @brief Clear who is next to bid or play in the center widget of the scene.
 */
void CPlayView::clearYourTurnOnTable()
{
    centerCards->clearYourTurn();
}

/**
 * @brief Enable continue play.
 */
void CPlayView::enableLeaderOnTable()
{
    centerCards->setEnabled(true);
}

/**
 * @brief Disable continue play.
 */
void CPlayView::disableLeaderOnTable()
{
    centerCards->setEnabled(false);
}
