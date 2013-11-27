#include "cplayview.h"
#include "CTblMngr.h"

CTblMngr::CTblMngr(CPlayView *playView, QObject *parent) :
    QObject(parent)
{
    this->playView = playView;

    showAll = false;

    createConnections();
}

CTblMngr::~CTblMngr()
{
}

void CTblMngr::createConnections()
{
    //From playview to tablemanager.
    connect(playView, &CPlayView::buttonClicked, this, &CTblMngr::buttonClicked);
    connect(playView, &CPlayView::bidValue, this, &CTblMngr::bidValue);
    connect(playView, &CPlayView::bidBackup, this, &CTblMngr::bidBackup);
    connect(playView, &CPlayView::bidHint, this, &CTblMngr::bidHint);
    connect(playView, &CPlayView::bidRestart, this, &CTblMngr::bidRestart);
    connect(playView, &CPlayView::bidClose, this, &CTblMngr::bidClose);
    connect(playView, &CPlayView::handClicked, this, &CTblMngr::handClicked);
    connect(playView, &CPlayView::playValue, this, &CTblMngr::playValue);
}

void CTblMngr::newDeal()
{
}

void CTblMngr::reBid()
{
}

void CTblMngr::rePlay()
{
}

void CTblMngr::undo()
{
}

//Slots for actor, common for all kinds of table manager.
void CTblMngr::sShowBidDialog(bool show)
{
    playView->showBidDialog(show);
}

void CTblMngr::sShowBid(Seat seat, Bids bid)
{
    playView->showBid(seat, bid);
}

void CTblMngr::sShowCenter(Team vulnerable)
{
    playView->showEWNSTextOnTable();

    if ((vulnerable == NORTH_SOUTH) || (vulnerable == BOTH))
        playView->showNSVulnerableOnTable();
    if ((vulnerable == EAST_WEST) || (vulnerable == BOTH))
        playView->showEWVulnerableOnTable();
}

void CTblMngr::sShowDummyCards(Seat seat, int cards[])
{
    playView->setAndShowCards(seat, true, true, cards);
}

void CTblMngr::sShowDummyOnTable(Seat seat)
{
    playView->showDummyOnTable(seat);
}

void CTblMngr::sShowYourTurnOnTable(Seat seat)
{
    playView->showYourTurnOnTable(seat);
}

void CTblMngr::sClearYourTurnOnTable()
{
    playView->clearYourTurnOnTable();
}

void CTblMngr::sShowPlayerPlays(Seat player, int card)
{
    playView->showCardOnTable(player, card);
    playView->clearCard(player, card);
}

void CTblMngr::sClearCardsOnTable()
{
    playView->clearCardsOnTable();
}

void CTblMngr::sShowTricks(int ewTricks, int nsTricks)
{
    playView->showEWTricks(ewTricks);
    playView->showNSTricks(nsTricks);
}

void CTblMngr::sEnableBidder(Seat bidder, Bids lastBid, Bids doubleBid)
{
    playView->enableBidder(bidder, lastBid, doubleBid);
}

void CTblMngr::sDisableBidder(Seat bidder)
{
    playView->disableBidder(bidder);
}

void CTblMngr::sEnablePlayer(Seat player)
{
    playView->enablePlayer(player);
}

void CTblMngr::sDisablePlayer(Seat player)
{
    playView->disablePlayer(player);
}
