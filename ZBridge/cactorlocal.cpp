#include "CTblMngr.h"
#include "cbidengine.h"
#include "cplayengine.h"
#include "defines.h"
#include "cactorlocal.h"

CActorLocal::CActorLocal(bool manual, QString teamName, Seat seat, int protocol,
                       CBidOptionDoc &bidOptionDocOwn, CBidOptionDoc &bidOptionDocOpp, CTblMngr *tableManager) :
    CActor(tableManager)
{
    this->manual = manual;
    this->teamName = teamName;
    this->protocol = protocol;
    bidAndPlay.generateEngines(bidOptionDocOwn, bidOptionDocOpp);
    this->tableManager = tableManager;
    showUser = false;

    zBridgeClient_init(&handle);
    zBridgeClientIface_set_client(&handle, seat);

    connect(this, &CActorLocal::sShowAuction, tableManager, &CTblMngr::sShowAuction);
    connect(this, &CActorLocal::sShowPlay, tableManager, &CTblMngr::sShowPlay);
    connect(this, &CActorLocal::sShowBidDialog, tableManager, &CTblMngr::sShowBidDialog);
    connect(this, &CActorLocal::sShowBid, tableManager, &CTblMngr::sShowBid);
    connect(this, &CActorLocal::sShowCenter, tableManager, &CTblMngr::sShowCenter);
    connect(this, &CActorLocal::sShowDummy, tableManager, &CTblMngr::sShowDummy);
    connect(this, &CActorLocal::sShowYourTurn, tableManager, &CTblMngr::sShowYourTurn);
    connect(this, &CActorLocal::sClearYourTurn, tableManager, &CTblMngr::sClearYourTurn);
    connect(this, &CActorLocal::sShowPlayerPlays, tableManager, &CTblMngr::sShowPlayerPlays);
    connect(this, &CActorLocal::sClearCards, tableManager, &CTblMngr::sClearCards);
    connect(this, &CActorLocal::sShowTricks, tableManager, &CTblMngr::sShowTricks);

    connect(this, &CActorLocal::sEnableBidder, tableManager, &CTblMngr::sEnableBidder);
    connect(this, &CActorLocal::sDisableBidder, tableManager, &CTblMngr::sDisableBidder);
    connect(this, &CActorLocal::sEnablePlayer, tableManager, &CTblMngr::sEnablePlayer);
    connect(this, &CActorLocal::sDisablePlayer, tableManager, &CTblMngr::sDisablePlayer);
    connect(this, &CActorLocal::sEnableContinue, tableManager, &CTblMngr::sEnableContinue);
    connect(this, &CActorLocal::sDisableContinue, tableManager, &CTblMngr::sDisableContinue);
}

void CActorLocal::startNewSession()
{ 
    zBridgeClient_enter(&handle);
    clientActions();
}

