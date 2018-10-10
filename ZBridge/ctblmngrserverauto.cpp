/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CTblMngrServerAuto.cpp
  Developers: eak

  Revision History:
  12-jun-2015 eak: Original

  Abstract: Table manager server for auto play.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the table manager server class for auto play.
 */

#include <cassert>
#include <QApplication>

#include "../src-gen/sc_types.h"
#include "czbridgedoc.h"
#include "cgamesdoc.h"
#include "cplayview.h"
#include "cactorlocalauto.h"
#include "cactorremoteauto.h"
#include "cbidengine.h"
#include "cplayengine.h"
#include "cremoteactorserver.h"
#include "cbidandplayengines.h"
#include "CTblMngrServerAuto.h"


/**
 * @brief Constructor for table manager server.
 * @param doc Pointer to model data.
 * @param games Pointer to game data.
 * @param hostAddress Host address.
 * @param parent Pointer to parent.
 *
 * The constructor can set up the table management server in two different modes:
 *
 *   - Standalone. In this case all actors are on the local computer (hostAddress is null).
 *   - Server. In this case some of the actors can be on remote computers.
 *
 * The constructor:
 *   - Initializes the table management server.
 *   - In server mode it sets up a tcp server for remote actors.
 */
CTblMngrServerAuto::CTblMngrServerAuto(CZBridgeDoc *doc, CGamesDoc *games,
                     CBidAndPlayEngines *bidAndPlayEngines, QHostAddress hostAddress,
                     QObject *parent) throw(NetProtocolException) :
    CTblMngrBase(parent)
{
    this->doc = doc;
    this->games = games;

    synchronizing = false;

    boardNo = -1;

    this->bidAndPlayEngines = bidAndPlayEngines;

    actors[WEST_SEAT] = 0;
    actors[NORTH_SEAT] = 0;
    actors[EAST_SEAT] = 0;
    actors[SOUTH_SEAT] = 0;

    //Start tcp server for remote clients.
    remoteActorServer = 0;
    if (!hostAddress.isNull())
    {
        remoteActorServer = new CRemoteActorServer(doc->getSeatOptions().protocol,
                                                   hostAddress, doc->getSeatOptions().portServer.toInt() + AUTOPORTOFFSET, this);

        //Connect for disconnect of remote client(s).
        connect(remoteActorServer, &CRemoteActorServer::clientDisconnected, this, &CTblMngrServerAuto::cleanTableManager);

        //Connect for info, warning and error messages for server/client connection(s).
        connect(remoteActorServer, &CRemoteActorServer::connectInfo, this, &CTblMngrServerAuto::connectInfo);
        connect(remoteActorServer, &CRemoteActorServer::connectWarning, this, &CTblMngrServerAuto::connectWarning);
        connect(remoteActorServer, &CRemoteActorServer::connectError, this, &CTblMngrServerAuto::connectError);
    }

    waiting = false;
}

CTblMngrServerAuto::~CTblMngrServerAuto()
{
    if (remoteActorServer != 0)
        delete remoteActorServer;

    cleanTableManager();
}

/**
 * @brief Run cycle for the Yakindu statechart.
 */
void CTblMngrServerAuto::serverRunCycle()
{
    //Handle raised in flags.
    zBridgeServer_runCycle(&handle);
    //Handle out flags raised by the run cycle.
    serverActions();
    //Assure all out flags gets cleared (they have been handled now and can disturb recursive calls).
    zBridgeServer_runCycle(&handle);
}

void CTblMngrServerAuto::serverSyncRunCycle()
{
    //Handle raised in flags.
    zBridgeServerSync_runCycle(&syncHandle);
    //Handle out flags raised by the run cycle.
    serverSyncActions();
}

/**
 * @brief Actions for the Yakindu statechart.
 *
 * Actions are identified by means of Yakindu out events. To follow the logic of this method one must
 * know the communication @ref protocol used in between server and actors. And also the Yakindu server
 * statechart must be known. Both can be found in the eclipse framework for the Yakindu statechart
 * models.
 */
