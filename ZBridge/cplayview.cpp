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
    connect(m_pBidDlg, &CBidDialog::bidBackup, this, &CPlayView::bidBackup);
    connect(m_pBidDlg, &CBidDialog::bidHint, this, &CPlayView::bidHint);
    connect(m_pBidDlg, &CBidDialog::bidRestart, this, &CPlayView::bidRestart);
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
 *     (Left, Top, Right or Bottom). It is converted to a playvalue signal.
 *   - WMS_HAND_CLICKED
 *     This event is generated when the hand is clicked in one of the cards view
 *     (Left, Top, Right or bottom). It is converted to a handClicked signal.
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
    //Reset and clean up cards (Left, Top, Right, Bottom).
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

    //Hide bid dialog.
    m_pBidDlg->hide();

    //Clean up table.
    clearCardsOnTable();
    clearVulnerableOnTable();
    clearEWNSTextOnTable();
}

void CPlayView::setInfoAuction(QString board, Team team, Seat dealer)
{
    topInfoAuction->setBoardId(board);
    topInfoAuction->setVulnerability(team);
    topInfoAuction->setDealer(dealer);
}

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

void CPlayView::setInfoPlay(QString board, Team team, Seat dealer, Seat declarer, Bids contract, Bids dbl)
{
    topInfoPlay->setBoardId(board);
    topInfoPlay->setVulnerability(team);
    topInfoPlay->setDealer(dealer);
    topInfoPlay->setDeclarer(declarer);
    topInfoPlay->setContract(contract, dbl);
}

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

void CPlayView::showNSTricks(int tricks)
{
    bottomInfoPlay->showNSTricks(tricks);
}

void CPlayView::showEWTricks(int tricks)
{
    bottomInfoPlay->showEWTricks(tricks);
}

void CPlayView::showInfoAuctionButton(bool show, int id)
{
    if (show)
    {
        bottomInfoAuction->hide();
        bottomInfoAuctionButton->setButtonId(id);
        bottomInfoAuctionButton->show();
    }
    else
    {
        bottomInfoAuctionButton->hide();
        bottomInfoAuction->show();
    }
}

void CPlayView::showInfoPlayButton(bool show, int id)
{
    if (show)
    {
        bottomInfoPlay->hide();
        bottomInfoAuctionButton->setButtonId(id);
        bottomInfoPlayButton->show();
    }
    else
    {
        bottomInfoPlayButton->hide();
        bottomInfoPlay->show();
    }
}

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

void CPlayView::setTrumpSuit(Suit trumpSuit)
{
    this->trumpSuit = trumpSuit;

    for (int i = 0; i < 4; i++)
    {
        actorCards[i]->setTrumpSuit(trumpSuit);
        actorCards[i]->showCards(true);
    }
}

void CPlayView::setAndShowAllCards(bool hasWest, bool showWest, int *westCards, bool hasNorth, bool showNorth, int *northCards, bool hasEast, bool showEast, int *eastCards, bool hasSouth, bool showSouth, int *southCards)
{
    trumpSuit = ANY;

    setAndShowCards(WEST_SEAT, hasWest, showWest, westCards);
    setAndShowCards(NORTH_SEAT, hasNorth, showNorth, northCards);
    setAndShowCards(EAST_SEAT, hasEast, showEast, eastCards);
    setAndShowCards(SOUTH_SEAT, hasSouth, showSouth, southCards);
}

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

void CPlayView::showBid(Seat seat, Bids bid)
{
    midInfoAuction->showBid(seat, bid);
}

void CPlayView::undoBid(int noBid)
{
}

void CPlayView::undoTrick(int wCard, int nCard, int eCard, int sCard)
{
}

void CPlayView::clearCardOnTable(Seat seat)
{
    Position pos = seatToPos[seat];
    centerCards->clearCardOnTable(pos);
}

void CPlayView::showCardOnTable(Seat seat, int card)
{
    Position pos = seatToPos[seat];
    centerCards->showCardOnTable(pos, card);
}

void CPlayView::clearCardsOnTable()
{
    centerCards->clearCardOnTable(LEFT_POS);
    centerCards->clearCardOnTable(TOP_POS);
    centerCards->clearCardOnTable(RIGHT_POS);
    centerCards->clearCardOnTable(BOTTOM_POS);
}

void CPlayView::showEWVulnerableOnTable()
{
    Position pos = seatToPos[WEST_SEAT];
    centerCards->showVulnerable(pos);
    pos = seatToPos[EAST_SEAT];
    centerCards->showVulnerable(pos);
}

void CPlayView::showNSVulnerableOnTable()
{
    Position pos = seatToPos[NORTH_SEAT];
    centerCards->showVulnerable(pos);
    pos = seatToPos[SOUTH_SEAT];
    centerCards->showVulnerable(pos);
}

void CPlayView::clearVulnerableOnTable()
{
    centerCards->clearVulnerable(LEFT_POS);
    centerCards->clearVulnerable(TOP_POS);
    centerCards->clearVulnerable(RIGHT_POS);
    centerCards->clearVulnerable(BOTTOM_POS);
}

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

void CPlayView::clearEWNSTextOnTable()
{
    centerCards->clearText(LEFT_POS);
    centerCards->clearText(TOP_POS);
    centerCards->clearText(RIGHT_POS);
    centerCards->clearText(BOTTOM_POS);
}

void CPlayView::showDummyOnTable(Seat dummy)
{
    Position pos = seatToPos[dummy];
    centerCards->showText(pos, 'D');
}

void CPlayView::showYourTurnOnTable(Seat turn)
{
    Position pos = seatToPos[turn];
    centerCards->showYourTurn(pos);
}

void CPlayView::clearYourTurnOnTable()
{
    centerCards->clearYourTurn();
}

void CPlayView::clearCard(Seat seat, int cardValue)
{
    assert((cardValue >= 0) && (cardValue <= 51));

    actorCards[seatToPos[seat]]->clearCard(cardValue);
}

void CPlayView::showClearedCard(Seat seat, int noCard)
{   
    actorCards[seatToPos[seat]]->showClearedCard(noCard);
}

void CPlayView::enableBidder(Seat bidder, Bids lastBid, Bids doubleBid)
{
    m_pBidDlg->enableBidder(bidder, lastBid, doubleBid);
}

void CPlayView::disableBidder(Seat bidder)
{
    m_pBidDlg->disableBidder(bidder);
}

void CPlayView::enablePlayer(Seat player)
{
    actorCards[seatToPos[player]]->setEnabled(true);
}

void CPlayView::disablePlayer(Seat player)
{
    actorCards[seatToPos[player]]->setEnabled(false);
}

void CPlayView::enableContinueOnTable()
{
    centerCards->setEnabled(true);
}

void CPlayView::disableContinueOnTable()
{
    centerCards->setEnabled(false);
}
