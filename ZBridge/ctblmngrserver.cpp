#include "../src-gen/sc_types.h"
#include "czbridgedoc.h"
#include "cplayview.h"
#include "cactor.h"
#include "cactorlocal.h"
#include "cactorremote.h"
#include "cbidengine.h"
#include "cplayengine.h"
#include "ctblmngrserver.h"
#include "cremoteactorserver.h"


//Table Manager server.
CTblMngrServer::CTblMngrServer(CZBridgeDoc *doc, CPlayView *playView, QObject *parent) :
    CTblMngr(playView, parent)
{
    this->doc = doc;
    this->playView = playView;

    boardNo = -1;

    actors[WEST_SEAT] = 0;
    actors[NORTH_SEAT] = 0;
    actors[EAST_SEAT] = 0;
    actors[SOUTH_SEAT] = 0;

    //Start tcp server for remote clients.
    remoteActorServer = new CRemoteActorServer(PROTOCOLS[doc->getSeatOptions().protocol],
                                               QHostAddress(doc->getSeatOptions().host),
                                               doc->getSeatOptions().port.toInt(), this);

    //Timer for supervision of continue button.
    continueButton = new QTimer(this);
    connect(continueButton, &QTimer::timeout, this, &CTblMngr::sContinuePlay);
    continueButton->setSingleShot(true);
    waiting = false;
}

CTblMngrServer::~CTblMngrServer()
{
}

void CTblMngrServer::cleanTableManager()
{
    playView->resetView();

    if (actors[WEST_SEAT] != 0)
    {
        actors[WEST_SEAT]->endOfSession();
        delete actors[WEST_SEAT];
    }
    if (actors[NORTH_SEAT] != 0)
    {
        actors[NORTH_SEAT]->endOfSession();
        delete actors[NORTH_SEAT];
    }
    if (actors[EAST_SEAT] != 0)
    {
        actors[EAST_SEAT]->endOfSession();
        delete actors[EAST_SEAT];
    }
    if (actors[SOUTH_SEAT] != 0)
    {
        actors[SOUTH_SEAT]->endOfSession();
        delete actors[SOUTH_SEAT];
    }

    actors[WEST_SEAT] = 0;
    actors[NORTH_SEAT] = 0;
    actors[EAST_SEAT] = 0;
    actors[SOUTH_SEAT] = 0;
}

//Give new deal in accordance with what the user has selected.
void CTblMngrServer::giveNewDeal()
{
    int i, j, inx;
    int cardDeck[52];
    Seat currentDealer;

    boardNo++;

    currentBoardNo = boardNo;
    currentDealer = dealer[boardNo%4];
    currentVulnerable = vulnerable[(boardNo%4 + boardNo/4)%4];
    zBridgeServerIface_set_dealer(&handle, currentDealer);

    //Shuffle card deck.
    for (i = 0; i < 52; i++)
        cardDeck[i] = i;
    QTime cur;
    qsrand(cur.currentTime().msec());
    for (i = 0; i < 52; i++)
    {
        inx = rand()%52;
        j = cardDeck[i];
        cardDeck[i] = cardDeck[inx];
        cardDeck[inx] = j;
    }

    for (i = 0; i < 4; i++)
        for (j = 0; j < 13; j++)
            currentCards[i][j] = cardDeck[i * 13 + j];
}

