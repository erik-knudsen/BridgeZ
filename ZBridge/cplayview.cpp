#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsAnchorLayout>
#include <QGraphicsWidget>
#include <QGraphicsAnchor>
#include <cassert>

#include "cplayview.h"
#include "ccentercards.h"
#include "clcards.h"
#include "ctcards.h"
#include "crcards.h"
#include "cbcards.h"
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

void CPlayView::createSceneAndWidgetsAndLayout()
{
    scene = new QGraphicsScene(0, 0, SCENE_HOR_SIZE, SCENE_VER_SIZE, this);
    setBackgroundBrush(Qt::gray);
    setScene(scene);

    centerCards = new CCenterCards();
    centerCards->setPreferredSize(QSizeF(CENTER_HOR_SIZE, CENTER_VER_SIZE));
    centerCards->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    centerCards->connectButton(this);

    lCards = new CLCards();
    lCards->setPreferredSize(QSizeF(LR_CARD_HOR_SIZE, CENTER_VER_SIZE));
    lCards->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    lCards->connectCards(this);

    tCards = new CTCards();
    tCards->setPreferredSize(QSizeF(TB_CARD_HOR_SIZE, TB_CARD_VER_SIZE));
    tCards->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    tCards->connectCards(this);

    rCards = new CRCards();
    rCards->setPreferredSize(QSizeF(LR_CARD_HOR_SIZE, LR_CARD_VER_SIZE));
    rCards->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    rCards->connectCards(this);

    bCards = new CBCards();
    bCards->setPreferredSize(QSizeF(TB_CARD_HOR_SIZE, TB_CARD_VER_SIZE));
    bCards->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    bCards->connectCards(this);

    midInfo = new CMidInfo();
    midInfo->setPreferredSize(QSizeF(INF_HOR_SIZE, INF_VER_SIZE));
    midInfo->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    topInfo = new CTopInfo();
    topInfo->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    topInfo->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    bottomInfo = new CBottomInfo();
    bottomInfo->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    bottomInfo->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    l->setSpacing(0);

    QGraphicsWidget *w = new QGraphicsWidget;
    w->setPos(0, 0);
    w->setLayout(l);

    l->addAnchor(tCards, Qt::AnchorTop, l, Qt::AnchorTop);
    l->addAnchor(lCards, Qt::AnchorLeft, l, Qt::AnchorLeft);
    l->addAnchor(bCards, Qt::AnchorBottom, l, Qt::AnchorBottom);
    l->addAnchor(rCards, Qt::AnchorRight, l, Qt::AnchorRight);
    l->addAnchor(lCards, Qt::AnchorRight, centerCards, Qt::AnchorLeft);
    l->addAnchor(lCards, Qt::AnchorVerticalCenter, centerCards, Qt::AnchorVerticalCenter);
    l->addAnchor(tCards, Qt::AnchorBottom, centerCards, Qt::AnchorTop);
    l->addAnchor(tCards, Qt::AnchorHorizontalCenter, centerCards, Qt::AnchorHorizontalCenter);
    l->addAnchor(rCards, Qt::AnchorLeft, centerCards, Qt::AnchorRight);
    l->addAnchor(rCards, Qt::AnchorVerticalCenter, centerCards, Qt::AnchorVerticalCenter);
    l->addAnchor(bCards, Qt::AnchorTop, centerCards, Qt::AnchorBottom);
    l->addAnchor(bCards, Qt::AnchorHorizontalCenter, centerCards, Qt::AnchorHorizontalCenter);
    l->addAnchor(midInfo, Qt::AnchorLeft, rCards, Qt::AnchorRight);
    l->addAnchor(midInfo, Qt::AnchorTop, rCards, Qt::AnchorTop);
    l->addAnchor(topInfo, Qt::AnchorBottom, midInfo, Qt::AnchorTop);
    l->addAnchor(topInfo, Qt::AnchorLeft, midInfo, Qt::AnchorLeft);
    l->addAnchor(bottomInfo, Qt::AnchorTop, midInfo, Qt::AnchorBottom);
    l->addAnchor(bottomInfo, Qt::AnchorLeft, midInfo, Qt::AnchorLeft);

    scene->addItem(w);
}

