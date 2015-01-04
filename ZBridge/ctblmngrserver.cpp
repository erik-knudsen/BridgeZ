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

#include <cassert>
#include <QApplication>

#include "../src-gen/sc_types.h"
#include "czbridgedoc.h"
#include "cgamesdoc.h"
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
CTblMngrServer::CTblMngrServer(CZBridgeDoc *doc, CGamesDoc *games, CPlayView *playView, QObject *parent) :
    CTblMngr(playView, parent)
{
    this->doc = doc;
    this->games = games;
    this->playView = playView;

    synchronizing = false;

    boardNo = -1;

    actors[WEST_SEAT] = 0;
    actors[NORTH_SEAT] = 0;
    actors[EAST_SEAT] = 0;
    actors[SOUTH_SEAT] = 0;

    //Start tcp server for remote clients.
    remoteActorServer = 0;
    if (doc->getSeatOptions().role == SERVER_ROLE)
    {
        //Determine IP address.
        QHostInfo hostInfo = QHostInfo::fromName(doc->getSeatOptions().hostServer);
        if (hostInfo.addresses().isEmpty())
            QMessageBox::warning(0, tr("ZBridge"), tr("Could not determine IP address."));
        else
        {
            QList<QHostAddress> hostAddresses = hostInfo.addresses();
            int hostInx;
            for (hostInx = 0; hostInx < hostAddresses.size(); hostInx++)
            {
                if (hostAddresses.at(hostInx).protocol() == QAbstractSocket::IPv4Protocol)
                    break;
            }
            if (hostInx == hostAddresses.size())
                QMessageBox::warning(0, tr("ZBridge"), tr("Could not determine IP address."));
            else
            {
                remoteActorServer = new CRemoteActorServer(doc->getSeatOptions().protocol,
                                               hostAddresses.at(hostInx),
                                               doc->getSeatOptions().portServer.toInt(), this);

                //Connect for disconnect of remote client.
                connect(remoteActorServer, &CRemoteActorServer::clientDisconnected, this, &CTblMngrServer::cleanTableManager);
            }
        }
    }

    //Enable/disable relevant menu actions.
    QApplication::postEvent(parent, new UPDATE_UI_ACTION_Event(UPDATE_UI_INITIAL, false));

    //Timer for supervision of continue button (only used in bascic protocol).
    leaderButton = new QTimer(this);
    connect(leaderButton, &QTimer::timeout, this, &CTblMngr::sContinueLeader);
    leaderButton->setSingleShot(true);
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

void CTblMngrServer::serverSyncRunCycle()
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
void CTblMngrServer::serverActions()
{
    //React to server out events.
    if (zBridgeServerIface_israised_seated(&handle))
    {
        //One of the actors has connected and conveyed its team name.
        Seat seat = (Seat)zBridgeServerIface_get_seated_value(&handle);

        //The actor is seated.
        char * teamName = (seat == WEST_SEAT) ? zBridgeServerIface_get_wTeamName(&handle) :
                          (seat == NORTH_SEAT) ? zBridgeServerIface_get_nTeamName(&handle) :
                          (seat == EAST_SEAT) ? zBridgeServerIface_get_eTeamName(&handle) :
                           zBridgeServerIface_get_nTeamName(&handle);

        actors[seat]->seated(QString(teamName));
    }

    else if (zBridgeServerIface_israised_teamNames(&handle))
    {
        //All actors have connected and their team names are set.
        teamNames[WEST_SEAT] = QString(zBridgeServerIface_get_wTeamName(&handle));
        teamNames[NORTH_SEAT] = QString(zBridgeServerIface_get_nTeamName(&handle));
        teamNames[EAST_SEAT] = QString(zBridgeServerIface_get_eTeamName(&handle));
        teamNames[SOUTH_SEAT] = QString(zBridgeServerIface_get_sTeamName(&handle));

        QString nsTeamName = teamNames[NORTH_SEAT];
        QString ewTeamName = teamNames[EAST_SEAT];

        if (protocol == ADVANCED_PROTOCOL)
        {
            nsTeamName += ":" + teamNames[SOUTH_SEAT];
            ewTeamName += ":" + teamNames[WEST_SEAT];
        }
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
        int delay = (protocol == BASIC_PROTOCOL) ? 1000 : 10;

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
        if (showUser)
        {
            //The server must show relevant information to the user.
            sShowAuction();                 //Show auction widgets in play view.
            sShowCenter(currentVulnerable); //Show center widget in play view.
        }

        //Determine which cards to show in play view.
        showDummy = false;
        bool showWest = showAll || (doc->getSeatOptions().westActor == MANUAL_ACTOR);
        bool showNorth = showAll || (doc->getSeatOptions().northActor == MANUAL_ACTOR);
        bool showEast = showAll || (doc->getSeatOptions().eastActor == MANUAL_ACTOR);
        bool showSouth = showAll || (doc->getSeatOptions().southActor == MANUAL_ACTOR);

        //Show the cards in play view.
        playView->setAndShowAllCards(true, showWest, currentCards[WEST_SEAT], true, showNorth, currentCards[NORTH_SEAT],
                            true, showEast, currentCards[EAST_SEAT], true, showSouth, currentCards[SOUTH_SEAT]);

        if (protocol == ADVANCED_PROTOCOL)
        {
            //Enable New Deal and Show All menu actions.
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_NEW_DEAL , true));
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_SHOW_ALL , true));
        }


        bidHistory.resetBidHistory();
        playHistory.resetPlayHistory();

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

        //Then inform players about bid (they might send signals back to server).
        if ((protocol != BASIC_PROTOCOL) || (bidder != WEST_SEAT)) actors[WEST_SEAT]->bidDone(bidder, bid);
        if ((protocol != BASIC_PROTOCOL) || (bidder != NORTH_SEAT)) actors[NORTH_SEAT]->bidDone(bidder, bid);
        if ((protocol != BASIC_PROTOCOL) || (bidder != EAST_SEAT)) actors[EAST_SEAT]->bidDone(bidder, bid);
        if ((protocol != BASIC_PROTOCOL) || (bidder != SOUTH_SEAT))  actors[SOUTH_SEAT]->bidDone(bidder, bid);
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
        if ((protocol != BASIC_PROTOCOL) || (seat != WEST_SEAT)) actors[WEST_SEAT]->playerPlays(player, card);
        if ((protocol != BASIC_PROTOCOL) || (seat != NORTH_SEAT)) actors[NORTH_SEAT]->playerPlays(player, card);
        if ((protocol != BASIC_PROTOCOL) || (seat != EAST_SEAT)) actors[EAST_SEAT]->playerPlays(player, card);
        if ((protocol != BASIC_PROTOCOL) || (seat != SOUTH_SEAT)) actors[SOUTH_SEAT]->playerPlays(player, card);
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

    else if (zBridgeServerIface_israised_undoPlay(&handle) || zBridgeServerIface_israised_undoBid(&handle))
    {
        //Undo play.
        if (zBridgeServerIface_israised_undoPlay(&handle))
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_REPLAY , false));

        //Undo bid always follows undo play.
        int val = zBridgeServerIface_get_undoBid_value(&handle);
        actors[WEST_SEAT]->undoBid(val == REBID);
        actors[NORTH_SEAT]->undoBid(val == REBID);
        actors[EAST_SEAT]->undoBid(val == REBID);
        actors[SOUTH_SEAT]->undoBid(val == REBID);
    }

    else if (zBridgeServerIface_israised_undoTrick(&handle))
    {
        //Undo trick.
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
        int delay = (protocol == BASIC_PROTOCOL) ? 1000 : 10;

        //Player to lead next trick.
        //For the basic protocol wait for one second to assure that the client to lead waits for this
        //message when it is sent.
        //The lead client on its side must assure that it has completed its end of trick work within
        //this second.
        //This is a requirement of the basic protocol.
        //For the advanced protocol just make sure the action is not called untill the run cycle of
        //the thread.
        QTimer::singleShot(delay, this, SLOT(playerToLead()));
    }

    //Can come after newDealClients.
    if (zBridgeServerIface_israised_synchronize(&handle))
    {
        if (protocol == BASIC_PROTOCOL)
        {
            zBridgeServerIface_raise_allSync(&handle);
            serverRunCycle();
        }
        else
        {
            //Synchronization of server and clients.
            zBridgeServerSync_init(&syncHandle);
            synchronizing = true;
            zBridgeServerSync_enter(&syncHandle);
            zBridgeServerSyncIface_raise_continue(&syncHandle);
            serverSyncRunCycle();
        }
    }

    //Can (in principle - not in practice) come together with bidInfo and must be processed after bidInfo.
    if (zBridgeServerIface_israised_dummyToLead(&handle))
    {
        int delay = (protocol == BASIC_PROTOCOL) ? 1000 : 10;

        //Dummy to lead next trick.
        //For the basic protocol wait for one second to assure that the client to lead waits for this
        //message when it is sent.
        //The lead client on its side must assure that it has completed its end of trick work within
        //this second.
        //This is a requirement of the basic protocol.
        //For the advanced protocol just make sure the action is not called until the run cycle of
        //the thread.
        QTimer::singleShot(delay, this, SLOT(dummyToLead()));
    }
}