void CTblMngrServerAuto::serverActions()
{
    //React to server out events.
    if (zBridgeServerIface_israised_seated(&handle))
    {
        //One of the actors has connected and conveyed its team name.
        Seat seat = (Seat)zBridgeServerIface_get_seated_value(&handle);

        //The actor is seated.
        actors[seat]->seated(teamNames[seat]);
    }

    else if (zBridgeServerIface_israised_teamNames(&handle))
    {
        //All actors have connected and their team names are set.
        QString nsTeamName = teamNames[NORTH_SEAT];
        QString ewTeamName = teamNames[EAST_SEAT];
        nsTeamName += ":" + teamNames[SOUTH_SEAT];
        ewTeamName += ":" + teamNames[WEST_SEAT];
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
        int delay = 10;

        //For synchronization reasons start of board must be time delayed in some cases.
        QTimer::singleShot(delay, this, SLOT(startOfBoard()));
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
        bidHistory.resetBidHistory();
        playHistory.resetPlayHistory();

        zBridgeServerIface_raise_continue(&handle);
        serverRunCycle();

        //Set cards in actors.
        actors[WEST_SEAT]->cards(currentCards);
        actors[NORTH_SEAT]->cards(currentCards);
        actors[EAST_SEAT]->cards(currentCards);
        actors[SOUTH_SEAT]->cards(currentCards);
    }

    else if (zBridgeServerIface_israised_bidDone(&handle))
    {
        //A bid has been given
        Seat bidder = (Seat)zBridgeServerIface_get_bidDone_value(&handle);
        Bids bid = (Bids)zBridgeServerIface_get_bidVal(&handle);

        //Set bid in bid history.
        CBid bidEntry(bidder, bid, "");
        bidHistory.appendBid(bidEntry);

        //First assure server goes to the next wait state to be able to handle client signals.
        zBridgeServerIface_raise_continue(&handle);
        serverRunCycle();

        //Then inform players about bid (they might send signals back to server).
        actors[WEST_SEAT]->bidDone(bidder, bid);
        actors[NORTH_SEAT]->bidDone(bidder, bid);
        actors[EAST_SEAT]->bidDone(bidder, bid);
        actors[SOUTH_SEAT]->bidDone(bidder, bid);
    }

    else if (zBridgeServerIface_israised_bidInfo(&handle))
    {
        //Bidding is finished.
        //Set bid, double and open leader in play history.
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

        //Set card played in play history.
        playHistory.setPlay(player, noTrick, card);

        //First assure server goes to the next wait state to be able to handle client signals.
        zBridgeServerIface_raise_continue(&handle);
        serverRunCycle();

        //Then inform other players about play (they might send signals back to server).
        //Note declarer plays dummy's cards.
        actors[WEST_SEAT]->playerPlays(player, card);
        actors[NORTH_SEAT]->playerPlays(player, card);
        actors[EAST_SEAT]->playerPlays(player, card);
        actors[SOUTH_SEAT]->playerPlays(player, card);
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
        zBridgeServerIface_raise_newLeader(&handle, playHistory.getNextLeader());

        serverRunCycle();
    }

    else if (zBridgeServerIface_israised_undoPlay(&handle) || zBridgeServerIface_israised_undoBid(&handle))
    {
        //Undo bid always follows undo play - not used with auto play.
        int val = zBridgeServerIface_get_undoBid_value(&handle);
        actors[WEST_SEAT]->undoBid(val == REBID);
        actors[NORTH_SEAT]->undoBid(val == REBID);
        actors[EAST_SEAT]->undoBid(val == REBID);
        actors[SOUTH_SEAT]->undoBid(val == REBID);
    }

    else if (zBridgeServerIface_israised_undoTrick(&handle))
    {
        //Undo trick - not used with auto play.
        int val = zBridgeServerIface_get_undoTrick_value(&handle);
        actors[WEST_SEAT]->undoTrick(val == REPLAY);
        actors[NORTH_SEAT]->undoTrick(val == REPLAY);
        actors[EAST_SEAT]->undoTrick(val == REPLAY);
        actors[SOUTH_SEAT]->undoTrick(val == REPLAY);
    }
    else if (zBridgeServerIface_israised_newDealClients(&handle))
    {
        //New deal for all actors (uses startOfBoard for this).
        actors[WEST_SEAT]->startOfBoard();
        actors[NORTH_SEAT]->startOfBoard();
        actors[EAST_SEAT]->startOfBoard();
        actors[SOUTH_SEAT]->startOfBoard();
    }

    //Can come together with bidInfo and must be processed after bidInfo.
    if (zBridgeServerIface_israised_playerToLead(&handle))
    {
        int delay = 10;

        //Player to lead next trick.
        //Just to make sure the action is not called untill the run cycle of
        //the thread.
        QTimer::singleShot(delay, this, SLOT(playerToLead()));
    }

    //Can come after newDealClients.
    if (zBridgeServerIface_israised_synchronize(&handle))
    {
        //Synchronization of server and clients.
        zBridgeServerSync_init(&syncHandle);
        synchronizing = true;
        zBridgeServerSync_enter(&syncHandle);
        zBridgeServerSyncIface_raise_continue(&syncHandle);
        serverSyncRunCycle();
    }

    //Can (in principle - not in practice) come together with bidInfo and must be processed after bidInfo.
    if (zBridgeServerIface_israised_dummyToLead(&handle))
    {
        int delay = 10;

        //Dummy to lead next trick.
        //Just to make sure the action is not called until the run cycle of
        //the thread.
        QTimer::singleShot(delay, this, SLOT(dummyToLead()));
    }
}

