/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CTblMngrClientAuto.cpp
  Developers: eak

  Revision History:
  12-oct-2015 eak: Original

  Abstract: Table manager client for auto play.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the table manager client class for auto play.
 */

#include <QApplication>
#include <QMessageBox>

#include "../src-gen/sc_types.h"
#include "czbridgedoc.h"
#include "cgamesdoc.h"
#include "cactorlocalAuto.h"
#include "cremoteactorclient.h"
#include "cremoteprotocol.h"
#include "ctblmngrclientauto.h"

/**
 * @brief Constructor for table manager client.
 * @param doc Pointer to model data.
 * @param games Pointer to game data.
 * @param hostAddress Host address.
 * @param parent Pointer to parent.
 *
 * The constructor initialize the table management client.
 */
CTblMngrClientAuto::CTblMngrClientAuto(CZBridgeDoc *doc, CGamesDoc *games, QHostAddress hostAddress,
                               QObject *parent) :
    CTblMngrBase(parent)
{
    this->doc = doc;
    this->games = games;
    this->hostAddress = hostAddress;

    actor = 0;

    remoteActorClient = 0;
    actor = 0;
}

CTblMngrClientAuto::~CTblMngrClientAuto()
{
    cleanTableManager();
}

/**
 * @brief Clean table manager so that it can start a new session.
 */
void CTblMngrClientAuto::cleanTableManager()
{
    //Delete actor.
    if (actor != 0)
    {
        actor->endOfSession();
        delete actor;
        actor = 0;
    }

    //Delete tcp/ip interface to server.
    if (remoteActorClient != 0)
    {
        delete remoteActorClient;
        remoteActorClient = 0;
    }
}

//Method activated by user through main frame menus (via a signal).
//-----------------------------------------------------------------------------
/**
 * @brief Start a new session (from main menu and through a signal).
 *
 * This slot starts a new session by trying to connect
 * to a table manager server.
 *
 *   - Determine IP address (must be IPV4 address).
 *   - Prepare for a new session.
 *   - Set up actor (local).
 *   - Start tcp/ip interface to server and try to connect.
 *   - Make relevant connects to tcp/ip interface.
 */
