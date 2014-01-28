/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTblMngrServer.cpp
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Table manager server.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the table manager server class.
 */

#include <QApplication>

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


/**
 * @brief Constructor for table manager server.
 * @param doc Pointer to model data.
 * @param playView Pointer to play view.
 * @param parent Pointer to parent.
 *
 * The constructor can set up the table management server in two different modes:
 *
 *   - Standalone. In this case all actors are on the local computer.
 *   - Server. In this case some of the actors can be on remote computers.
 *
 * The constructor:
 *   - Initializes the table management server.
 *   - In server mode it sets up a tcp server for remote actors.
 *   - Enables/disables relevant main menu entries.
 */
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
    remoteActorServer = 0;
    if (doc->getSeatOptions().role == SERVER_ROLE)
    {
        remoteActorServer = new CRemoteActorServer(doc->getSeatOptions().protocol,
                                               QHostAddress(doc->getSeatOptions().hostServer),
                                               doc->getSeatOptions().portServer.toInt(), this);

        //Connect for disconnect of remote client.
        connect(remoteActorServer, &CRemoteActorServer::clientDisconnected, this, &CTblMngrServer::cleanTableManager);
    }

    //Enable/disable relevant menu actions.
    QApplication::postEvent(parent, new UPDATE_UI_ACTION_Event(UPDATE_UI_INITIAL));

    //Timer for supervision of continue button.
    continueButton = new QTimer(this);
    connect(continueButton, &QTimer::timeout, this, &CTblMngr::sContinuePlay);
    continueButton->setSingleShot(true);
    waiting = false;
}

CTblMngrServer::~CTblMngrServer()
{
    if (remoteActorServer != 0)
        delete remoteActorServer;

    cleanTableManager();
}

/**
 * @brief Run cycle for the Yakindu statechart.
 */
void CTblMngrServer::serverRunCycle()
{
    //Handle raised in flags.
    zBridgeServer_runCycle(&handle);
    //Handle out flags raised by the run cycle.
    serverActions();
    //Assure all out flags gets cleared (they have been handled now and can disturb recursive calls).
    zBridgeServer_runCycle(&handle);
}

/**
 * @brief Actions for the Yakindu statechart.
 *
 * Actions are identified by means of Yakindu out events. To follow the logic of this method one must
 * know the communication @ref protocol used in between server and actors. And also the Yakindu server
 * statechart must be known. Both can be found in the eclipse framework for the Yakindu statechart
 * models.
 */