/**
 * @brief Synchronization between server and clients.
 */
void CTblMngrServerAuto::serverSyncActions()
{
    bool israised_sendConfirmSync = zBridgeServerSyncIface_israised_sendConfirmSync (&syncHandle);

    //React to sync server out events.
    if (zBridgeServerSyncIface_israised_sendAttemptSyncAll(&syncHandle))
    {
        for (int i = 0; i < 4; i++)
            actors[i]->attemptSyncFromServerToClient();
    }
    else if (zBridgeServerSyncIface_israised_sendAttemptSync (&syncHandle))
    {
        Seat seat = (Seat)zBridgeServerSyncIface_get_sendAttemptSync_value(&syncHandle);
        actors[seat]->attemptSyncFromServerToClient();
    }

    else if (zBridgeServerSyncIface_israised_sendAllSync(&syncHandle))
    {
        zBridgeServerSyncIface_raise_continue(&syncHandle);
        serverSyncRunCycle();
        synchronizing = false;

        zBridgeServerIface_raise_allSync(&handle);
        serverRunCycle();

        for (int i = 0; i < 4; i++)
            actors[i]->allSyncFromServerToClient();
    }

    //Comes together with and after sendAttemptSync (gets cleared in some cases by sendAttemptSync).
    if (israised_sendConfirmSync)
    {
        //Continue.
        zBridgeServerSyncIface_raise_continue(&syncHandle);
        serverSyncRunCycle();

        int syncState = zBridgeServerIface_get_syncState(&handle);
        //Synchronization after bid and before play.
        if (syncState == SP)
        {
            Seat declarer = bidHistory.getDeclarer();
            Seat dummy = (Seat)((declarer + 2) % 4);
            Seat leader = (Seat)((declarer + 1) % 4);
            zBridgeServerIface_set_declarer(&handle, declarer);
            zBridgeServerIface_set_dummy(&handle, dummy);
            zBridgeServerIface_set_leader(&handle, leader);
            zBridgeServerIface_set_player(&handle, leader);
        }

        //Check if game info should be updated (can only happen with 4 remote clients).
        if (updateGameInfo && (syncState == SS))
            sUpdateGame();

        //Tell ordinary play that we are ready for the next game.
        if (syncState == SS)
            emit sigPlayStart();

        //Wait for ordinary play?
        if ((syncState == SS) && !playContinue)
            playWaiting = true;
        else
        {
            if (syncState == SS)
                playContinue = false;

            //Tell clients.
            for (int i = 0; i < 4; i++)
                actors[i]->confirmSyncFromServerToClient();
        }
    }
}

/**
 * @brief Clean table manager so that it can start a new session.
 */
