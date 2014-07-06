/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CActorLocal.cpp
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Actor local class.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the actor local class.
 */

#include <cassert>
#include <QTimer>

#include "CTblMngr.h"
#include "cbidengine.h"
#include "cplayengine.h"
#include "defines.h"
#include "cactorlocal.h"

/**
 * @brief Constructor for local actor.
 * @param manual If true then the actor is manual else it is automatic.
 * @param teamName The team name for the cooperating pair of actors.
 * @param seat This actors seat.
 * @param protocol The protocol to use (advanced or basic).
 * @param bidOptionDocOwn Own bid options.
 * @param bidOptionDocOpp Opponents bid options.
 * @param tableManager The controlling table manager.
 *
 * The constructor:
 *   - Saves parameters for easy use later on.
 *   - Generates bif and play engines.
 *   - Initializes the Yakindu client state chart.
 *   - Connects signals, mostly meant to control the play view, to the table manager.
 */
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
    synchronizing = false;

    zBridgeClient_init(&handle);
    zBridgeClientIface_set_client(&handle, seat);

    connect(this, &CActorLocal::sShowAuction, tableManager, &CTblMngr::sShowAuction);
    connect(this, &CActorLocal::sShowPlay, tableManager, &CTblMngr::sShowPlay);
    connect(this, &CActorLocal::sShowBidDialog, tableManager, &CTblMngr::sShowBidDialog);
    connect(this, &CActorLocal::sShowBid, tableManager, &CTblMngr::sShowBid);
    connect(this, &CActorLocal::sShowCenter, tableManager, &CTblMngr::sShowCenter);
    connect(this, &CActorLocal::sShowDummyCards, tableManager, &CTblMngr::sShowDummyCards);
    connect(this, &CActorLocal::sShowDummyOnTable, tableManager, &CTblMngr::sShowDummyOnTable);
    connect(this, &CActorLocal::sShowYourTurnOnTable, tableManager, &CTblMngr::sShowYourTurnOnTable);
    connect(this, &CActorLocal::sClearYourTurnOnTable, tableManager, &CTblMngr::sClearYourTurnOnTable);
    connect(this, &CActorLocal::sShowPlayerPlays, tableManager, &CTblMngr::sShowPlayerPlays);
    connect(this, &CActorLocal::sClearCardsOnTable, tableManager, &CTblMngr::sClearCardsOnTable);
    connect(this, &CActorLocal::sShowTricks, tableManager, &CTblMngr::sShowTricks);
    connect(this, &CActorLocal::sUndoBid, tableManager, &CTblMngr::sUndoBid);

    connect(this, &CActorLocal::sEnableBidder, tableManager, &CTblMngr::sEnableBidder);
    connect(this, &CActorLocal::sDisableBidder, tableManager, &CTblMngr::sDisableBidder);
    connect(this, &CActorLocal::sEnablePlayer, tableManager, &CTblMngr::sEnablePlayer);
    connect(this, &CActorLocal::sDisablePlayer, tableManager, &CTblMngr::sDisablePlayer);
    connect(this, &CActorLocal::sEnableContinueLeader, tableManager, &CTblMngr::sEnableContinueLeader);
    connect(this, &CActorLocal::sDisableContinueLeader, tableManager, &CTblMngr::sDisableContinueLeader);
    connect(this, &CActorLocal::sEnableContinueSync, tableManager, &CTblMngr::sEnableContinueSync);
    connect(this, &CActorLocal::sDisableContinueSync, tableManager, &CTblMngr::sDisableContinueSync);
}

/**
 * @brief Start a new session for the actor.
 */