void CTblMngrServer::serverActions()
{
    //React to server out events.
    if (zBridgeServerIface_israised_seated(&handle))
    {
        //One of the actors has connected and conveyed its team name.
        Seat seat = (Seat)zBridgeServerIface_get_seated_value(&handle);

        //The actor is seated.
        char * teamName = ((seat == WEST_SEAT) || (seat == EAST_SEAT)) ?
                    zBridgeServerIface_get_ewTeamName(&handle): zBridgeServerIface_get_nsTeamName(&handle);

        actors[seat]->seated(QString(teamName));
    }

    else if (zBridgeServerIface_israised_teamNames(&handle))
    {
        //All actors have connected and their team names are set.
        QString nsTeamName = QString(zBridgeServerIface_get_nsTeamName(&handle));
        QString ewTeamName = QString(zBridgeServerIface_get_ewTeamName(&handle));
        actors[WEST_SEAT]->teamNames(nsTeamName, ewTeamName);
        actors[NORTH_SEAT]->teamNames(nsTeamName, ewTeamName);
        actors[EAST_SEAT]->teamNames(nsTeamName, ewTeamName);
        actors[SOUTH_SEAT]->teamNames(nsTeamName, ewTeamName);
    }

    else if (zBridgeServerIface_israised_startOfBoard(&handle))
    {
        //All actors are ready for start of board.
        startOfBoard();
    }
    else if (zBridgeServerIface_israised_startOfBoardDelayed(&handle))
    {
        //For synchronization reasons start of board must be time delayed in some cases.
        QTimer::singleShot(1000, this, SLOT(startOfBoard()));
    }

    else if (zBridgeServerIface_israised_dealInfo(&handle))
    {
        //One of the actors aready for deal information.
        Seat seat = (Seat)zBridgeServerIface_get_dealInfo_value(&handle);

        actors[seat]->dealInfo(currentBoardNo, (Seat)zBridgeServerIface_get_dealer(&handle), currentVulnerable);
    }

    else if (zBridgeServerIface_israised_cards(&handle))
    {
        //All actors have received their deal info.
        if (showUser)
        {
            //The server must show relevant information to the user.
            sShowAuction();                 //Show auction widgets in play view.
            sShowCenter(currentVulnerable); //Show center widget in play view.
        }

        //Determine which cards to show in play view.
        bool showWest = showAll || (doc->getSeatOptions().westActor == MANUAL_ACTOR);
        bool showNorth = showAll || (doc->getSeatOptions().northActor == MANUAL_ACTOR);
        bool showEast = showAll || (doc->getSeatOptions().eastActor == MANUAL_ACTOR);
        bool showSouth = showAll || (doc->getSeatOptions().southActor == MANUAL_ACTOR);

        //Show the cards in play view.
        playView->setAndShowAllCards(true, showWest, currentCards[WEST_SEAT], true, showNorth, currentCards[NORTH_SEAT],
                            true, showEast, currentCards[EAST_SEAT], true, showSouth, currentCards[SOUTH_SEAT]);


        bidHistory.resetBidHistory();

        zBridgeServerIface_raise_continue(&handle);
        serverRunCycle();

        //Set cards in actors.
        actors[WEST_SEAT]->cards(currentCards);
        actors[NORTH_SEAT]->cards(currentCards);
        actors[EAST_SEAT]->cards(currentCards);
        actors[SOUTH_SEAT]->cards(currentCards);

        //Show bid dialog.
        if (showUser)
        {
            sShowBidDialog(true);
            sShowBid((Seat)zBridgeServerIface_get_dealer(&handle), BID_PLAYER);
        }
    }

    else if (zBridgeServerIface_israised_bidDone(&handle))
    {
        //A bid has been given
        Seat bidder = (Seat)zBridgeServerIface_get_bidDone_value(&handle);
        Bids bid = (Bids)zBridgeServerIface_get_bidVal(&handle);

        //Show bid in play view.
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
        //Bidding is finished.
        if (showUser)
        {
            //Show relevant information to the user in play view.
            sShowBidDialog(false);
            sShowBid((Seat)zBridgeServerIface_get_bidder(&handle), BID_BLANK);
            sClearYourTurnOnTable();
            sShowDummyOnTable((Seat)((zBridgeServerIface_get_declarer(&handle) + 2) & 3));
            sShowPlay();        //Show play widget in play view.
        }

        //Set bid, double and declarer (last bidder) in play history.
        playHistory.setBidInfo((Bids)zBridgeServerIface_get_lastBid(&handle),
                               (Bids)zBridgeServerIface_get_bidDouble(&handle),
                               (Seat)zBridgeServerIface_get_bidInfo_value(&handle));
    }

    else if (zBridgeServerIface_israised_sendPlayerPlays(&handle))
    {
        //All actors are ready to receive the next card played.
        Seat player = (Seat)zBridgeServerIface_get_sendPlayerPlays_value(&handle);
        int card = zBridgeServerIface_get_cardVal(&handle);
        int noTrick = zBridgeServerIface_get_noTrick(&handle);

        //Show card played in play view.
        if (showUser)
            sShowPlayerPlays(player, card);

        //Set card played in play history.
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
        //Actor is ready to receive dummy's cards.
        Seat dummy = (Seat)zBridgeServerIface_get_dummy(&handle);
        Seat seat = (Seat)zBridgeServerIface_get_dummyCards_value(&handle);

        actors[seat]->dummyCards(currentCards[dummy]);
    }

    else if (zBridgeServerIface_israised_getLeader(&handle))
    {
        //Get the next leader.
        if (showUser)
        {
            //Initialize play view for next trick.
            playView->clearCardsOnTable();
            playView->clearYourTurnOnTable();
        }

        zBridgeServerIface_raise_newLeader(&handle, playHistory.getNextLeader());

        //Show number of tricks in play view.
        if (showUser)
            sShowTricks(playHistory.getEWTricks(), playHistory.getNSTricks());

        serverRunCycle();
    }

    else if (zBridgeServerIface_israised_undoBid(&handle))
    {
        //Undo bid.
        int val = zBridgeServerIface_get_undoBid_value(&handle);
        actors[WEST_SEAT]->undoBid(val);
        actors[NORTH_SEAT]->undoBid( val);
        actors[EAST_SEAT]->undoBid(val);
        actors[SOUTH_SEAT]->undoBid(val);
    }

    else if (zBridgeServerIface_israised_undoTrick(&handle))
    {
        //Undo trick.
        int val = zBridgeServerIface_get_undoTrick_value(&handle);
        actors[WEST_SEAT]->undoTrick(val);
        actors[NORTH_SEAT]->undoTrick(val);
        actors[EAST_SEAT]->undoTrick(val);
        actors[SOUTH_SEAT]->undoTrick(val);
    }

    else if (zBridgeServerIface_israised_endOfSession(&handle))
    {
        //End of session.
        actors[WEST_SEAT]->endOfSession();
        actors[NORTH_SEAT]->endOfSession();
        actors[EAST_SEAT]->endOfSession();
        actors[SOUTH_SEAT]->endOfSession();

        cleanTableManager();
    }

    //Can come together with bidInfo and must be processed after bidInfo.
    if (zBridgeServerIface_israised_playerToLead(&handle))
    {
        //Player to lead next trick.
        //Wait for one second to assure that the client to lead waits for this message when it is sent.
        //The lead client on its side must assure that it has completed its end of trick work within
        //this second.
        //This is a requirement of the protocol.
        QTimer::singleShot(1000, this, SLOT(playerToLead()));
    }

    //Can (in principle - not in practice) come together with bidInfo and must be processed after bidInfo.
    if (zBridgeServerIface_israised_dummyToLead(&handle))
    {
        //Dummy to lead next trick.
        //Wait for one second to assure that the client to lead waits for this message when it is sent.
        //The lead client on its side must assure that it has completed its end of trick work within
        //this second.
        //This is a requirement of the protocol.
        QTimer::singleShot(1000, this, SLOT(dummyToLead()));
    }
}

