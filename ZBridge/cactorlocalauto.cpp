/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CActorLocalAuto.cpp
  Developers: eak

  Revision History:
  12-oct-2015 eak: Original

  Abstract: Actor local class for auto play.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the actor local class for auto play.
 */

#include <cassert>
#include <QTimer>

#include "CTblMngrBase.h"
#include "cbidengine.h"
#include "cplayengine.h"
#include "defines.h"
#include "cactorlocalauto.h"

/**
 * @brief Constructor for local automatic actor.
 * @param teamName The team name for the cooperating pair of actors.
 * @param seat This actors seat.
 * @param bidAndPlayEngines Bid and Play engines.
 * @param tableManager The controlling table manager.
 *
 * The constructor:
 *   - Saves parameters for easy use later on.
 *   - Generates bid and play engines.
 *   - Initializes the Yakindu client state chart.
 */
CActorLocalAuto::CActorLocalAuto(Protocol protocol, QString teamName, Seat seat,
           CBidAndPlayEngines *bidAndPlayEngines, CTblMngrBase *tableManager) :
    CActorBase(tableManager)
{
    this->protocol = protocol;
    this->teamName = teamName;
    bidAndPlay.setBidAndPlayEngines(bidAndPlayEngines);
    bidAndPlay.setSeat(seat);
    this->tableManager = tableManager;
    updateGameInfo = false;
    synchronizing = false;

    zBridgeClient_init(&handle);
    zBridgeClientIface_set_client(&handle, seat);
}

/**
 * @brief Start a new session for the actor.
 */
void CActorLocalAuto::startNewSession()
{
    zBridgeClient_enter(&handle);
    clientActions();
}

/**
 * @brief Actions for the Yakindu statechart.
 *
 * Actions are identified by means of Yakindu out events. To follow the logic of this method one must
 * know the communication @ref protocol used in between server and actors. And also the Yakindu client
 * statechart must be known. Both can be found in the eclipse framework for the Yakindu statechart
 * models.
 */
void CActorLocalAuto::clientActions()
{
    //React to client out events.
    if (zBridgeClientIface_israised_connect(&handle))
    {
        //Connect to the server (upon entry of the statechart).
        emit sConnect(teamName ,  (Seat)zBridgeClientIface_get_client(&handle), protocol);
    }

    else if (zBridgeClientIface_israised_rTNames(&handle))
    {
        //Actor is seated and ready for team names.
        emit sRTNames( (Seat)zBridgeClientIface_get_client(&handle));
    }

    else if (zBridgeClientIface_israised_rSBoard(&handle))
    {
        //Actor has received team names and is ready for start of board.
        emit sRSBoard( (Seat)zBridgeClientIface_get_client(&handle));
    }

    else if (zBridgeClientIface_israised_rDealInfo(&handle))
    {
        //Actor has received start of board and is ready for deal information.
        emit sRDealInfo( (Seat)zBridgeClientIface_get_client(&handle));
    }

    else if (zBridgeClientIface_israised_rCards(&handle))
    {
        //Actor has received deal information and is ready for cards.
        emit sRCards((Seat)zBridgeClientIface_get_client(&handle));

        updateCurrentGameInfo = updateGameInfo;
    }

    else if (zBridgeClientIface_israised_bidInfo(&handle))
    {
        //Bidding is finished.
        //Set in bid history,
        bidAndPlay.setBidInfo((Bids)zBridgeClientIface_get_lastBid(&handle),
                               (Bids)zBridgeClientIface_get_bidDouble(&handle),
                               (Seat)zBridgeClientIface_get_bidInfo_value(&handle));
    }

    else if (zBridgeClientIface_israised_clientPlays(&handle))
    {
        //Actor plays a card.
        //Must get card to play from automatic play.
        playValue();
    }

    else if (zBridgeClientIface_israised_readyForDummyCards(&handle))
    {
        //Actor is ready for dummys cards.
        emit sReadyForDummyCards((Seat)zBridgeClientIface_get_client(&handle));
    }

    else if (zBridgeClientIface_israised_getLeader(&handle))
    {
        //Get next leader.
        zBridgeClientIface_raise_newLeader(&handle, bidAndPlay.getNextLeader());

        //State chart run cycle.
        clientRunCycle();
    }

    else if (zBridgeClientIface_israised_undoPlay(&handle) || zBridgeClientIface_israised_undoBid(&handle))
    {
    }

    else if (zBridgeClientIface_israised_undoTrick(&handle))
    {
    }

    //Can come together with undoBid and must be processed after undoBid.
    if (zBridgeClientIface_israised_giveBid(&handle))
    {
        //Must get bid from automatic player.
        //Calculate automatic bid.
        bidValue();
    }

    //Can come together with undoBid and must be processed after undoBid.
    if (zBridgeClientIface_israised_rBid(&handle))
    {
        //Actor is ready for bid.
        emit sRBid((Seat)zBridgeClientIface_get_client(&handle), (Seat)zBridgeClientIface_get_bidder(&handle));
    }

    //Initially comes together with bidInfo and must be processed after bidInfo.
    if (zBridgeClientIface_israised_readyForDummy(&handle))
    {
        //Actor is ready for dummy to play a card.
        emit sReadyForDummy((Seat)zBridgeClientIface_get_client(&handle), zBridgeClientIface_get_noTrick(&handle));
    }

    //Initially comes together with bidInfo and must be processed after bidInfo.
    if (zBridgeClientIface_israised_readyForPlayer(&handle))
    {
        //Actor is ready for player to play a card.
        emit sReadyForPlayer((Seat)zBridgeClientIface_get_client(&handle),
                             (Seat)zBridgeClientIface_get_player(&handle), zBridgeClientIface_get_noTrick(&handle));
    }

    if (zBridgeClientIface_israised_synchronize(&handle))
    {
        //Synchronization of server and clients.
        zBridgeClientSync_init(&syncHandle);
        int syncState = zBridgeClientIface_get_syncState(&handle);
        zBridgeClientSyncIface_set_syncState(&syncHandle, syncState);
        synchronizing = true;
        zBridgeClientSync_enter(&syncHandle);
        clientSyncActions();
    }
}