void CTblMngrClientAuto::sNewSession()
{
    //Prepare for new session.
    cleanTableManager();
    //Set up actor.
    if (doc->getSeatOptions().seat == WEST_SEAT)
        actor = new CActorLocalAuto(tr(AUTO_SEAT_NAMES[WEST_SEAT]),
                                WEST_SEAT, doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if (doc->getSeatOptions().seat == NORTH_SEAT)
        actor = new CActorLocalAuto(tr(AUTO_SEAT_NAMES[NORTH_SEAT]),
                                NORTH_SEAT, doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if (doc->getSeatOptions().seat == EAST_SEAT)
        actor = new CActorLocalAuto(tr(AUTO_SEAT_NAMES[EAST_SEAT]),
                                EAST_SEAT, doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else
        actor = new CActorLocalAuto(tr(AUTO_SEAT_NAMES[SOUTH_SEAT]),
                                SOUTH_SEAT, doc->getNSBidOptions(), doc->getEWBidOptions(), this);

    //Only for update of Table Manager game info.
    actor->setUpdateGameInfo(true);

    //Start tcp/ip interface to server.
    remoteActorClient = new CRemoteActorClient(hostAddress.toString(),
                         doc->getSeatOptions().portClient.toInt() + AUTOPORTOFFSET, this);

    //Make relevant connects to tcp/ip interface.
    connect(remoteActorClient, &CRemoteActorClient::clientConnected, this, &CTblMngrClientAuto::clientConnected);
    connect(remoteActorClient, &CRemoteActorClient::clientDisConnected, this, &CTblMngrClientAuto::clientDisConnected, Qt::QueuedConnection);
    connect(remoteActorClient, &CRemoteActorClient::receiveLine, this, &CTblMngrClientAuto::receiveLine);
    connect(remoteActorClient, &CRemoteActorClient::sSocketError, this, &CTblMngrClientAuto::sSocketError, Qt::QueuedConnection);

    handle = actor->getHandle();
}

void CTblMngrClientAuto::sltDisconnect()
{
    cleanTableManager();
}

//Slots for actor.
//-----------------------------------------------------------------------------
/**
 * @brief Actor requests connect (actor slot).
 * @param name Team name for actor.
 * @param seat Seat for actor.
 * @param protocol Protocol to use (always advanced protocol).
 *
 * The name is set in the statechart and connect is signalled to the statechart.
 */
void CTblMngrClientAuto::sConnect(QString name, Seat seat, int protocol)
{
    CConnectMsg connectMsg(name, seat, protocol);
    remoteActorClient->sendLine(connectMsg.line);
}

/**
 * @brief Actor is ready for team names (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrClientAuto::sRTNames(Seat seat)
{
    CRTNamesMsg rTNamesMsg(seat);
    remoteActorClient->sendLine(rTNamesMsg.line);
}

/**
 * @brief Actor is ready for start of board (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrClientAuto::sRSBoard(Seat seat)
{
    CRSBoardMsg rSBoardMsg(seat);
    remoteActorClient->sendLine(rSBoardMsg.line);
}

/**
 * @brief Actor is ready for deal info (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrClientAuto::sRDealInfo(Seat seat)
{
    CRDealInfoMsg rDealInfoMsg(seat);
    remoteActorClient->sendLine(rDealInfoMsg.line);
}

/**
 * @brief Actor is ready for cards (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrClientAuto::sRCards(Seat seat)
{
    CRCardsMsg rCardsMsg(seat);
    remoteActorClient->sendLine(rCardsMsg.line);
}

/**
 * @brief Player is ready for bid (actor slot).
 * @param player The players seat.
 * @param bidder The bidders seat.
 */
void CTblMngrClientAuto::sRBid(Seat player, Seat bidder)
{
    CRBidMsg rBidMsg(player, bidder);
    remoteActorClient->sendLine(rBidMsg.line);
}

/**
 * @brief Bidders bid (actor slot).
 * @param bidder The bidders seat.
 * @param bid The bid.
 */
void CTblMngrClientAuto::sBid(Seat bidder, Bids bid)
{
    CBidMsg bidMsg(bidder, bid);
    remoteActorClient->sendLine(bidMsg.line);
}

/**
 * @brief Players play (actor slot).
 * @param player The players play.
 * @param card The card played.
 */
void CTblMngrClientAuto::sPlayerPlays(Seat player, int card)
{
    CPlayerPlaysMsg playerPlaysMsg(player, card);
    remoteActorClient->sendLine(playerPlaysMsg.line);
}

/**
 * @brief Actor is ready for player (actor slot).
 * @param seat The actors seat.
 * @param player The players seat.
 * @param trick The trick.
 */
void CTblMngrClientAuto::sReadyForPlayer(Seat seat, Seat player, int trick)
{
    CReadyForPlayerMsg readyForPlayerMsg(seat, player, trick);
    remoteActorClient->sendLine(readyForPlayerMsg.line);
}

/**
 * @brief Actor is ready for dummy (actor slot).
 * @param seat Actors seat.
 * @param trick The trick.
 */
void CTblMngrClientAuto::sReadyForDummy(Seat seat, int trick)
{
    CReadyForDummyMsg readyForDummyMsg(seat, trick);
    remoteActorClient->sendLine(readyForDummyMsg.line);
}

/**
 * @brief Actor is ready for dummys cards (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrClientAuto::sReadyForDummyCards(Seat seat)
{
    CReadyForDummyCardsMsg readyForDummyCardsMsg(seat);
    remoteActorClient->sendLine(readyForDummyCardsMsg.line);
}

/**
 * @brief Synchronization signal from client to the server.
 * @param syncher The clients seat.
 */
void CTblMngrClientAuto::sAttemptSyncFromClientToServer(Seat syncher)
{
    CAttemptSynchronizeMsg attemptSynchronizeMsg(syncher);
    remoteActorClient->sendLine(attemptSynchronizeMsg.line);
}

/**
 * @brief Synchronization signal from client to the server.
 * @param syncher The clients seat.
 */
void CTblMngrClientAuto::sConfirmSyncFromClientToServer(Seat syncher)
{
    CConfirmSynchronizeMsg confirmSynchronizeMsg(syncher);
    remoteActorClient->sendLine(confirmSynchronizeMsg.line);
}

/**
 * @brief Update game info.
 */
void CTblMngrClientAuto::sUpdateGame()
{
    //Update current game.
    games->setAutoResult(actor->getBidHistory(), actor->getPlayHistory(), teamNames[WEST_SEAT],
                           teamNames[NORTH_SEAT], teamNames[EAST_SEAT], teamNames[SOUTH_SEAT]);
}

//Slots for tcp client.
//-----------------------------------------------------------------------------
/**
 * @brief There was a socket error on the tcp/ip connection.
 * @param err
 *
 *  Reset table manager and enable new session action.
 */
void CTblMngrClientAuto::sSocketError(QString err)
{
    cleanTableManager();
    emit sigDisconnect();
}

/**
 * @brief Client is now connected to the server.
 *
 * Start a new session for the actor.
 */
void CTblMngrClientAuto::clientConnected()
{
    //Start actor.
    actor->startNewSession();
}

/**
 * @brief Client was disconnected from the server.
 *
 * Start a new session for the actor.
 */
void CTblMngrClientAuto::clientDisConnected()
{
    cleanTableManager();
    emit sigDisconnect();
}

/**
 * @brief Receive a line from the server.
 * @param line The line.
 *
 * The line is dechiffered, the message from the server identified and relevant action is taken. Most
 * of the messages are just sent to the local actor. To understand the messages check the
 * @ref protocol.
 */
void CTblMngrClientAuto::receiveLine(QString line)
{
    try
    {
    //Get the message type.
    MsgType msgType = ::getMessageType(line);

    switch (msgType)
    {
    case SEATED_MSG:
    {
        //Seated message was received.
        CSeatedMsg seatedMsg(line);
        actor->seated(seatedMsg.teamName);
        break;
    }

    case TEAMNAMES_MSG:
    {
        //Team names message was received.
        CTeamNamesMsg teamNamesMsg(line);
        actor->teamNames(teamNamesMsg.nsTeamName, teamNamesMsg.ewTeamName);
        int inx = teamNamesMsg.nsTeamName.indexOf(":");
        teamNames[NORTH_SEAT] = teamNamesMsg.nsTeamName.left(inx);
        teamNames[SOUTH_SEAT] = teamNamesMsg.nsTeamName.mid(inx + 1);
        inx = teamNamesMsg.ewTeamName.indexOf(":");
        teamNames[EAST_SEAT] = teamNamesMsg.ewTeamName.left(inx);
        teamNames[WEST_SEAT] = teamNamesMsg.ewTeamName.mid(inx + 1);
        break;
    }

    case STARTOFBOARD_MSG:
    {
        CStartOfBoardMsg startOfBoardMsg(line);
        actor->startOfBoard();
        break;
    }

    case DEALINFO_MSG:
    {
        //Deal info message was received.
        CDealInfoMsg dealInfoMsg(line);
        actor->dealInfo(dealInfoMsg.boardNumber, dealInfoMsg.dealer, dealInfoMsg.vulnerability);
        noHands = 0;
        break;
    }

    case CARDS_MSG:
    {
        //Cards message was received.
        CCardsMsg cardsMsg(line);
        //Initialize all hands of current cards.
        for (int i = 0; i < 13; i++)
            currentCards[cardsMsg.player][i] = cardsMsg.cards[i];
        noHands++;

        //Have we received all hands?
        if (noHands == 4)
        {
            //Set actor cards.
            actor->cards(currentCards);
        }
        break;
    }

    case BID_MSG:
    {
        //Bid message was received.
        CBidMsg bidMsg(line);

        actor->bidDone(bidMsg.bidder, bidMsg.bid);
        break;
    }

    case ILLEGALBID_MSG:
    {
        break;
    }

    case PLAYERTOLEAD_MSG:
    {
        //Player to lead message was received.
        CPlayerToLeadMsg playerToLeadMsg(line);
        actor->playerToLead(playerToLeadMsg.player);
        break;
    }

    case DUMMYTOLEAD_MSG:
    {
        //Dummy to lead message was received.
        CDummyToLeadMsg dummyToLeadMsg(line);
        actor->dummyToLead();
        break;
    }

    case PLAYERPLAYS_MSG:
    {
        //Player plays message was received.
        CPlayerPlaysMsg playerPlaysMsg(line);
        actor->playerPlays(playerPlaysMsg.player, playerPlaysMsg.card);
        break;
    }

    case ILLEGALCARD_MSG:
    {
        break;
    }

    case DUMMYCARDS_MSG:
    {
        //Dummy cards message was received.
        CDummyCardsMsg dummyCardsMsg(line);
        actor->dummyCards(dummyCardsMsg.cards);
        break;
    }

    case UNDOBID_MSG:
    {
        //Undo bid message was received.
        CUndoBidMsg undoBidMsg(line);
        actor->undoBid(false);
        break;
    }

    case  UNDOTRICK_MSG:
    {
        //Undo trick message was received.
        CUndoTrickMsg undoTrickMsg(line);
        actor->undoTrick(false);
        break;
    }

    case REBID_MSG:
    {
        //Rebid message was received.
        CReBidMsg reBidMsg(line);
        actor->undoBid(true);
        break;
    }

    case REPLAY_MSG:
    {
        //Replay message was received.
        CRePlayMsg rePlayMsg(line);
        actor->undoTrick(true);
        break;
    }

    case ENDOFSESSION_MSG:
    {
        //End of session message was received.
        CEndOfSessionMsg endOfSession(line);
        actor->reStart();
        break;
    }

    case ATTEMPT_SYNCHRONIZE_MSG:
    {
        //Attempt synchronize message was received.
        CAttemptSynchronizeMsg attemptSynchronize(line);
        actor->attemptSyncFromServerToClient();
        break;
    }

    case CONFIRM_SYNCHRONIZE_MSG:
    {
        //Confirm synchronize message was received.
        CConfirmSynchronizeMsg confirmSynchronize(line);
        actor->confirmSyncFromServerToClient();
        break;
    }

    case ALL_SYNCHRONIZED_MSG:
    {
        //All synchronized message was received.
        CAllSynchronizedMsg allSynchronized(line);
        actor->allSyncFromServerToClient();
        break;
    }

    default:
        throw NetProtocolException("Net - Illegal message: " + line.toStdString());
        break;

    }
    }
    catch (NetProtocolException &e)
    {
        QMessageBox::critical(0, tr("ZBridge"), e.what());

        cleanTableManager();
    }
}

/**
 * @brief Quit the auto play thread and clean up.
 */
void CTblMngrClientAuto::sAutoQuit()
{
    deleteLater();
    thread()->quit();
}