void CPlayView::createChildren()
{
    topInfoAuction = new CTopInfoAuction();
    topInfoAuction->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    topInfoAuction->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    topInfoAuction->setParentItem(topInfo);
    topInfoAuction->hide();

    topInfoPlay = new CTopInfoPlay();
    topInfoPlay->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    topInfoPlay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    topInfoPlay->setParentItem(topInfo);
    topInfoPlay->hide();

    midInfoAuction = new CMidInfoAuction();
    midInfoAuction->setPreferredSize(QSizeF(INF_HOR_SIZE, INF_VER_SIZE));
    midInfoAuction->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    midInfoAuction->setParentItem(midInfo);
    midInfoAuction->hide();
    midInfoAuction->connectBids(this);

    midInfoPlay = new CMidInfoPlay();
    midInfoPlay->setPreferredSize(QSizeF(INF_HOR_SIZE, INF_VER_SIZE));
    midInfoPlay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    midInfoPlay->setParentItem(midInfo);
    midInfoPlay->hide();

    bottomInfoAuction = new CBottomInfoAuction();
    bottomInfoAuction->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    bottomInfoAuction->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    bottomInfoAuction->setParentItem(bottomInfo);
    bottomInfoAuction->hide();

    bottomInfoPlay = new CBottomInfoPlay();
    bottomInfoPlay->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    bottomInfoPlay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    bottomInfoPlay->setParentItem(bottomInfo);
    bottomInfoPlay->hide();

    bottomInfoAuctionButton = new CBottomInfoButton(INF_HOR_SIZE, TB_CARD_VER_SIZE, "Start\nAuction");
    bottomInfoAuctionButton->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    bottomInfoAuctionButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    bottomInfoAuctionButton->connectButton(this);
    bottomInfoAuctionButton->setParentItem(bottomInfo);
    bottomInfoAuctionButton->hide();

    bottomInfoPlayButton = new CBottomInfoButton(INF_HOR_SIZE, TB_CARD_VER_SIZE, "Start\nPlay");
    bottomInfoPlayButton->setPreferredSize(QSizeF(INF_HOR_SIZE, TB_CARD_VER_SIZE));
    bottomInfoPlayButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    bottomInfoPlayButton->connectButton(this);
    bottomInfoPlayButton->setParentItem(bottomInfo);
    bottomInfoPlayButton->hide();
}

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