void CTblMngrServerAuto::cleanTableManager()
{        
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

void CTblMngrServerAuto::connectInfo(QString text)
{
}

void CTblMngrServerAuto::connectWarning(QString text)
{
}

void CTblMngrServerAuto::connectError(QString text)
{
}

/**
 * @brief Give a new deal in accordance with what the user has selected.
 */
void CTblMngrServerAuto::giveNewDeal()
{
    Seat currentDealer;

    games->getCurrentDeal(&currentBoardNo, currentCards, &currentDealer, &currentVulnerable);
    zBridgeServerIface_set_dealer(&handle, currentDealer);
}

//Methods activated by user through main frame menus.
//----------------------------------------------------------------------------
/**
 * @brief Start a new session.
 *
 * This slot starts a new session.
 *
 *   - Prepare for a new session.
 *   - Set up actors (local or remote etc.).
 *   - Start all actors with a new session.
 */
void CTblMngrServerAuto::sNewSession()
{
    CActorBase *actor;

    cleanTableManager();

    waiting = false;
    boardNo = 0;

    playWaiting = playContinue = false;
    firstAutoSync = true;

    //Set up actors.
    Protocol protocol = doc->getSeatOptions().protocol;
    if ((remoteActorServer != 0) && remoteActorServer->isConnected(WEST_SEAT))
        actor = new CActorRemoteAuto(WEST_SEAT,  remoteActorServer->getFrontend(WEST_SEAT), this);
    else
        actor = new CActorLocalAuto(protocol, AUTO_SEAT_NAME_PREFIX + SEAT_NAMES[WEST_SEAT], WEST_SEAT,
                bidAndPlayEngines, this);
    actors[WEST_SEAT] = actor;

    if ((remoteActorServer != 0) && remoteActorServer->isConnected(NORTH_SEAT))
        actor = new CActorRemoteAuto(NORTH_SEAT, remoteActorServer->getFrontend(NORTH_SEAT), this);
    else
        actor = new CActorLocalAuto(protocol, AUTO_SEAT_NAME_PREFIX + SEAT_NAMES[NORTH_SEAT], NORTH_SEAT,
                bidAndPlayEngines, this);
    actors[NORTH_SEAT] = actor;

    if ((remoteActorServer != 0) && remoteActorServer->isConnected(EAST_SEAT))
        actor = new CActorRemoteAuto(EAST_SEAT, remoteActorServer->getFrontend(EAST_SEAT), this);
    else
        actor = new CActorLocalAuto(protocol, AUTO_SEAT_NAME_PREFIX + SEAT_NAMES[EAST_SEAT], EAST_SEAT,
                bidAndPlayEngines, this);
    actors[EAST_SEAT] = actor;

    if ((remoteActorServer != 0) && remoteActorServer->isConnected(SOUTH_SEAT))
        actor = new CActorRemoteAuto(SOUTH_SEAT, remoteActorServer->getFrontend(SOUTH_SEAT), this);
    else
        actor = new CActorLocalAuto(protocol, AUTO_SEAT_NAME_PREFIX + SEAT_NAMES[SOUTH_SEAT], SOUTH_SEAT,
                bidAndPlayEngines, this);
    actors[SOUTH_SEAT] = actor;

    setUpdateGameInfo();

    //Start server state table.
    zBridgeServer_init(&handle);
    zBridgeServerIface_set_noOfBoards(&handle, 512);
    zBridgeServer_enter(&handle);
    serverActions();

    if (!playContinue)
        playWaiting = true;
    else
    {
        playContinue = firstAutoSync = false;
        //Start actors.
        actors[WEST_SEAT]->startNewSession();
        actors[NORTH_SEAT]->startNewSession();
        actors[EAST_SEAT]->startNewSession();
        actors[SOUTH_SEAT]->startNewSession();
    }
}

/**
 * @brief Determine who updates game info.
 */
void CTblMngrServerAuto::setUpdateGameInfo()
{
    updateGameInfo = false;
    actors[WEST_SEAT]->setUpdateGameInfo(false);
    actors[NORTH_SEAT]->setUpdateGameInfo(false);
    actors[EAST_SEAT]->setUpdateGameInfo(false);
    actors[SOUTH_SEAT]->setUpdateGameInfo(false);

    if (actors[WEST_SEAT]->getActorType() != REMOTE_ACTOR)
        actors[WEST_SEAT]->setUpdateGameInfo(true);
    else if (actors[NORTH_SEAT]->getActorType() != REMOTE_ACTOR)
        actors[NORTH_SEAT]->setUpdateGameInfo(true);
    else if (actors[EAST_SEAT]->getActorType() != REMOTE_ACTOR)
        actors[EAST_SEAT]->setUpdateGameInfo(true);
    else if (actors[SOUTH_SEAT]->getActorType() != REMOTE_ACTOR)
        actors[SOUTH_SEAT]->setUpdateGameInfo(true);
    else
        updateGameInfo = true;
}

//Delayed methods for Yakindu server actions.
//--------------------------------------------------------------
/**
 * @brief Start of board was raised.
 *
 * All actors are now ready for start of board.
 */
void CTblMngrServerAuto::startOfBoard()
{
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
 * Send "player to lead" to the relevant client.
 *
 */
void CTblMngrServerAuto::playerToLead()
{
    Seat player = (Seat)zBridgeServerIface_get_playerToLead_value(&handle);
    actors[player]->playerToLead(player);
}

/**
 * @brief Dummy to lead the next trick.
 *
 * Send "dummy to lead" to the relevant client.
 */
void CTblMngrServerAuto::dummyToLead()
{
    Seat declarer = (Seat)zBridgeServerIface_get_dummyToLead_value(&handle);
    actors[declarer]->dummyToLead();
}

/**
 * @brief Synchronization with ordinary play.
 *
 * Is called (via a signal) from ordinary play to tell ordinary play has now
 * updated game info. So the next game can now be played by auto play.
 */
void CTblMngrServerAuto::sltPlayStart()
{
    //Waiting for synchronization signal from ordinary play?
    if (playWaiting)
    {
        playWaiting = false;
        playContinue = false;
        if (firstAutoSync)
        {
            firstAutoSync = false;
            //Start actors.
            actors[WEST_SEAT]->startNewSession();
            actors[NORTH_SEAT]->startNewSession();
            actors[EAST_SEAT]->startNewSession();
            actors[SOUTH_SEAT]->startNewSession();
        }
        else
        {
            //Tell clients to continue synchronization with server.
            for (int i = 0; i < 4; i++)
                actors[i]->confirmSyncFromServerToClient();
        }
    }
    else
        playContinue = true;
}

/**
 * @brief Quit the auto play thread and clean up.
 */
void CTblMngrServerAuto::sAutoQuit()
{
    deleteLater();
    thread()->quit();
}

//Slots for actors.
//-----------------------------------------------------------------------------
/**
 * @brief Actor requests connect (actor slot).
 * @param teamName Team name for actor.
 * @param seat Seat for actor.
 * @param protocol Protocol to use.
 *
 * The name is saved and connect is signalled to the statechart.
 */
void CTblMngrServerAuto::sConnect(QString teamName, Seat seat, int protocol)
{
    teamNames[seat] = teamName;
    zBridgeServerIface_raise_connect(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Actor is ready for team names (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrServerAuto::sRTNames(Seat seat)
{
    zBridgeServerIface_raise_rTNames(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Actor is ready for start of board (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrServerAuto::sRSBoard(Seat seat)
{
    zBridgeServerIface_raise_rSBoard(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Actor is ready for deal info (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrServerAuto::sRDealInfo(Seat seat)
{
    zBridgeServerIface_raise_rDealInfo(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Actor is ready for cards (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrServerAuto::sRCards(Seat seat)
{
    zBridgeServerIface_raise_rCards(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Player is ready for bid (actor slot).
 * @param player The players seat.
 * @param bidder The bidders seat.
 */
void CTblMngrServerAuto::sRBid(Seat player, Seat bidder)
{
    zBridgeServerIface_raise_rBid(&handle, player);
    serverRunCycle();
}

/**
 * @brief Bidders bid (actor slot).
 * @param bidder The bidders seat.
 * @param bid The bid.
 */
void CTblMngrServerAuto::sBid(Seat bidder, Bids bid)
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
void CTblMngrServerAuto::sPlayerPlays(Seat player, int card)
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
void CTblMngrServerAuto::sReadyForPlayer(Seat seat, Seat player, int trick)
{
    zBridgeServerIface_raise_readyForPlayer(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Actor is ready for dummy (actor slot).
 * @param seat Actors seat.
 * @param trick The trick.
 */
void CTblMngrServerAuto::sReadyForDummy(Seat seat, int trick)
{
    zBridgeServerIface_raise_readyForDummy(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Actor is ready for dummys cards (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrServerAuto::sReadyForDummyCards(Seat seat)
{
    zBridgeServerIface_raise_readyForDummyCards(&handle, seat);
    serverRunCycle();
}

/**
 * @brief Synchronization signal from one of the four clients to the server.
 * @param syncher The clients seat.
 */
void CTblMngrServerAuto::sAttemptSyncFromClientToServer(Seat syncher)
{
    if (synchronizing)
    {
        zBridgeServerSyncIface_raise_attemptSync(&syncHandle, syncher);
        serverSyncRunCycle();
    }
}

/**
 * @brief Synchronization signal from one of the four clients to the server.
 * @param syncher The clients seat.
 */
void CTblMngrServerAuto::sConfirmSyncFromClientToServer(Seat syncher)
{
    zBridgeServerSyncIface_raise_confirmSync(&syncHandle, syncher);
    serverSyncRunCycle();
}

/**
 * @brief Update game info.
 */
void CTblMngrServerAuto::sUpdateGame()
{
    games->setAutoResult(bidHistory, playHistory, teamNames[WEST_SEAT], teamNames[NORTH_SEAT],
                         teamNames[EAST_SEAT], teamNames[SOUTH_SEAT]);
}