void CActorLocalAuto::clientSyncActions()
{
    //React to sync client out events.
    if (zBridgeClientSyncIface_israised_sendAttemptSync(&syncHandle))
    {
        //Update Table Manager game info.
        int syncState = zBridgeClientSyncIface_get_syncState(&syncHandle);
        if (updateCurrentGameInfo && (syncState == SS))
        {
            emit sUpdateGame();
            updateCurrentGameInfo = false;
        }

        Seat seat = (Seat)zBridgeClientIface_get_client(&handle);
        emit sAttemptSyncFromClientToServer(seat);
    }

    else if (zBridgeClientSyncIface_israised_sendConfirmSync(&syncHandle))
    {
        int syncState = zBridgeClientSyncIface_get_syncState(&syncHandle);
        //Synchronization after bid and before play?
        if (syncState == SP)
        {
            Seat declarer = bidAndPlay.getBidHistory().getDeclarer();
            Seat dummy = (Seat)((declarer + 2) % 4);
            Seat leader = (Seat)((declarer + 1) % 4);
            zBridgeClientIface_set_declarer(&handle, declarer);
            zBridgeClientIface_set_dummy(&handle, dummy);
            zBridgeClientIface_set_leader(&handle, leader);
            zBridgeClientIface_set_player(&handle, leader);
        }
        //Continue.
        continueSync();
    }

    else if (zBridgeClientSyncIface_israised_okSync(&syncHandle))
    {
        synchronizing = false;
        zBridgeClientIface_raise_allSync(&handle);
        clientRunCycle();
    }
}

/**
 * @brief Run cycle for the Yakindu statechart.
 */
void CActorLocalAuto::clientRunCycle()
{
    //Handle raised in flags.
    zBridgeClient_runCycle(&handle);
    //Handle out flags raised by the run cycle.
    clientActions();
    //Assure all out flags gets cleared (they have been handled now and will otherwise disturb recursive calls).
    zBridgeClient_runCycle(&handle);
}

/**
 * @brief Run cycle for the Yakindu synchronization statechart.
 */
void CActorLocalAuto::clientSyncRunCycle()
{
    //Handle raised in flags.
    zBridgeClientSync_runCycle(&syncHandle);
    //Handle out flags raised by the run cycle.
    clientSyncActions();
}

/**
 * @brief Get next bid automatically.
 */