void CActorLocal::clientActions()
{
    //React to client out events (mostly only one out event is raised).
    if (zBridgeClientIface_israised_connect(&handle))
    {
        emit sConnect(teamName ,  (Seat)zBridgeClientIface_get_client(&handle), protocol);
    }

    else if (zBridgeClientIface_israised_rTNames(&handle))
    {
        emit sRTNames( (Seat)zBridgeClientIface_get_client(&handle));
    }

    else if (zBridgeClientIface_israised_rSBoard(&handle))
    {
        emit sRSBoard( (Seat)zBridgeClientIface_get_client(&handle));
    }

    else if (zBridgeClientIface_israised_rDealInfo(&handle))
    {
        emit sRDealInfo( (Seat)zBridgeClientIface_get_client(&handle));
    }

    else if (zBridgeClientIface_israised_rCards(&handle))
    {
        if (showUser)
        {
            emit sShowAuction();
            emit sShowCenter((Team)zBridgeClientIface_get_vulnerability(&handle));
        }

        emit sRCards((Seat)zBridgeClientIface_get_client(&handle));
    }

    else if (zBridgeClientIface_israised_bidInfo(&handle))
    {
        if (showUser)
        {
            emit sShowBidDialog(false);
            emit sShowBid((Seat)zBridgeClientIface_get_bidder(&handle), BID_BLANK);
            emit sClearYourTurn();
            emit sShowDummy((Seat)((zBridgeClientIface_get_declarer(&handle) + 2) & 3));
            emit sShowPlay();
        }

        bidAndPlay.setBidInfo((Bids)zBridgeClientIface_get_lastBid(&handle),
                               (Bids)zBridgeClientIface_get_bidDouble(&handle),
                               (Seat)zBridgeClientIface_get_bidInfo_value(&handle));
    }

    else if (zBridgeClientIface_israised_clientPlays(&handle))
    {
        if (manual)
        {
            Seat player = (Seat)zBridgeClientIface_get_player(&handle);
            emit sShowYourTurn(player);
            emit sEnablePlayer(player);
        }
        else
        {
            //Calculate automatic play.
            emit sClearYourTurn();
            playValue(bidAndPlay.getNextPlay((Seat)zBridgeClientIface_get_player(&handle),
                                             (Seat)zBridgeClientIface_get_dummy(&handle)));
        }
    }

    else if (zBridgeClientIface_israised_readyForDummyCards(&handle))
    {
        emit sReadyForDummyCards((Seat)zBridgeClientIface_get_client(&handle));
    }

    else if (zBridgeClientIface_israised_getLeader(&handle))
    {
        if (manual)
            emit sEnableContinue();
        else
           continuePlay();
    }

    else if (zBridgeClientIface_israised_undoBid(&handle))
    {

    }

    else if (zBridgeClientIface_israised_undoTrick(&handle))
    {

    }

    //Can come together with undoBid and must be processed after undoBid.
    if (zBridgeClientIface_israised_giveBid(&handle))
    {
        if (manual)
        {
            emit sEnableBidder((Seat)zBridgeClientIface_get_bidder(&handle),
                               (Bids)zBridgeClientIface_get_lastBid(&handle),
                               (Bids)zBridgeClientIface_get_bidEnable(&handle));
        }
        else
        {
            //Calculate automatic bid.
            bidValue(bidAndPlay.getNextBid((Seat)zBridgeClientIface_get_bidder(&handle)));
        }
    }

    //Can come together with undoBid and must be processed after undoBid.
    if (zBridgeClientIface_israised_rBid(&handle))
    {
        emit sRBid((Seat)zBridgeClientIface_get_client(&handle), (Seat)zBridgeClientIface_get_bidder(&handle));
    }

    //Initially comes together with bidInfo and must be processed after bidInfo.
    if (zBridgeClientIface_israised_readyForDummy(&handle))
    {
        emit sReadyForDummy((Seat)zBridgeClientIface_get_client(&handle), zBridgeClientIface_get_noTrick(&handle));
    }

    //Initially comes together with bidInfo and must be processed after bidInfo.
    if (zBridgeClientIface_israised_readyForPlayer(&handle))
    {
        emit sReadyForPlayer((Seat)zBridgeClientIface_get_client(&handle),
                             (Seat)zBridgeClientIface_get_player(&handle), zBridgeClientIface_get_noTrick(&handle));
    }
}

void CActorLocal::clientRunCycle()
{
    //Handle raised in flags.
    zBridgeClient_runCycle(&handle);
    //Handle out flags raised by the run cycle.
    clientActions();
    //Assure all out flags gets cleared (they have been handled now and will otherwise disturb recursive calls).
    zBridgeClient_runCycle(&handle);
}

void CActorLocal::bidValue(Bids bid)
{
    Seat bidder = (Seat)zBridgeClientIface_get_client(&handle);

    if (manual)
        emit sDisableBidder(bidder);

    //Server must continue to next (Bidding Wait) states before clients can send
    //signals to these states.
    emit sBid(bidder, bid);  //Server first then other clients.

    bidDone(bidder, bid);    //This client.
}

void CActorLocal::playValue(int card)
{
    Seat player = (Seat)zBridgeClientIface_get_player(&handle);
    Seat client  = (Seat)zBridgeClientIface_get_client(&handle);

    //For declarer: player can be dummy.
    if (bidAndPlay.cardOk(card, player, player != client))
    {
        if (manual)
            emit sDisablePlayer(player);

        //Server must continue to next (Playing Wait) states before clients can send
        //signals to these states.
        emit sPlayerPlays((Seat)zBridgeClientIface_get_client(&handle), card); //Server first then other clients.

        playerPlays(player, card); //This client.
    }
}