void CTblMngrServer::serverSyncActions()
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

        zBridgeServerIface_raise_allSync(&handle);
        serverRunCycle();

        for (int i = 0; i < 4; i++)
            actors[i]->allSyncFromServerToClient();

        synchronizing = false;
    }

    //Comes together with and after sendAttemptSync (gets cleared in some cases by sendAttemptSync).
    if (israised_sendConfirmSync)
    {
        zBridgeServerSyncIface_raise_continue(&syncHandle);
        serverSyncRunCycle();

        for (int i = 0; i < 4; i++)
         actors[i]->confirmSyncFromServerToClient();
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
    Seat currentDealer;

    games->getNextDeal(&currentBoardNo, currentCards, &currentDealer, &currentVulnerable);
    zBridgeServerIface_set_dealer(&handle, currentDealer);
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

    waiting = false;
    boardNo = 0;

    protocol = doc->getSeatOptions().protocol;
//EAK    protocol = (remoteActorServer != 0) ? doc->getSeatOptions().protocol : ADVANCED_PROTOCOL;

    //Enable/disable relevant menu actions.
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_SERVER , protocol == ADVANCED_PROTOCOL));

    //EAK TEMPORARY.
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_SCORE , true));

    //Set up actors.
    if (doc->getSeatOptions().westActor == MANUAL_ACTOR)
        actor = new CActorLocal(true, doc->getSeatOptions().westName, WEST_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if ((remoteActorServer != 0) && remoteActorServer->isConnected(WEST_SEAT))
        actor = new CActorRemote(WEST_SEAT, protocol, remoteActorServer->getFrontend(WEST_SEAT), this);
    else
        actor = new CActorLocal(false, doc->getSeatOptions().westName, WEST_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    actors[WEST_SEAT] = actor;

    if (doc->getSeatOptions().northActor == MANUAL_ACTOR)
        actor = new CActorLocal(true, doc->getSeatOptions().northName, NORTH_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if ((remoteActorServer != 0) && remoteActorServer->isConnected(NORTH_SEAT))
        actor = new CActorRemote(NORTH_SEAT, protocol, remoteActorServer->getFrontend(NORTH_SEAT), this);
    else
        actor = new CActorLocal(false, doc->getSeatOptions().northName, NORTH_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    actors[NORTH_SEAT] = actor;

    if (doc->getSeatOptions().eastActor == MANUAL_ACTOR)
        actor = new CActorLocal(true, doc->getSeatOptions().eastName, EAST_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if ((remoteActorServer != 0) && remoteActorServer->isConnected(EAST_SEAT))
        actor = new CActorRemote(EAST_SEAT, protocol, remoteActorServer->getFrontend(EAST_SEAT), this);
    else
        actor = new CActorLocal(false, doc->getSeatOptions().eastName, EAST_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    actors[EAST_SEAT] = actor;

    if (doc->getSeatOptions().southActor == MANUAL_ACTOR)
        actor = new CActorLocal(true, doc->getSeatOptions().southName, SOUTH_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if ((remoteActorServer != 0) && remoteActorServer->isConnected(SOUTH_SEAT))
        actor = new CActorRemote(SOUTH_SEAT, protocol, remoteActorServer->getFrontend(SOUTH_SEAT), this);
    else
        actor = new CActorLocal(false, doc->getSeatOptions().southName, SOUTH_SEAT, protocol,
                doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    actors[SOUTH_SEAT] = actor;

    setShowUser(showAll);

    zBridgeServer_init(&handle);
    zBridgeServerIface_set_noOfBoards(&handle, 512);
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
    waiting = false;

    playView->resetView();

    zBridgeServerIface_raise_newDeal(&handle);
    serverRunCycle();
}

void CTblMngrServer::showAllCards()
{
    if (protocol == BASIC_PROTOCOL)
        return;

    showAll = !showAll;

    //Determine which cards to show in play view.
    bool showWest = showAll || (doc->getSeatOptions().westActor == MANUAL_ACTOR) ||
            (showDummy && (WEST_SEAT == dummy));
    bool showNorth = showAll || (doc->getSeatOptions().northActor == MANUAL_ACTOR) ||
            (showDummy && (NORTH_SEAT == dummy));
    bool showEast = showAll || (doc->getSeatOptions().eastActor == MANUAL_ACTOR) ||
            (showDummy && (EAST_SEAT == dummy));
    bool showSouth = showAll || (doc->getSeatOptions().southActor == MANUAL_ACTOR) ||
            (showDummy && (SOUTH_SEAT == dummy));

    playView->showCards(WEST_SEAT, showWest);
    playView->showCards(NORTH_SEAT, showNorth);
    playView->showCards(EAST_SEAT, showEast);
    playView->showCards(SOUTH_SEAT, showSouth);
}

/**
 * @brief Rebid the current deal (from main menu).
 */
void CTblMngrServer::reBid()
{
    assert(zBridgeServer_isActive(&handle, ZBridgeServer_entry__Bidding) ||
        zBridgeServer_isActive(&handle, ZBridgeServer_entry__Playing) ||
        zBridgeServer_isActive(&handle, ZBridgeServer_entry__SyncLeader));

    playHistory.resetPlayHistory();
    bidHistory.resetBidHistory();

    zBridgeServerIface_raise_undo(&handle, REBID);
    serverRunCycle();
}

/**
 * @brief Replay the current deal (from main menu).
 */
void CTblMngrServer::rePlay()
{
    assert(zBridgeServer_isActive(&handle, ZBridgeServer_entry__Playing) ||
           zBridgeServer_isActive(&handle, ZBridgeServer_entry__SyncLeader));

    playHistory.resetPlayHistory();

    zBridgeServerIface_raise_undo(&handle, REPLAY);
    serverRunCycle();
}

/**
 * @brief Undo bid or play (from main menu).
 */
void CTblMngrServer::undo()
{
    assert(zBridgeServer_isActive(&handle, ZBridgeServer_entry__Bidding) ||
           zBridgeServer_isActive(&handle, ZBridgeServer_entry__Playing) ||
           zBridgeServer_isActive(&handle, ZBridgeServer_entry__SyncLeader));

    if (zBridgeServer_isActive(&handle, ZBridgeServer_entry__Bidding))
    {
        Bids bidVal;
        int bidder = bidHistory.undo(&bidVal);
        if (bidder != REBID)
            zBridgeServerIface_set_bidVal(&handle, bidVal);
        zBridgeServerIface_raise_undo(&handle, bidder);
        serverRunCycle();
    }
    else if (zBridgeServer_isActive(&handle, ZBridgeServer_entry__Playing))
    {
        int leader = playHistory.undo(PT);
        zBridgeServerIface_raise_undo(&handle, leader);
        serverRunCycle();
    }
    else if (zBridgeServer_isActive(&handle, ZBridgeServer_entry__SyncLeader))
    {
        int leader = playHistory.undo(CT);
        zBridgeServerIface_raise_undo(&handle, leader);
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
    if ((protocol == BASIC_PROTOCOL) && (button == BUTTON_LEADER))
        sContinueLeader();
    else
        sContinueSync();
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
    zBridgeServerIface_set_name(&handle, teamName.toLatin1().data());
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
 * @brief Synchronization signal from one of the four clients to the server.
 * @param syncher The clients seat.
 */
void CTblMngrServer::sAttemptSyncFromClientToServer(Seat syncher)
{
    if (synchronizing)
    {
        zBridgeServerSyncIface_raise_attemptSync(&syncHandle, syncher);
        serverSyncRunCycle();
    }
}

void CTblMngrServer::sConfirmSyncFromClientToServer(Seat syncher)
{
    zBridgeServerSyncIface_raise_confirmSync(&syncHandle, syncher);
    serverSyncRunCycle();
}

/**
 * @brief Show auction info widgets in play view (actor slot).
 */
void CTblMngrServer::sShowAuction()
{
    playView->showInfoPlay(false);

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
 * @brief Enable Leader button (only basic protocol).
 *
 * For the basic protocol it is assured that the waiting time before the user presses the continue
 * button is less than one second. This is the maximum time the server waits for the clients to be ready.
 * This is a requirement of the basic protocol./n
 */
void CTblMngrServer::sEnableContinueLeader()
{
    if (!waiting)
    {
        waiting = true;

        //Waiting time must be less than one second.
        leaderButton->start(700);

        //Also show and enable continue button.
        playView->enableLeaderOnTable();
    }
}

/**
 * @brief Disable Continue button (only basic protocol).
 */
void CTblMngrServer::sDisableContinueLeader()
{
    if (waiting)
    {
        waiting = false;
        playView->disableLeaderOnTable();
    }
}

/**
 * @brief Enable auction, play, leader or next deal button (only advanced protocol).
 * @param syncState Identifies the button to enable.
 */
void CTblMngrServer::sEnableContinueSync(int syncState)
{
    if (!waiting)
    {
        waiting = true;
        switch (syncState)
        {
        case BUTTON_AUCTION:
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_UNDO , false));
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_REBID , false));
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_REPLAY , false));
            playView->showInfoAuctionButton(true, BUTTON_AUCTION);
            break;

        case BUTTON_PLAY:
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_UNDO , false));
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_REBID , false));
            playView->showInfoPlayButton(true, BUTTON_PLAY);
            break;

        case BUTTON_LEADER:
            playView->enableLeaderOnTable();
            break;

        case BUTTON_DEAL:          
            //Disable Undo and Rebid and replay and New Deal and Show All menu actions.
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_UNDO , false));
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_REBID , false));
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_REPLAY , false));
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_NEW_DEAL , false));
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_SHOW_ALL , false));

            games->setPlayedResult(bidHistory, playHistory, teamNames[WEST_SEAT], teamNames[NORTH_SEAT],
                                   teamNames[EAST_SEAT], teamNames[SOUTH_SEAT]);

            QMessageBox::information(0, tr("ZBridge"), tr("South made 6 in 2spx -300."));

            playView->showInfoNextButton(true, BUTTON_DEAL);
            break;

        default:
            ;
        }
    }
}