void CActorLocalAuto::bidValue()
{
    CBid nextBid = bidAndPlay.getNextBid((Seat)zBridgeClientIface_get_bidder(&handle),
                                         (Team)zBridgeClientIface_get_vulnerability(&handle));
//    nextBid.delRules = true;            //Delete non db rules on destroy.
    nextBid.deleteRules();

    bidValue(nextBid.bid);
}

/**
 * @brief Next bid.
 * @param bid Value of next bid.
 */
void CActorLocalAuto::bidValue(Bids bid)
{
    Seat bidder = (Seat)zBridgeClientIface_get_client(&handle);

    //Server must continue to next (Bidding Wait) states before clients can send
    //signals to these states.
    emit sBid(bidder, bid);  //Server first then clients.
}

/**
 * @brief Get next play automatically.
 */
void CActorLocalAuto::playValue()
{
    int nextPlay = bidAndPlay.getNextPlay((Seat)zBridgeClientIface_get_player(&handle),
                                          (Seat)zBridgeClientIface_get_dummy(&handle));
    playValue(nextPlay);
}

/**
 * @brief Next play
 * @param card value of next play.
 *
 * This method also takes care of the situation for the declarer where player is the dummy.
 */
void CActorLocalAuto::playValue(int card)
{
    Seat player = (Seat)zBridgeClientIface_get_player(&handle);
    Seat client  = (Seat)zBridgeClientIface_get_client(&handle);

    //For declarer: player can be dummy.
    if (bidAndPlay.cardOk(card, player, player != client))
    {
        //Server must continue to next (Playing Wait) states before clients can send
        //signals to these states.
        emit sPlayerPlays((Seat)zBridgeClientIface_get_client(&handle), card); //Server first then clients.
    }
}

/**
 * @brief Continue after synchronization.
 */
void CActorLocalAuto::continueSync()
{
    Seat seat = (Seat)zBridgeClientIface_get_client(&handle);
    emit sConfirmSyncFromClientToServer(seat);
}

/**
 * @brief Seated message from table manager.
 * @param teamName The team name.
 */
void CActorLocalAuto::seated(QString teamName)
{
    //Should perhaps check that the teamname in the parameter, coming from the table manager server
    //is the same as used by the actor in the connect message (local teamname).
    zBridgeClientIface_raise_seated(&handle);
    clientRunCycle();
}

/**
 * @brief Team names message from table manager.
 * @param nsTeamName North/south team name (not used).
 * @param ewTeamName East/west team name (not used).
 */
void CActorLocalAuto::teamNames(QString nsTeamName, QString ewTeamName)
{
    //The parameter team names are not used by the actor. The table manager server or client
    //handles the names.
    zBridgeClientIface_raise_teamNames(&handle);
    clientRunCycle();
}

/**
 * @brief Start of board message from table manager.
 */
void CActorLocalAuto::startOfBoard()
{
    zBridgeClientIface_raise_startOfBoard(&handle);
    clientRunCycle();
}

/**
 * @brief Deal info message from table manager.
 * @param boardNumber The board number.
 * @param dealer The dealer.
 * @param vulnerability Vulnerability info.
 */
void CActorLocalAuto::dealInfo(int boardNumber, Seat dealer, Team vulnerability)
{
    zBridgeClientIface_set_boardNumber(&handle, boardNumber);
    zBridgeClientIface_set_dealer(&handle, dealer);
    zBridgeClientIface_set_vulnerability(&handle, vulnerability);

    zBridgeClientIface_raise_dealInfo(&handle);
    clientRunCycle();
}

/**
 * @brief Cards message from table manager.
 * @param cards The cards.
 */
void CActorLocalAuto::cards(int cards[4][13])
{
    //Save cards.
    bidAndPlay.setActorsCards(cards[(Seat)zBridgeClientIface_get_client(&handle)]);

    //Initialize for a new bid and play history.
    bidAndPlay.resetBidHistory();
    bidAndPlay.resetPlayHistory();

    zBridgeClientIface_raise_cards(&handle);
    clientRunCycle();
}

/**
 * @brief A bid has been done.
 * @param bidder The bidder.
 * @param bid The bid.
 *
 * This message is from the table manager for the 3 players waiting for a bid from the bidder.
 * The message is from the bidder itself for the bidder.
 */