/**
 * @brief Clean table manager so that it can start a new session.
 */
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

/**
 * @brief Give a new deal in accordance with what the user has selected.
 */
void CTblMngrServer::giveNewDeal()
{
    int i, j, inx;
    int cardDeck[52];
    Seat currentDealer;

    //Info about board.
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

    //Give cards.
    for (i = 0; i < 4; i++)
        for (j = 0; j < 13; j++)
            currentCards[i][j] = cardDeck[i * 13 + j];
}

//Methods activated by user through main frame menus.
//----------------------------------------------------------------------------
/**
 * @brief Start a new session (from main menu).
 *
 * This method is activated from the main menu. It starts a new session.
 *
 *   - Prepare for a new session.
 *   - Determine protocol (Advanced or Basic).
 *   - Enable/disable relevant main menu actions.
 *   - Set up actors (local or remote etc.).
 *   - Start all actors with a new session.
 */
void CTblMngrServer::newSession()
{
    CActor *actor;

    cleanTableManager();

    boardNo = 0;

    protocol = (remoteActorServer != 0) ? doc->getSeatOptions().protocol : ADVANCED_PROTOCOL;

    //Enable/disable relevant menu actions.
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_SERVER , protocol == ADVANCED_PROTOCOL));

    QString ewTeamName = "ZBridge";
    QString nsTeamName = "ZBridge";

    //Set up actors.
    if (doc->getSeatOptions().westActor == MANUAL_ACTOR)
        actor = new CActorLocal(true, ewTeamName, WEST_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if ((remoteActorServer != 0) && remoteActorServer->isConnected(WEST_SEAT))
        actor = new CActorRemote(WEST_SEAT, remoteActorServer->getFrontend(WEST_SEAT), this);
    else
        actor = new CActorLocal(false, ewTeamName, WEST_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    actors[WEST_SEAT] = actor;

    if (doc->getSeatOptions().northActor == MANUAL_ACTOR)
        actor = new CActorLocal(true, nsTeamName, NORTH_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if ((remoteActorServer != 0) && remoteActorServer->isConnected(NORTH_SEAT))
        actor = new CActorRemote(NORTH_SEAT, remoteActorServer->getFrontend(NORTH_SEAT), this);
    else
        actor = new CActorLocal(false, nsTeamName, NORTH_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    actors[NORTH_SEAT] = actor;

    if (doc->getSeatOptions().eastActor == MANUAL_ACTOR)
        actor = new CActorLocal(true, ewTeamName, EAST_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if ((remoteActorServer != 0) && remoteActorServer->isConnected(EAST_SEAT))
        actor = new CActorRemote(EAST_SEAT, remoteActorServer->getFrontend(EAST_SEAT), this);
    else
        actor = new CActorLocal(false, ewTeamName, EAST_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    actors[EAST_SEAT] = actor;

    if (doc->getSeatOptions().southActor == MANUAL_ACTOR)
        actor = new CActorLocal(true, nsTeamName, SOUTH_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if ((remoteActorServer != 0) && remoteActorServer->isConnected(SOUTH_SEAT))
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

/**
 * @brief Give a new deal (from main menu).
 */
void CTblMngrServer::newDeal()
{
    zBridgeServerIface_raise_newDeal(&handle);
    serverRunCycle();
}

/**
 * @brief Rebid the current deal (from main menu).
 */
void CTblMngrServer::reBid()
{
    zBridgeServerIface_raise_undo(&handle, -1);
    serverRunCycle();
}

/**
 * @brief Replay the current deal (from main menu).
 */
void CTblMngrServer::rePlay()
{
    if (zBridgeServer_isActive(&handle, ZBridgeServer_entry__Playing))
    {
        zBridgeServerIface_raise_undo(&handle, 0);
        serverRunCycle();
    }
}

/**
 * @brief Undo bid or play (from main menu).
 */
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

/**
 * @brief Determine who controls the play view.
 * @param showAll If true all cards should be shown in the play view.
 */
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

//Slots for play view.
//-----------------------------------------------------------------------------
/**
 * @brief A button was clicked in the play view.
 * @param button Identifies which button was clicked.
 */
void CTblMngrServer::buttonClicked(int button)
{
    //The Continue button was clicked (from Center widget in play view).
    if (button == BUTTON_CONTINUE)
        sContinuePlay();
}

/**
 * @brief A bid was given in the bid dialog.
 * @param bid The bid given.
 */
void CTblMngrServer::bidValue(Bids bid)
{
    actors[zBridgeServerIface_get_bidder(&handle)]->bidValue(bid);
}

/**
 * @brief A card was played in the play view.
 * @param card The card played.
 */
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

//Delayed methods for Yakindu server actions.
//--------------------------------------------------------------
/**
 * @brief Start of board was raised.
 *
 * All actors are now ready for start of board.
 */
void CTblMngrServer::startOfBoard()
{
    //Prepare play view.
    playView->resetView();

    //EAK WAIT At this point we must intiate a new deal.
    //We could be here because of first play in a session, pass out, play finished or user
    //wanted a new deal.
    giveNewDeal();

    //The next continue is only needed for the first play in a session, but does no harm otherwise.
    zBridgeServerIface_raise_continue(&handle);
    serverRunCycle();

    //Start all actors.
    actors[WEST_SEAT]->startOfBoard();
    actors[NORTH_SEAT]->startOfBoard();
    actors[EAST_SEAT]->startOfBoard();
    actors[SOUTH_SEAT]->startOfBoard();
}

/**
 * @brief Player to lead next trick.
 *
 * Send "player to lead" to the relevant client. This routine is called after a one second delay.
 * It is a requirement of the protocol to have this one second delay.
 *
 */
void CTblMngrServer::playerToLead()
{
    Seat player = (Seat)zBridgeServerIface_get_playerToLead_value(&handle);
    actors[player]->playerToLead(player);
}

/**
 * @brief Dummy to lead the next trick.
 *
 * Send "dummy to lead" to the relevant client. This routine is called after a one second delay.
 * It is a requirement of the protocol to have this one second delay.
 */
void CTblMngrServer::dummyToLead()
{
    Seat declarer = (Seat)zBridgeServerIface_get_dummyToLead_value(&handle);
    actors[declarer]->dummyToLead();
}

//Slots for actors.
//-----------------------------------------------------------------------------
/**
 * @brief Actor requests connect (actor slot).
 * @param teamName Team name for actor.
 * @param seat Seat for actor.
 * @param protocol Protocol to use.
 *
 * The name is set in the statechart and connect is signalled to the statechart.
 */
void CTblMngrServer::sConnect(QString teamName, Seat seat, int protocol)
{
    this->teamNames[seat] = teamName.toLatin1();
    zBridgeServerIface_set_name(&handle, this->teamNames[seat].data());
    zBridgeServerIface_raise_connect(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Actor is ready for team names (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrServer::sRTNames(Seat seat)
{
    zBridgeServerIface_raise_rTNames(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Actor is ready for start of board (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrServer::sRSBoard(Seat seat)
{
    zBridgeServerIface_raise_rSBoard(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Actor is ready for deal info (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrServer::sRDealInfo(Seat seat)
{
    zBridgeServerIface_raise_rDealInfo(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Actor is ready for cards (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrServer::sRCards(Seat seat)
{
    zBridgeServerIface_raise_rCards(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Player is ready for bid (actor slot).
 * @param player The players seat.
 * @param bidder The bidders seat.
 */
void CTblMngrServer::sRBid(Seat player, Seat bidder)
{
    zBridgeServerIface_raise_rBid(&handle, player);
    serverRunCycle();
}

/**
 * @brief Bidders bid (actor slot).
 * @param bidder The bidders seat.
 * @param bid The bid.
 */
void CTblMngrServer::sBid(Seat bidder, Bids bid)
{
    zBridgeServerIface_set_bidVal(&handle, bid);
    zBridgeServerIface_raise_bid(&handle, bidder);
    serverRunCycle();
}

/**
 * @brief Players play (actor slot).
 * @param player The players play.
 * @param card The card played.
 */
void CTblMngrServer::sPlayerPlays(Seat player, int card)
{
    zBridgeServerIface_set_cardVal(&handle, card);
    zBridgeServerIface_raise_playerPlays(&handle, player);
    serverRunCycle();
}

/**
 * @brief Actor is ready for player (actor slot).
 * @param seat The actors seat.
 * @param player The players seat.
 * @param trick The trick.
 */
void CTblMngrServer::sReadyForPlayer(Seat seat, Seat player, int trick)
{
    zBridgeServerIface_raise_readyForPlayer(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Actor is ready for dummy (actor slot).
 * @param seat Actors seat.
 * @param trick The trick.
 */
void CTblMngrServer::sReadyForDummy(Seat seat, int trick)
{
    zBridgeServerIface_raise_readyForDummy(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Actor is ready for dummys cards (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrServer::sReadyForDummyCards(Seat seat)
{
    zBridgeServerIface_raise_readyForDummyCards(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Show auction info widgets in play view (actor slot).
 */
void CTblMngrServer::sShowAuction()
{
    playView->setParams(doc->getSeatOptions().seat, doc->getDisplayOptions().cardBack);

    QString str;
    str.setNum(currentBoardNo);
    playView->setInfoAuction(str, currentVulnerable, (Seat)zBridgeServerIface_get_dealer(&handle));
    playView->showInfoAuction(true);
}

/**
 * @brief Show play info widgets in play view (actor slot).
 */
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

/**
 * @brief Enable Continue button (actor slot).
 *
 * It is assured that the waiting time before the user presses the continue button is less than
 * one second. This is the maximun time the server waits for the clients to be ready. This is a
 * requirement of the protocol.
 */
void CTblMngrServer::sEnableContinue()
{
    if (!waiting)
    {
        waiting = true;
        //Waiting time must be less than one second.
        continueButton->start(700);

        //Also show and enable continue button.
        playView->enableContinueOnTable();
    }
}

/**
 * @brief Disable Continue button (actor slot).
 */
void CTblMngrServer::sDisableContinue()
{
    if (waiting)
    {
        waiting = false;
        playView->disableContinueOnTable();
    }
}

/**
 * @brief Continue play with next trick (actor slot).
 */
void CTblMngrServer::sContinuePlay()
{
    continueButton->stop();

    if (actors[WEST_SEAT]->getActorType() == MANUAL_ACTOR) actors[WEST_SEAT]->continuePlay();
    if (actors[NORTH_SEAT]->getActorType() == MANUAL_ACTOR) actors[NORTH_SEAT]->continuePlay();
    if (actors[EAST_SEAT]->getActorType() == MANUAL_ACTOR) actors[EAST_SEAT]->continuePlay();
    if (actors[SOUTH_SEAT]->getActorType() == MANUAL_ACTOR) actors[SOUTH_SEAT]->continuePlay();
}