void CActorLocal::continuePlay()
{
    if (manual)
        emit sDisableContinue();

    if (showUser)
    {
        emit sClearCards();
        emit sClearYourTurn();
    }

    zBridgeClientIface_raise_newLeader(&handle, bidAndPlay.getNextLeader());

    if (showUser)
    {
        emit sShowTricks(bidAndPlay.getEWTricks(), bidAndPlay.getNSTricks());
    }

    clientRunCycle();
}

void CActorLocal::seated(QString teamName)
{
    //EAK What to do with teamName?
    zBridgeClientIface_raise_seated(&handle);
    clientRunCycle();
}

void CActorLocal::teamNames(QString nsTeamName, QString ewTeamName)
{
    //EAK What to do  with team names?
    zBridgeClientIface_raise_teamNames(&handle);
    clientRunCycle();
}

void CActorLocal::startOfBoard()
{
    zBridgeClientIface_raise_startOfBoard(&handle);
    clientRunCycle();
}

void CActorLocal::dealInfo(int boardNumber, Seat dealer, Team vulnerability)
{
    zBridgeClientIface_set_boardNumber(&handle, boardNumber);
    zBridgeClientIface_set_dealer(&handle, dealer);
    zBridgeClientIface_set_vulnerability(&handle, vulnerability);

    zBridgeClientIface_raise_dealInfo(&handle);
    clientRunCycle();
}

void CActorLocal::cards(Seat player, int cards[])
{
    bidAndPlay.setActorsCards(cards);

    bidAndPlay.resetBidHistory();

    if (showUser)
    {
        emit sShowBidDialog(true);
        emit sShowBid((Seat)zBridgeClientIface_get_dealer(&handle), BID_PLAYER);
    }

    zBridgeClientIface_raise_cards(&handle);
    clientRunCycle();
}

void CActorLocal::bidDone(Seat bidder, Bids bid)
{
    CBid bidEntry(bidder, bid, "alert");

    bidAndPlay.appendBid(bidEntry);

    if (showUser)
    {
        emit sShowBid(bidder, bid);
        emit sShowBid((Seat)((bidder + 1) & 3), BID_PLAYER);
    }

    zBridgeClientIface_raise_bidDone(&handle, bid);
    clientRunCycle();
}

void CActorLocal::playerToLead(Seat player)
{
    //This signal requires special attention. It might be received in the Bid state and
    //in the Play state and in the Lead state. This somewhat messy handling is due to the
    //way the protocol is structured.
    zBridgeClientIface_raise_playerToLead(&handle);
    clientRunCycle();
}

void CActorLocal::dummyToLead()
{
    //This signal requires special attention. It might be received in the Play state and in the
    //Lead state. This somewhat messy handling is due to the way the protocol is structured.
    zBridgeClientIface_raise_dummyToLead(&handle);
    clientRunCycle();
}

void CActorLocal::playerPlays(Seat player, int card)
{
    int trick = zBridgeClientIface_get_noTrick(&handle);
    bidAndPlay.setPlay(player, trick, card);

    if (showUser)
        emit sShowPlayerPlays(player, card);

    zBridgeClientIface_raise_playerPlays(&handle, card);
    clientRunCycle();
}

void CActorLocal::dummyCards(int cards[])
{
    bidAndPlay.setDummysCards(cards);

    zBridgeClientIface_raise_dummyCards(&handle);
    clientRunCycle();
}

void CActorLocal::undoBid(bool reBid)
{
    int lastBidder;

    if (reBid)
    {
        lastBidder = -1;
        bidAndPlay.resetBidHistory();
    }
    else
    {
        Bids bid;
        lastBidder = bidAndPlay.bidUndo(&bid);
        zBridgeClientIface_set_bidVal(&handle, bid);
    }
    zBridgeClientIface_raise_undo(&handle, lastBidder);
    clientRunCycle();
}

void CActorLocal::undoTrick(bool rePlay)
{
    int noTrick;

    if (rePlay)
    {
        bidAndPlay.resetPlayHistory();
        noTrick = 0;
    }
    else
        noTrick = bidAndPlay.playUndo();

    zBridgeClientIface_raise_undo(&handle, noTrick);
    clientRunCycle();
}

void CActorLocal::endOfSession()
{
    zBridgeClientIface_raise_endOfSession(&handle);
    clientRunCycle();
}