void CPlayView::setParams(Seat bottomSeat, int cardBack)
{
    this->bottomSeat = bottomSeat;
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

void CPlayView::resetView()
{
    lCards->setEnabled(false);
    tCards->setEnabled(false);
    rCards->setEnabled(false);
    bCards->setEnabled(false);

    centerCards->setEnabled(false);

    topInfoAuction->hide();
    topInfoPlay->hide();
    midInfoAuction->hide();
    midInfoAuction->reset();
    midInfoPlay->hide();
    bottomInfoAuction->hide();
    bottomInfoPlay->hide();
    bottomInfoAuctionButton->hide();
    bottomInfoPlayButton->hide();

    m_pBidDlg->hide();

    lCards->clearCards();
    tCards->clearCards();
    rCards->clearCards();
    bCards->clearCards();

    lCards->setTrumpSuit(ANY);
    tCards->setTrumpSuit(ANY);
    rCards->setTrumpSuit(ANY);
    bCards->setTrumpSuit(ANY);

    clearCards();
    clearVulnerable();
    clearEWNSText();
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
    lCards->setTrumpSuit(trumpSuit);
    tCards->setTrumpSuit(trumpSuit);
    rCards->setTrumpSuit(trumpSuit);
    bCards->setTrumpSuit(trumpSuit);
}

void CPlayView::showCards(bool hasWest, int *westCards, bool hasNorth, int *northCards,
                          bool hasEast, int *eastCards, bool hasSouth, int *southCards)
{
    Position pos;

    lCards->clearCards();
    tCards->clearCards();
    rCards->clearCards();
    bCards->clearCards();

    if (hasWest)
    {
        pos = seatToPos[WEST_SEAT];
        if (pos == LEFT_POS)
        for (int i = 0; i < 13; i++)
            lCards->setFace(westCards[i]);
        else if (pos == TOP_POS)
        for (int i = 0; i < 13; i++)
            tCards->setFace(westCards[i]);
        else if (pos == RIGHT_POS)
        for (int i = 0; i < 13; i++)
            rCards->setFace(westCards[i]);
        else if (pos == BOTTOM_POS)
        for (int i = 0; i < 13; i++)
            bCards->setFace(westCards[i]);
    }

    if (hasNorth)
    {
        pos = seatToPos[NORTH_SEAT];
        if (pos == LEFT_POS)
        for (int i = 0; i < 13; i++)
            lCards->setFace(northCards[i]);
        else if (pos == TOP_POS)
        for (int i = 0; i < 13; i++)
            tCards->setFace(northCards[i]);
        else if (pos == RIGHT_POS)
        for (int i = 0; i < 13; i++)
            rCards->setFace(northCards[i]);
        else if (pos == BOTTOM_POS)
        for (int i = 0; i < 13; i++)
            bCards->setFace(northCards[i]);
    }

    if (hasEast)
    {
        pos = seatToPos[EAST_SEAT];
        if (pos == LEFT_POS)
        for (int i = 0; i < 13; i++)
            lCards->setFace(eastCards[i]);
        else if (pos == TOP_POS)
        for (int i = 0; i < 13; i++)
            tCards->setFace(eastCards[i]);
        else if (pos == RIGHT_POS)
        for (int i = 0; i < 13; i++)
            rCards->setFace(eastCards[i]);
        else if (pos == BOTTOM_POS)
        for (int i = 0; i < 13; i++)
            bCards->setFace(eastCards[i]);
    }

    if (hasSouth)
    {
        pos = seatToPos[SOUTH_SEAT];
        if (pos == LEFT_POS)
        for (int i = 0; i < 13; i++)
            lCards->setFace(southCards[i]);
        else if (pos == TOP_POS)
        for (int i = 0; i < 13; i++)
            tCards->setFace(southCards[i]);
        else if (pos == RIGHT_POS)
        for (int i = 0; i < 13; i++)
            rCards->setFace(southCards[i]);
        else if (pos == BOTTOM_POS)
        for (int i = 0; i < 13; i++)
            bCards->setFace(southCards[i]);
    }

    pos = seatToPos[WEST_SEAT];
    if (hasWest)
    {
        if (pos == LEFT_POS)
            lCards->showFaces();
        else if (pos == TOP_POS)
            tCards->showFaces();
        else if (pos == RIGHT_POS)
            rCards->showFaces();
        else if (pos == BOTTOM_POS)
            bCards->showFaces();
    }
    else
    {
        if (pos == LEFT_POS)
            lCards->showBacks(cardBack);
        else if (pos == TOP_POS)
            tCards->showBacks(cardBack);
        else if (pos == RIGHT_POS)
            rCards->showBacks(cardBack);
        else if (pos == BOTTOM_POS)
            bCards->showBacks(cardBack);
    }

    pos = seatToPos[NORTH_SEAT];
    if (hasNorth)
    {
        if (pos == LEFT_POS)
            lCards->showFaces();
        else if (pos == TOP_POS)
            tCards->showFaces();
        else if (pos == RIGHT_POS)
            rCards->showFaces();
        else if (pos == BOTTOM_POS)
            bCards->showFaces();
    }
    else
    {
        if (pos == LEFT_POS)
            lCards->showBacks(cardBack);
        else if (pos == TOP_POS)
            tCards->showBacks(cardBack);
        else if (pos == RIGHT_POS)
            rCards->showBacks(cardBack);
        else if (pos == BOTTOM_POS)
            bCards->showBacks(cardBack);
    }

    pos = seatToPos[EAST_SEAT];
    if (hasEast)
    {
        if (pos == LEFT_POS)
            lCards->showFaces();
        else if (pos == TOP_POS)
            tCards->showFaces();
        else if (pos == RIGHT_POS)
            rCards->showFaces();
        else if (pos == BOTTOM_POS)
            bCards->showFaces();
    }
    else
    {
        if (pos == LEFT_POS)
            lCards->showBacks(cardBack);
        else if (pos == TOP_POS)
            tCards->showBacks(cardBack);
        else if (pos == RIGHT_POS)
            rCards->showBacks(cardBack);
        else if (pos == BOTTOM_POS)
            bCards->showBacks(cardBack);
    }

    pos = seatToPos[SOUTH_SEAT];
    if (hasSouth)
    {
        if (pos == LEFT_POS)
            lCards->showFaces();
        else if (pos == TOP_POS)
            tCards->showFaces();
        else if (pos == RIGHT_POS)
            rCards->showFaces();
        else if (pos == BOTTOM_POS)
            bCards->showFaces();
    }
    else
    {
        if (pos == LEFT_POS)
            lCards->showBacks(cardBack);
        else if (pos == TOP_POS)
            tCards->showBacks(cardBack);
        else if (pos == RIGHT_POS)
            rCards->showBacks(cardBack);
        else if (pos == BOTTOM_POS)
            bCards->showBacks(cardBack);
    }
}

void CPlayView::showBid(Seat seat, Bids bid)
{
    midInfoAuction->showBid(seat, bid);

    if (bid == BID_PLAYER)
        showYourTurn(seat);
}

void CPlayView::undoBid(int noBid)
{
}

void CPlayView::undoTrick(int wCard, int nCard, int eCard, int sCard)
{
}

void CPlayView::clearCard(Seat seat)
{
    Position pos = seatToPos[seat];
    centerCards->clearCard(pos);
}

void CPlayView::showCard(Seat seat, int card)
{
    Position pos = seatToPos[seat];
    centerCards->showCard(pos, card);
}

void CPlayView::clearCards()
{
    centerCards->clearCard(LEFT_POS);
    centerCards->clearCard(TOP_POS);
    centerCards->clearCard(RIGHT_POS);
    centerCards->clearCard(BOTTOM_POS);
}

void CPlayView::showEWVulnerable()
{
    Position pos = seatToPos[WEST_SEAT];
    centerCards->showVulnerable(pos);
    pos = seatToPos[EAST_SEAT];
    centerCards->showVulnerable(pos);
}

void CPlayView::showNSVulnerable()
{
    Position pos = seatToPos[NORTH_SEAT];
    centerCards->showVulnerable(pos);
    pos = seatToPos[SOUTH_SEAT];
    centerCards->showVulnerable(pos);
}

void CPlayView::clearVulnerable()
{
    centerCards->clearVulnerable(LEFT_POS);
    centerCards->clearVulnerable(TOP_POS);
    centerCards->clearVulnerable(RIGHT_POS);
    centerCards->clearVulnerable(BOTTOM_POS);
}

void CPlayView::showEWNSText()
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

void CPlayView::clearEWNSText()
{
    centerCards->clearText(LEFT_POS);
    centerCards->clearText(TOP_POS);
    centerCards->clearText(RIGHT_POS);
    centerCards->clearText(BOTTOM_POS);
}

void CPlayView::showDummy(Seat dummy)
{
    Position pos = seatToPos[dummy];
    centerCards->showText(pos, 'D');
}

void CPlayView::showYourTurn(Seat turn)
{
    Position pos = seatToPos[turn];
    centerCards->showYourTurn(pos);
}

void CPlayView::clearYourTurn()
{
    centerCards->clearYourTurn();
}

void CPlayView::showFace(Seat seat, int cardValue, bool visible)
{
    assert((cardValue >= 0) && (cardValue <= 51));

    Position pos = seatToPos[seat];
    switch(pos)
    {
    case LEFT_POS:
        lCards->showFace(cardValue, visible);
        break;

    case TOP_POS:
        tCards->showFace(cardValue, visible);
        break;

    case RIGHT_POS:
        rCards->showFace(cardValue, visible);
        break;

    default:
        bCards->showFace(cardValue, visible);
        break;
    }
}

void CPlayView::showBack(Seat seat, int backIndex, bool visible)
{
    assert((backIndex >= 0) && (backIndex <= 12));

    Position pos = seatToPos[seat];

        switch(pos)
        {
        case LEFT_POS:
            lCards->showBack(backIndex, visible);
            break;

        case TOP_POS:
            tCards->showBack(backIndex, visible);
            break;

        case RIGHT_POS:
            rCards->showBack(backIndex, visible);
            break;

        default:
            bCards->showBack(backIndex, visible);
            break;
        }
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
    Position pos = seatToPos[player];

    switch(pos)
    {
    case LEFT_POS:
        lCards->setEnabled(true);
        break;

    case TOP_POS:
        tCards->setEnabled(true);
        break;

    case RIGHT_POS:
        rCards->setEnabled(true);
        break;

    default:
        bCards->setEnabled(true);
        break;
    }
}

void CPlayView::disablePlayer(Seat player)
{
    Position pos = seatToPos[player];

    switch(pos)
    {
    case LEFT_POS:
        lCards->setEnabled(false);
        break;

    case TOP_POS:
        tCards->setEnabled(false);
        break;

    case RIGHT_POS:
        rCards->setEnabled(false);
        break;

    default:
        bCards->setEnabled(false);
        break;
    }
}

void CPlayView::enableContinue()
{
    centerCards->setEnabled(true);
}

void CPlayView::disableContinue()
{
    centerCards->setEnabled(false);
}