//Methods activated by user through main frame menus.
//-----------------------------------------------------------------------------
void CTblMngrServer::newSession()
{
    CActor *actor;

    cleanTableManager();

    boardNo = 0;

    protocol = PROTOCOLS[doc->getSeatOptions().protocol];

    QString ewTeamName = "ewTeam";
    QString nsTeamName = "nsTeam";

    if (ACTORS[doc->getSeatOptions().westActor] == MANUAL_ACTOR)
        actor = new CActorLocal(true, ewTeamName, WEST_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if (remoteActorServer->isConnected(WEST_SEAT))
        actor = new CActorRemote(WEST_SEAT, remoteActorServer->getFrontend(WEST_SEAT), this);
    else
        actor = new CActorLocal(false, ewTeamName, WEST_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    actors[WEST_SEAT] = actor;

    if (ACTORS[doc->getSeatOptions().northActor] == MANUAL_ACTOR)
        actor = new CActorLocal(true, nsTeamName, NORTH_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if (remoteActorServer->isConnected(NORTH_SEAT))
        actor = new CActorRemote(NORTH_SEAT, remoteActorServer->getFrontend(NORTH_SEAT), this);
    else
        actor = new CActorLocal(false, nsTeamName, NORTH_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    actors[NORTH_SEAT] = actor;

    if (ACTORS[doc->getSeatOptions().eastActor] == MANUAL_ACTOR)
        actor = new CActorLocal(true, ewTeamName, EAST_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if (remoteActorServer->isConnected(EAST_SEAT))
        actor = new CActorRemote(EAST_SEAT, remoteActorServer->getFrontend(EAST_SEAT), this);
    else
        actor = new CActorLocal(false, ewTeamName, EAST_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    actors[EAST_SEAT] = actor;

    if (ACTORS[doc->getSeatOptions().southActor] == MANUAL_ACTOR)
        actor = new CActorLocal(true, nsTeamName, SOUTH_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if (remoteActorServer->isConnected(SOUTH_SEAT))
        actor = new CActorRemote(SOUTH_SEAT, remoteActorServer->getFrontend(SOUTH_SEAT), this);
    else
        actor = new CActorLocal(false, nsTeamName, SOUTH_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    actors[SOUTH_SEAT] = actor;

    setShowUser(showAll);

    zBridgeServer_init(&handle);
    zBridgeServerIface_set_noOfBoards(&handle, 16);
    zBridgeServer_enter(&handle);
    serverActions();

    //Start actors.
    actors[WEST_SEAT]->startNewSession();
    actors[NORTH_SEAT]->startNewSession();
    actors[EAST_SEAT]->startNewSession();
    actors[SOUTH_SEAT]->startNewSession();
}

void CTblMngrServer::newDeal()
{
    zBridgeServerIface_raise_newDeal(&handle);
    serverRunCycle();
}

void CTblMngrServer::reBid()
{
    zBridgeServerIface_raise_undo(&handle, -1);
    serverRunCycle();
}

void CTblMngrServer::rePlay()
{
    if (zBridgeServer_isActive(&handle, ZBridgeServer_entry__Playing))
    {
        zBridgeServerIface_raise_undo(&handle, 0);
        serverRunCycle();
    }
}

void CTblMngrServer::undo()
{
    if (zBridgeServer_isActive(&handle, ZBridgeServer_entry__Bidding))
    {
        int bidder;
        Bids bidVal;
        bidder = bidHistory.undo(&bidVal);
        if (bidder != -1)
            zBridgeServerIface_set_bidVal(&handle, bidVal);
        zBridgeServerIface_raise_undo(&handle, bidder);
        serverRunCycle();
    }
    else if (zBridgeServer_isActive(&handle, ZBridgeServer_entry__Playing))
    {
        int noTrick;

        noTrick = playHistory.undo();
        zBridgeServerIface_raise_undo(&handle, noTrick);
        serverRunCycle();
    }
}

void CTblMngrServer::serverActions()
{
    //React to server out events (only one out event is allowed).
    if (zBridgeServerIface_israised_seated(&handle))
    {
        Seat seat = (Seat)zBridgeServerIface_get_seated_value(&handle);

        char * teamName = ((seat == WEST_SEAT) || (seat == EAST_SEAT)) ?
                    zBridgeServerIface_get_ewTeamName(&handle): zBridgeServerIface_get_nsTeamName(&handle);

        actors[seat]->seated(QString(teamName));
    }

    else if (zBridgeServerIface_israised_teamNames(&handle))
    {
        QString nsTeamName = QString(zBridgeServerIface_get_nsTeamName(&handle));
        QString ewTeamName = QString(zBridgeServerIface_get_ewTeamName(&handle));
        actors[WEST_SEAT]->teamNames(nsTeamName, ewTeamName);
        actors[NORTH_SEAT]->teamNames(nsTeamName, ewTeamName);
        actors[EAST_SEAT]->teamNames(nsTeamName, ewTeamName);
        actors[SOUTH_SEAT]->teamNames(nsTeamName, ewTeamName);
    }

    else if (zBridgeServerIface_israised_startOfBoard(&handle))
    {
        startOfBoard();
    }
    else if (zBridgeServerIface_israised_startOfBoardDelayed(&handle))
    {
        QTimer::singleShot(1000, this, SLOT(startOfBoard()));
    }

    else if (zBridgeServerIface_israised_dealInfo(&handle))
    {
        Seat seat = (Seat)zBridgeServerIface_get_dealInfo_value(&handle);

        actors[seat]->dealInfo(currentBoardNo, (Seat)zBridgeServerIface_get_dealer(&handle), currentVulnerable);
    }

    else if (zBridgeServerIface_israised_cards(&handle))
    {
        if (showUser)
        {
            sShowAuction();
            sShowCenter(currentVulnerable);
        }

        bool showWest = showAll || (ACTORS[doc->getSeatOptions().westActor] == MANUAL_ACTOR);
        bool showNorth = showAll || (ACTORS[doc->getSeatOptions().northActor] == MANUAL_ACTOR);
        bool showEast = showAll || (ACTORS[doc->getSeatOptions().eastActor] == MANUAL_ACTOR);
        bool showSouth = showAll || (ACTORS[doc->getSeatOptions().southActor] == MANUAL_ACTOR);

        playView->setAndShowAllCards(true, showWest, currentCards[WEST_SEAT], true, showNorth, currentCards[NORTH_SEAT],
                            true, showEast, currentCards[EAST_SEAT], true, showSouth, currentCards[SOUTH_SEAT]);


        bidHistory.resetBidHistory();

        zBridgeServerIface_raise_continue(&handle);
        serverRunCycle();

        actors[WEST_SEAT]->cards(currentCards);
        actors[NORTH_SEAT]->cards(currentCards);
        actors[EAST_SEAT]->cards(currentCards);
        actors[SOUTH_SEAT]->cards(currentCards);

        if (showUser)
        {
            sShowBidDialog(true);
            sShowBid((Seat)zBridgeServerIface_get_dealer(&handle), BID_PLAYER);
        }
    }

    else if (zBridgeServerIface_israised_bidDone(&handle))
    {
        Seat bidder = (Seat)zBridgeServerIface_get_bidDone_value(&handle);
        Bids bid = (Bids)zBridgeServerIface_get_bidVal(&handle);

        if (showUser)
        {
            sShowBid(bidder, bid);
            sShowBid((Seat)((bidder + 1) & 3), BID_PLAYER);
        }

        //Set bid in bid history.
        CBid bidEntry(bidder, bid, "alert");
        bidHistory.appendBid(bidEntry);

        //First assure server goes to the next wait state to be able to handle client signals.
        zBridgeServerIface_raise_continue(&handle);
        serverRunCycle();

        //Then inform other players about bid (they might send signals back to server).
        if (bidder != WEST_SEAT) actors[WEST_SEAT]->bidDone(bidder, bid);
        if (bidder != NORTH_SEAT) actors[NORTH_SEAT]->bidDone(bidder, bid);
        if (bidder != EAST_SEAT) actors[EAST_SEAT]->bidDone(bidder, bid);
        if (bidder != SOUTH_SEAT)  actors[SOUTH_SEAT]->bidDone(bidder, bid);
    }

    else if (zBridgeServerIface_israised_bidInfo(&handle))
    {
        if (showUser)
        {
            sShowBidDialog(false);
            sShowBid((Seat)zBridgeServerIface_get_bidder(&handle), BID_BLANK);
            sClearYourTurnOnTable();
            sShowDummyOnTable((Seat)((zBridgeServerIface_get_declarer(&handle) + 2) & 3));
            sShowPlay();
        }

        //Set bid, double and declarer (last bidder).
        playHistory.setBidInfo((Bids)zBridgeServerIface_get_lastBid(&handle),
                               (Bids)zBridgeServerIface_get_bidDouble(&handle),
                               (Seat)zBridgeServerIface_get_bidInfo_value(&handle));
    }

    else if (zBridgeServerIface_israised_sendPlayerPlays(&handle))
    {
        Seat player = (Seat)zBridgeServerIface_get_sendPlayerPlays_value(&handle);
        int card = zBridgeServerIface_get_cardVal(&handle);
        int noTrick = zBridgeServerIface_get_noTrick(&handle);

        if (showUser)
            sShowPlayerPlays(player, card);

        playHistory.setPlay(player, noTrick, card);

        //First assure server goes to the next wait state to be able to handle client signals.
        zBridgeServerIface_raise_continue(&handle);
        serverRunCycle();

        //Then inform other players about play (they might send signals back to server).
        //Note declarer plays dummy's cards.
        Seat seat = (player == (Seat)zBridgeServerIface_get_dummy(&handle)) ?
                    (Seat)zBridgeServerIface_get_declarer(&handle) : player;
        if (seat != WEST_SEAT) actors[WEST_SEAT]->playerPlays(player, card);
        if (seat != NORTH_SEAT) actors[NORTH_SEAT]->playerPlays(player, card);
        if (seat != EAST_SEAT) actors[EAST_SEAT]->playerPlays(player, card);
        if (seat != SOUTH_SEAT) actors[SOUTH_SEAT]->playerPlays(player, card);
    }

    else if (zBridgeServerIface_israised_dummyCards(&handle))
    {  
        Seat dummy = (Seat)zBridgeServerIface_get_dummy(&handle);
        Seat seat = (Seat)zBridgeServerIface_get_dummyCards_value(&handle);

        actors[seat]->dummyCards(currentCards[dummy]);
    }

    else if (zBridgeServerIface_israised_getLeader(&handle))
    {
        if (showUser)
        {
            playView->clearCardsOnTable();
            playView->clearYourTurnOnTable();
        }

        zBridgeServerIface_raise_newLeader(&handle, playHistory.getNextLeader());

        if (showUser)
            sShowTricks(playHistory.getEWTricks(), playHistory.getNSTricks());

        serverRunCycle();
    }

    else if (zBridgeServerIface_israised_undoBid(&handle))
    {
        int val = zBridgeServerIface_get_undoBid_value(&handle);
        actors[WEST_SEAT]->undoBid(val);
        actors[NORTH_SEAT]->undoBid( val);
        actors[EAST_SEAT]->undoBid(val);
        actors[SOUTH_SEAT]->undoBid(val);
    }

    else if (zBridgeServerIface_israised_undoTrick(&handle))
    {
        int val = zBridgeServerIface_get_undoTrick_value(&handle);
        actors[WEST_SEAT]->undoTrick(val);
        actors[NORTH_SEAT]->undoTrick(val);
        actors[EAST_SEAT]->undoTrick(val);
        actors[SOUTH_SEAT]->undoTrick(val);
    }

    else if (zBridgeServerIface_israised_endOfSession(&handle))
    {
        actors[WEST_SEAT]->endOfSession();
        actors[NORTH_SEAT]->endOfSession();
        actors[EAST_SEAT]->endOfSession();
        actors[SOUTH_SEAT]->endOfSession();

        cleanTableManager();
    }

    //Can come together with bidInfo and must be processed after bidInfo.
    if (zBridgeServerIface_israised_playerToLead(&handle))
    {
        //Wait for one second to assure that the client to lead waits for this message when it is sent.
        //The lead client on its side must assure that it has completed its end of trick work within
        //this second.
        //This is a requirement of the protocol.
        QTimer::singleShot(1000, this, SLOT(playerToLead()));
    }

    //Can (in principle - not in practice) come together with bidInfo and must be processed after bidInfo.
    if (zBridgeServerIface_israised_dummyToLead(&handle))
    {
        //Wait for one second to assure that the client to lead waits for this message when it is sent.
        //The lead client on its side must assure that it has completed its end of trick work within
        //this second.
        //This is a requirement of the protocol.
        QTimer::singleShot(1000, this, SLOT(dummyToLead()));
    }
}

void CTblMngrServer::setShowUser(bool showAll)
{
    showUser = false;
    actors[WEST_SEAT]->setShowUser(false);
    actors[NORTH_SEAT]->setShowUser(false);
    actors[EAST_SEAT]->setShowUser(false);
    actors[SOUTH_SEAT]->setShowUser(false);

    if (actors[WEST_SEAT]->getActorType() == MANUAL_ACTOR)
        actors[WEST_SEAT]->setShowUser(true);
    else if (actors[NORTH_SEAT]->getActorType() == MANUAL_ACTOR)
        actors[NORTH_SEAT]->setShowUser(true);
    else if (actors[EAST_SEAT]->getActorType() == MANUAL_ACTOR)
        actors[EAST_SEAT]->setShowUser(true);
    else if (actors[SOUTH_SEAT]->getActorType() == MANUAL_ACTOR)
        actors[SOUTH_SEAT]->setShowUser(true);
    else if (showAll)
        showUser = true;
}

void CTblMngrServer::serverRunCycle()
{
    //Handle raised in flags.
    zBridgeServer_runCycle(&handle);
    //Handle out flags raised by the run cycle.
    serverActions();
    //Assure all out flags gets cleared (they have been handled now and can disturb recursive calls).
    zBridgeServer_runCycle(&handle);
}

//Slots for actors.
//-----------------------------------------------------------------------------
void CTblMngrServer::sConnect(QString name, Seat seat, int protocol)
{
    QByteArray ba = name.toLatin1();
    zBridgeServerIface_set_name(&handle, ba.data());
    zBridgeServerIface_raise_connect(&handle, seat);
    serverRunCycle();
}

void CTblMngrServer::sRTNames(Seat seat)
{
    zBridgeServerIface_raise_rTNames(&handle, seat);
    serverRunCycle();
}

void CTblMngrServer::sRSBoard(Seat seat)
{
    zBridgeServerIface_raise_rSBoard(&handle, seat);
    serverRunCycle();
}

void CTblMngrServer::sRDealInfo(Seat seat)
{
    zBridgeServerIface_raise_rDealInfo(&handle, seat);
    serverRunCycle();
}

void CTblMngrServer::sRCards(Seat seat)
{
    zBridgeServerIface_raise_rCards(&handle, seat);
    serverRunCycle();
}

void CTblMngrServer::sRBid(Seat player, Seat bidder)
{
    zBridgeServerIface_raise_rBid(&handle, player);
    serverRunCycle();
}

void CTblMngrServer::sBid(Seat bidder, Bids bid)
{
    zBridgeServerIface_set_bidVal(&handle, bid);
    zBridgeServerIface_raise_bid(&handle, bidder);
    serverRunCycle();
}

void CTblMngrServer::sPlayerPlays(Seat player, int card)
{
    zBridgeServerIface_set_cardVal(&handle, card);
    zBridgeServerIface_raise_playerPlays(&handle, player);
    serverRunCycle();
}

void CTblMngrServer::sReadyForPlayer(Seat seat, Seat player, int trick)
{
    zBridgeServerIface_raise_readyForPlayer(&handle, seat);
    serverRunCycle();
}

void CTblMngrServer::sReadyForDummy(Seat seat, int trick)
{
    zBridgeServerIface_raise_readyForDummy(&handle, seat);
    serverRunCycle();
}

void CTblMngrServer::sReadyForDummyCards(Seat seat)
{
    zBridgeServerIface_raise_readyForDummyCards(&handle, seat);
    serverRunCycle();
}

void CTblMngrServer::sShowAuction()
{
    playView->setParams(SEATS[doc->getSeatOptions().seat], doc->getDisplayOptions().cardBack);

    QString str;
    str.setNum(currentBoardNo);
    playView->setInfoAuction(str, currentVulnerable, (Seat)zBridgeServerIface_get_dealer(&handle));
    playView->showInfoAuction(true);
}

void CTblMngrServer::sShowPlay()
{
    //Show play info window with dealer, declarer and contract.
    playView->showInfoAuction(false);

    QString str;
    str.setNum(currentBoardNo);
    playView->setInfoPlay(str, currentVulnerable, (Seat)zBridgeServerIface_get_dealer(&handle),
                          (Seat)zBridgeServerIface_get_declarer(&handle),
                          (Bids)zBridgeServerIface_get_lastBid(&handle),
                          (Bids)zBridgeServerIface_get_bidDouble(&handle));

    playView->showNSTricks(0);
    playView->showEWTricks(0);

    playView->showInfoPlay(true);

    //Rearrange display of cards for the contracts trumpsuit.
    playView->setTrumpSuit(BID_SUIT((Bids)zBridgeServerIface_get_lastBid(&handle)));
}

void CTblMngrServer::sEnableContinue()
{
    if (!waiting)
    {
        waiting = true;
        //Waiting time must be less than one second. This is the maximum time the server waits
        //for the clients to be ready.
        //This is a requirement of the protocol.
        continueButton->start(700);

        //Also show and enable continue button.
        playView->enableContinueOnTable();
    }
}

void CTblMngrServer::sDisableContinue()
{
    if (waiting)
    {
        waiting = false;
        playView->disableContinueOnTable();
    }
}

void CTblMngrServer::sContinuePlay()
{
    continueButton->stop();

    if (actors[WEST_SEAT]->getActorType() == MANUAL_ACTOR) actors[WEST_SEAT]->continuePlay();
    if (actors[NORTH_SEAT]->getActorType() == MANUAL_ACTOR) actors[NORTH_SEAT]->continuePlay();
    if (actors[EAST_SEAT]->getActorType() == MANUAL_ACTOR) actors[EAST_SEAT]->continuePlay();
    if (actors[SOUTH_SEAT]->getActorType() == MANUAL_ACTOR) actors[SOUTH_SEAT]->continuePlay();
}

//Slots for play view.
//-----------------------------------------------------------------------------
void CTblMngrServer::buttonClicked(int button)
{
    if (button == BUTTON_CONTINUE)
        sContinuePlay();
}

void CTblMngrServer::bidValue(Bids bid)
{
    actors[zBridgeServerIface_get_bidder(&handle)]->bidValue(bid);
}

void CTblMngrServer::playValue(int card)
{

    Seat declarer = (Seat)zBridgeServerIface_get_declarer(&handle);
    Seat dummy = (Seat)zBridgeServerIface_get_dummy(&handle);
    Seat player = (Seat)zBridgeServerIface_get_player(&handle);

    Seat seat = (player == dummy) ? (declarer) : (player);
    actors[seat]->playValue(card);
}

void CTblMngrServer::bidBackup()
{
}

void CTblMngrServer::bidHint()
{
}

void CTblMngrServer::bidRestart()
{
}

void CTblMngrServer::bidClose()
{
}

void CTblMngrServer::handClicked(Seat seat)
{
}

void CTblMngrServer::startOfBoard()
{
    playView->resetView();

    //EAK WAIT At this point we must intiate a new deal.
    //We could be here because of first play in a session, pass out, play finished or user
    //wanted a new deal.
    giveNewDeal();

    //The next continue is only needed for the first play in a session, but does no harm otherwise.
    zBridgeServerIface_raise_continue(&handle);
    serverRunCycle();

    actors[WEST_SEAT]->startOfBoard();
    actors[NORTH_SEAT]->startOfBoard();
    actors[EAST_SEAT]->startOfBoard();
    actors[SOUTH_SEAT]->startOfBoard();
}

//Send "player to lead" to the relevant client. This routine is called after a one second delay.
//It is a requirement of the protocol to have this one second delay.
void CTblMngrServer::playerToLead()
{
    Seat player = (Seat)zBridgeServerIface_get_playerToLead_value(&handle);
    actors[player]->playerToLead(player);
}

//Send "dummy to lead" to the relevant client. This routine is called after a one second delay.
//It is a requirement of the protocol to have this one second delay.
void CTblMngrServer::dummyToLead()
{
    Seat declarer = (Seat)zBridgeServerIface_get_dummyToLead_value(&handle);
    actors[declarer]->dummyToLead();
}