void CActorLocal::startNewSession()
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
void CActorLocal::clientActions()
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
        if (showUser)
        {
            //Must show the user the auction info and the table.
            emit sShowAuction();
            emit sShowCenter((Team)zBridgeClientIface_get_vulnerability(&handle));
        }

        emit sRCards((Seat)zBridgeClientIface_get_client(&handle));
    }

    else if (zBridgeClientIface_israised_bidInfo(&handle))
    {
        //Bidding is finished.
        if (showUser)
        {
            //Must clear bid dialog, clear auction info and show play info.
            emit sShowBidDialog(false);
            emit sShowBid((Seat)zBridgeClientIface_get_bidder(&handle), BID_BLANK);
            emit sShowDummyOnTable((Seat)((zBridgeClientIface_get_declarer(&handle) + 2) & 3));
            emit sShowPlay();
        }

        //Set in bid history,
        bidAndPlay.setBidInfo((Bids)zBridgeClientIface_get_lastBid(&handle),
                               (Bids)zBridgeClientIface_get_bidDouble(&handle),
                               (Seat)zBridgeClientIface_get_bidInfo_value(&handle));
    }

    else if (zBridgeClientIface_israised_clientPlays(&handle))
    {
        //Actor plays a card.
        if (manual)
        {
            //Must get card to play from manual player.
            Seat player = (Seat)zBridgeClientIface_get_player(&handle);
            emit sShowYourTurnOnTable(player);
            emit sEnablePlayer(player);
        }
        else
        {
            //Must get card to play from automatic play.
            //Calculate automatic play.
            //For the basic protocol we need a delay to assure server gets ready for next play.
            if (protocol == BASIC_PROTOCOL)
                QTimer::singleShot(1000, this, SLOT(playValue()));
            else
                playValue();
        }
    }

    else if (zBridgeClientIface_israised_readyForDummyCards(&handle))
    {
        //Actor is ready for dummys cards.
        emit sReadyForDummyCards((Seat)zBridgeClientIface_get_client(&handle));
    }

    else if (zBridgeClientIface_israised_getLeader(&handle))
    {
        if (protocol == BASIC_PROTOCOL)
        {
            //Get leader of next play.
            if (manual)
                emit sEnableContinueLeader();
            else
                continueLeader();
        }
        else
        {
            //Get next leader.
            zBridgeClientIface_raise_newLeader(&handle, bidAndPlay.getNextLeader());

            //State chart run cycle.
            clientRunCycle();
        }
    }

    else if (zBridgeClientIface_israised_undoBid(&handle))
    {
        //Undo bid.
        if (showUser && protocol == ADVANCED_PROTOCOL)
        {
            int undo = zBridgeClientIface_get_undoBid_value(&handle);
            emit sUndoBid(undo);
        }
    }

    else if (zBridgeClientIface_israised_undoTrick(&handle))
    {
        //Undo trick.
        if (showUser && protocol == ADVANCED_PROTOCOL)
        {
            int undo = zBridgeClientIface_get_undoTrick_value(&handle);
//            emit sUndoTrick(undo);
        }
    }

    //Can come together with undoBid and must be processed after undoBid.
    if (zBridgeClientIface_israised_giveBid(&handle))
    {
        //Get next bid.
        if (manual)
        {
            //Must get bid from manual player.
            Seat bidder = (Seat)zBridgeClientIface_get_bidder(&handle);
            emit sShowYourTurnOnTable(bidder);

            emit sEnableBidder(bidder,
                               (Bids)zBridgeClientIface_get_lastBid(&handle),
                               (Bids)zBridgeClientIface_get_bidEnable(&handle));
        }
        else
        {
            //Must get bid from automatic player.
            //Calculate automatic bid.
            //for the basic protocol we need a delay to assure server gets ready for next bid.
            if (protocol == BASIC_PROTOCOL)
                QTimer::singleShot(1000, this, SLOT(bidValue()));
            else
                bidValue();
        }
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

    //Can Come after undoTrick.
    if (zBridgeClientIface_israised_synchronize(&handle))
    {
        if (protocol == BASIC_PROTOCOL)
        {
            //There is no synchronization.
            zBridgeClientIface_raise_allSync(&handle);
            clientRunCycle();
        }
        else
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
}

void CActorLocal::clientSyncActions()
{
    //React to sync client out events.
    if (zBridgeClientSyncIface_israised_sendAttemptSync(&syncHandle))
    {
        Seat seat = (Seat)zBridgeClientIface_get_client(&handle);
        emit sAttemptSyncFromClientToServer(seat);
    }

    else if (zBridgeClientSyncIface_israised_sendConfirmSync(&syncHandle))
    {
        int syncState = zBridgeClientSyncIface_get_syncState(&syncHandle);
        if (manual && ((syncState == SA) || (syncState == SP) || (syncState == SS) || (syncState == SL)))
            emit sEnableContinueSync(zBridgeClientSyncIface_get_syncState(&syncHandle));
        else
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
void CActorLocal::clientRunCycle()
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
void CActorLocal::clientSyncRunCycle()
{
    //Handle raised in flags.
    zBridgeClientSync_runCycle(&syncHandle);
    //Handle out flags raised by the run cycle.
    clientSyncActions();
}

/**
 * @brief Get next bid automatically.
 *
 * For the basic protocol:
 *   - Bidders bid must be delayed to assure that the other 3 players have reported they are ready.
 *     This method is activated after a delay, when the bid is done automatically to assure this. When
 *     the bid is done manually this happens automatically. The problem is that bidders bid is followed
 *     immediately by bidder reporting ready for next bid.\n
 *
 * For the advanced protocol:
 *   - All 4 bidders requires the bid to be returned and this eliminates the need for a delay.
 */
void CActorLocal::bidValue()
{
    Bids nextBid = bidAndPlay.getNextBid((Seat)zBridgeClientIface_get_bidder(&handle));
    bidValue(nextBid);
}

/**
 * @brief Next bid.
 * @param bid Value of next bid.
 */
void CActorLocal::bidValue(Bids bid)
{
    Seat bidder = (Seat)zBridgeClientIface_get_client(&handle);

    //If the bid is manual then disable the bid dialog.
    if (manual)
    {
        emit sDisableBidder(bidder);
        emit sClearYourTurnOnTable();
    }

    //Server must continue to next (Bidding Wait) states before clients can send
    //signals to these states.
    emit sBid(bidder, bid);  //Server first then clients.

    if (protocol == BASIC_PROTOCOL)
        bidDone(bidder, bid);    //This client.
}

/**
 * @brief Get next play automatically.
 *
 * For the basic protocol:
 *   - Players play must be delayed to assure that the other 3 players have reported they are ready. This
 *     method is activated after a delay, when the play is done automatically to assure this. When the play
 *     is done manually this happens automatically. The problem is that players play is followed immediately
 *     by player reporting ready for next play.\n
 *
 * For the advanced protocol:
 *   - All 4 players requires the play to be returned and this eliminates the need for a delay.
 */
void CActorLocal::playValue()
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
void CActorLocal::playValue(int card)
{
    Seat player = (Seat)zBridgeClientIface_get_player(&handle);
    Seat client  = (Seat)zBridgeClientIface_get_client(&handle);

    //For declarer: player can be dummy.
    if (bidAndPlay.cardOk(card, player, player != client))
    {
        //If the play is manual then disable the player.
        if (manual)
        {
            emit sDisablePlayer(player);
            emit sClearYourTurnOnTable();
        }


        //Server must continue to next (Playing Wait) states before clients can send
        //signals to these states.
        emit sPlayerPlays((Seat)zBridgeClientIface_get_client(&handle), card); //Server first then clients.

        if (protocol == BASIC_PROTOCOL)
            playerPlays(player, card); //This client.
    }
}

/**
 * @brief Continue with play when a trick has been played by all four players (only basic protocol).
 *
 * Prepare and initialize for the next trick to be played.
 */
void CActorLocal::continueLeader()
{
    //If manual then disable the continue button.
    if (manual)
        emit sDisableContinueLeader();

    //Must prepare play view for next trick.
    if (showUser)
        emit sClearCardsOnTable();

    //Get next leader.
    zBridgeClientIface_raise_newLeader(&handle, bidAndPlay.getNextLeader());

    if (showUser)
    {
        //Show number of tricks.
        emit sShowTricks(bidAndPlay.getEWTricks(), bidAndPlay.getNSTricks());
    }

    //State chart run cycle.
    clientRunCycle();
}

/**
 * @brief Continue after synchronization (only advanced protocol).
 */
void CActorLocal::continueSync()
{
    int syncState = zBridgeClientSyncIface_get_syncState(&syncHandle);
    if (manual && ((syncState == SA) || (syncState == SP) || (syncState == SS) || (syncState == SL)))
        emit sDisableContinueSync(zBridgeClientSyncIface_get_syncState(&syncHandle));

    if (showUser &&(syncState == SL))
    {
        //Must prepare play view for next trick.
        emit sClearCardsOnTable();

        //Show number of tricks.
        emit sShowTricks(bidAndPlay.getEWTricks(), bidAndPlay.getNSTricks());
    }

    Seat seat = (Seat)zBridgeClientIface_get_client(&handle);
    emit sConfirmSyncFromClientToServer(seat);
}

/**
 * @brief Seated message from table manager.
 * @param teamName The team name.
 */
void CActorLocal::seated(QString teamName)
{
    //EAK What to do with teamName?
    zBridgeClientIface_raise_seated(&handle);
    clientRunCycle();
}

/**
 * @brief Team names message from table manager.
 * @param nsTeamName North/south team name.
 * @param ewTeamName East/west team name.
 */
void CActorLocal::teamNames(QString nsTeamName, QString ewTeamName)
{
    //EAK What to do  with team names?
    zBridgeClientIface_raise_teamNames(&handle);
    clientRunCycle();
}

/**
 * @brief Start of board message from table manager.
 */
void CActorLocal::startOfBoard()
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
void CActorLocal::dealInfo(int boardNumber, Seat dealer, Team vulnerability)
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
void CActorLocal::cards(int cards[4][13])
{
    //Save cards.
    bidAndPlay.setActorsCards(cards[(Seat)zBridgeClientIface_get_client(&handle)]);

    //Initialize for a new bid history.
    bidAndPlay.resetBidHistory();

    if (showUser)
    {
        //Prepare for bidding.
        emit sShowBidDialog(true);
        emit sShowBid((Seat)zBridgeClientIface_get_dealer(&handle), BID_PLAYER);
    }

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
void CActorLocal::bidDone(Seat bidder, Bids bid)
{
    CBid bidEntry(bidder, bid, "alert");

    //Save bid in bid history.
    bidAndPlay.appendBid(bidEntry);

    if (showUser)
    {
        //Show bid in play view.
        emit sShowBid(bidder, bid);
        emit sShowBid((Seat)((bidder + 1) & 3), BID_PLAYER);
    }

    zBridgeClientIface_raise_bidDone(&handle, bid);
    clientRunCycle();
}

/**
 * @brief Player to lead message from table manager.
 * @param player The player to lead.
 *
 * This signal requires special attention. It might be received in the Bid state and
 * in the Play state and in the Lead state. This somewhat messy handling is due to the
 * way the protocol is structured.
 */
void CActorLocal::playerToLead(Seat player)
{
    zBridgeClientIface_raise_playerToLead(&handle);
    clientRunCycle();
}

/**
 * @brief Dummy to lead message from table manager.
 *
 * This signal requires special attention. It might be received in the Play state and in the
 * Lead state. This somewhat messy handling is due to the way the protocol is structured.
 */
void CActorLocal::dummyToLead()
{
    zBridgeClientIface_raise_dummyToLead(&handle);
    clientRunCycle();
}

/**
 * @brief Player plays message from table manager.
 * @param player The player
 * @param card The card played.
 */
void CActorLocal::playerPlays(Seat player, int card)
{
    //Update play history.
    int trick = zBridgeClientIface_get_noTrick(&handle);
    bidAndPlay.setPlay(player, trick, card);

    //Show in play view.
    if (showUser)
        emit sShowPlayerPlays(player, card);

    zBridgeClientIface_raise_playerPlays(&handle, card);
    clientRunCycle();
}

/**
 * @brief Dummy cards message from table manager.
 * @param cards Dummys cards.
 */
void CActorLocal::dummyCards(int cards[])
{
    //Actor gets dummy's cards.
    bidAndPlay.setDummysCards(cards);

    //Also show the cards on the display.
    if (showUser)
        emit sShowDummyCards((Seat)zBridgeClientIface_get_dummy(&handle), cards);

    zBridgeClientIface_raise_dummyCards(&handle);
    clientRunCycle();
}

/**
 * @brief Undo bid message from table manager.
 * @param reBid If true then start bidding from scratch.
 */
void CActorLocal::undoBid(bool reBid)
{
    int undo;

    if (reBid)
    {
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
void CActorLocal::undoTrick(bool rePlay)
{
    int leader;

    if (rePlay)
    {
        bidAndPlay.resetPlayHistory();
        leader = REPLAY;
    }
    else if (zBridgeClient_isActive(&handle, ZBridgeClient_main_region_Play))
        leader = bidAndPlay.playUndo(PT);
    else if (zBridgeClient_isActive(&handle, ZBridgeClient_main_region_SyncLeader))
        leader = bidAndPlay.playUndo(CT);

    zBridgeClientIface_raise_undo(&handle, leader);
    clientRunCycle();
}

/**
 * @brief End of session message from table manager.
 */
void CActorLocal::endOfSession()
{
    zBridgeClientIface_raise_endOfSession(&handle);
    clientRunCycle();
}

void CActorLocal::reStart()
{
    zBridgeClientIface_raise_reStart(&handle);
    clientRunCycle();
}

void CActorLocal::attemptSyncFromServerToClient()
{
    if (synchronizing)
    {
        zBridgeClientSyncIface_raise_attemptSync(&syncHandle);
        clientSyncRunCycle();
    }
}

void CActorLocal::confirmSyncFromServerToClient()
{
    zBridgeClientSyncIface_raise_confirmSync(&syncHandle);
    clientSyncRunCycle();
}

void CActorLocal::allSyncFromServerToClient()
{
    zBridgeClientSyncIface_raise_allSync(&syncHandle);
    clientSyncRunCycle();
}