/**
 * @brief Disable auction, play, leader or next deal button (only advanced protocol).
 * @param syncState Identifies the button to disable.
 */
void CTblMngrServer::sDisableContinueSync(int syncState)
{
    if (waiting)
    {
        waiting = false;
        switch (syncState)
        {
        case BUTTON_AUCTION:
            playView->showInfoAuctionButton(false, BUTTON_AUCTION);
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_UNDO , true));
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_REBID , true));
            break;

        case BUTTON_PLAY:
            playView->showInfoPlayButton(false, BUTTON_PLAY);
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_UNDO , true));
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_REBID , true));
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_REPLAY , true));
            break;

        case BUTTON_LEADER:
            playView->disableLeaderOnTable();
            break;

        case BUTTON_DEAL:
            playView->showInfoNextButton(false, BUTTON_DEAL);
            break;

        default: ;
        }
    }
}

/**
 * @brief Continue play with next trick (only basic protocol).
 */
void CTblMngrServer::sContinueLeader()
{
    leaderButton->stop();

    if (actors[WEST_SEAT]->getActorType() == MANUAL_ACTOR) actors[WEST_SEAT]->continueLeader();
    if (actors[NORTH_SEAT]->getActorType() == MANUAL_ACTOR) actors[NORTH_SEAT]->continueLeader();
    if (actors[EAST_SEAT]->getActorType() == MANUAL_ACTOR) actors[EAST_SEAT]->continueLeader();
    if (actors[SOUTH_SEAT]->getActorType() == MANUAL_ACTOR) actors[SOUTH_SEAT]->continueLeader();
}

/**
 * @brief Continue after button (only advanced protocol).
 */
void CTblMngrServer::sContinueSync()
{
    if (actors[WEST_SEAT]->getActorType() == MANUAL_ACTOR) actors[WEST_SEAT]->continueSync();
    if (actors[NORTH_SEAT]->getActorType() == MANUAL_ACTOR) actors[NORTH_SEAT]->continueSync();
    if (actors[EAST_SEAT]->getActorType() == MANUAL_ACTOR) actors[EAST_SEAT]->continueSync();
    if (actors[SOUTH_SEAT]->getActorType() == MANUAL_ACTOR) actors[SOUTH_SEAT]->continueSync();
}