void CActorLocalAuto::bidDone(Seat bidder, Bids bid)
{
    //Save bid in bid history.
    bidAndPlay.appendBid(bidder, bid, (Team)zBridgeClientIface_get_vulnerability(&handle));

    zBridgeClientIface_raise_bidDone(&handle, bid);
    clientRunCycle();
}

/**
 * @brief Player to lead message from table manager.
 * @param player The player to lead.
 *
 * This signal requires special attention. It might be received in the Bid state and
 * in the Play state and in the Lead state.
 */
void CActorLocalAuto::playerToLead(Seat player)
{
    zBridgeClientIface_raise_playerToLead(&handle);
    clientRunCycle();
}

/**
 * @brief Dummy to lead message from table manager.
 *
 * This signal requires special attention. It might be received in the Play state and in the
 * Lead state.
 */
void CActorLocalAuto::dummyToLead()
{
    zBridgeClientIface_raise_dummyToLead(&handle);
    clientRunCycle();
}

/**
 * @brief Player plays message from table manager.
 * @param player The player
 * @param card The card played.
 */
void CActorLocalAuto::playerPlays(Seat player, int card)
{
    //Update play history.
    int trick = zBridgeClientIface_get_noTrick(&handle);
    bidAndPlay.setPlay(player, trick, card);

    zBridgeClientIface_raise_playerPlays(&handle, card);
    clientRunCycle();
}

/**
 * @brief Dummy cards message from table manager.
 * @param cards Dummys cards.
 */
void CActorLocalAuto::dummyCards(int cards[])
{
    //Actor gets dummy's cards.
    bidAndPlay.setDummysCards(cards);

    zBridgeClientIface_raise_dummyCards(&handle);
    clientRunCycle();
}

/**
 * @brief Undo bid message from table manager.
 * @param reBid If true then start bidding from scratch.
 */
void CActorLocalAuto::undoBid(bool reBid)
{
    int undo;

    if (reBid)
    {
        bidAndPlay.resetPlayHistory();  //Could come from play part.
        bidAndPlay.resetBidHistory();
        undo = REBID;
    }
    else
    {
        Bids bid;
        undo = bidAndPlay.bidUndo(&bid);
        if (undo >= 0)
            zBridgeClientIface_set_bidVal(&handle, bid);
    }
    zBridgeClientIface_raise_undo(&handle, undo);
    clientRunCycle();
}

/**
 * @brief Undo trick message from table manager.
 * @param rePlay Might start from scratch with playing.
 */
void CActorLocalAuto::undoTrick(bool rePlay)
{
    int leader = NO_SEAT;

    if (rePlay)
    {
        bidAndPlay.resetPlayHistory(false);
        leader = REPLAY;
    }
    else if (zBridgeClient_isStateActive(&handle, ZBridgeClient_main_region_Play))
        leader = bidAndPlay.playUndo(PT);
    else if (zBridgeClient_isStateActive(&handle, ZBridgeClient_main_region_SyncLeader))
        leader = bidAndPlay.playUndo(CT);

    zBridgeClientIface_raise_undo(&handle, leader);
    clientRunCycle();
}

/**
 * @brief End of session message from table manager.
 */
void CActorLocalAuto::endOfSession()
{
    zBridgeClientIface_raise_endOfSession(&handle);
    clientRunCycle();
}

/**
 * @brief Restart message from table manager.
 */
void CActorLocalAuto::reStart()
{
    zBridgeClientIface_raise_reStart(&handle);
    clientRunCycle();
}

/**
 * @brief Synchronization signal from server to client.
 */
void CActorLocalAuto::attemptSyncFromServerToClient()
{
    if (synchronizing)
    {
        zBridgeClientSyncIface_raise_attemptSync(&syncHandle);
        clientSyncRunCycle();
    }
}

/**
 * @brief Synchronization signal from server to client.
 */
void CActorLocalAuto::confirmSyncFromServerToClient()
{
    zBridgeClientSyncIface_raise_confirmSync(&syncHandle);
    clientSyncRunCycle();
}

/**
 * @brief Synchronization signal from server to client.
 */
void CActorLocalAuto::allSyncFromServerToClient()
{
    zBridgeClientSyncIface_raise_allSync(&syncHandle);
    clientSyncRunCycle();
}
