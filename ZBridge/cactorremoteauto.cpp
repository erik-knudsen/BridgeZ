/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CActorRemoteAuto.cpp
  Developers: eak

  Revision History:
  12-oct-2015 eak: Original

  Abstract: Actor remote class for auto play.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the actor remote class for auto play.
 */

#include "CTblMngrBase.h"
#include "cremoteactorserver.h"
#include "cremoteprotocol.h"
#include "ZBridgeException.h"
#include "cactorremoteauto.h"

/**
 * @brief Constructor for actor remote class for auto play.
 * @param seat The seat for the actor.
 * @param frontEnd Pointer to the front end communicating with the client.
 * @param tableManager The table manager.
 *
 * Saves parameters for later use and connects to the front end.
 */
CActorRemoteAuto::CActorRemoteAuto(Seat seat, CRemoteActorFrontEnd *frontEnd, CTblMngrBase *tableManager) :
    CActorBase(tableManager)
{
    this->seat = seat;
    this->frontEnd = frontEnd;
    this->tableManager = tableManager;

    connect(this, &CActorRemoteAuto::start, frontEnd, &CRemoteActorFrontEnd::start);
    connect(frontEnd, &CRemoteActorFrontEnd::receiveLine, this, &CActorRemoteAuto::receiveLine);
    connect(this, &CActorRemoteAuto::sendLine, frontEnd, &CRemoteActorFrontEnd::sendLine);
}

/**
 * @brief Start a new session with the remote client.
 *
 * The client has already connected and exchanged the first connect message with the server. This
 * message is stored in the front end. The method emits a start signal to the front end, which in
 * response to this sends a connect message to the table manager server (injected into the Yakindu
 * state chart).
 */
void CActorRemoteAuto::startNewSession()
{
    emit start();
}

/**
 * @brief Seated message from table manager (pack and send to client via front end).
 * @param teamName The team name.
 */
void CActorRemoteAuto::seated(QString teamName)
{
    CSeatedMsg seatedMsg(seat, teamName);
    emit sendLine(seatedMsg.line);
}

/**
 * @brief Team names message from table manager (pack and send to client via front end).
 * @param nsTeamName North/south team name.
 * @param ewTeamName East/west team name.
 */
void CActorRemoteAuto::teamNames(QString nsTeamName, QString ewTeamName)
{
    CTeamNamesMsg teamNamesMsg(nsTeamName, ewTeamName);
    emit sendLine(teamNamesMsg.line);
}

/**
 * @brief Start of board message from table manager (pack and send to client via front end).
 */
void CActorRemoteAuto::startOfBoard()
{
    CStartOfBoardMsg startOfBoardMsg;
    emit sendLine(startOfBoardMsg.line);
}

/**
 * @brief Deal info message from table manager (pack and send to client via front end).
 * @param boardNumber The board number.
 * @param dealer The dealer.
 * @param vulnerability Vulnerability info.
 */
void CActorRemoteAuto::dealInfo(int boardNumber, Seat dealer, Team vulnerability)
{
    CDealInfoMsg dealInfoMsg(boardNumber, dealer, vulnerability);
    emit sendLine(dealInfoMsg.line);
}

/**
 * @brief Cards message from table manager (pack and send to client via front end).
 * @param cards The cards.
 */
void CActorRemoteAuto::cards(int cards[4][13])
{
    for (int i = 0; i < 4; i++)
    {
        CCardsMsg cardsMsg((Seat)i, cards[i]);
        emit sendLine(cardsMsg.line);
    }
}

/**
 * @brief A bid has been done (pack and send to client via front end).
 * @param bidder The bidder.
 * @param bid The bid.
 */
void CActorRemoteAuto::bidDone(Seat bidder, Bids bid)
{
    CBidMsg bidMsg(bidder, bid);
    emit sendLine(bidMsg.line);
}

/**
 * @brief Player to lead message from table manager (pack and send to client via front end).
 * @param player The player to lead.
 */
void CActorRemoteAuto::playerToLead(Seat player)
{
    CPlayerToLeadMsg playerToLeadMsg(player);
    emit sendLine(playerToLeadMsg.line);
}

/**
 * @brief Dummy to lead message from table manager (pack and send to client via front end).
 */
void CActorRemoteAuto::dummyToLead()
{
    CDummyToLeadMsg dummyToLeadMsg;
    emit sendLine(dummyToLeadMsg.line);
}

/**
 * @brief Player plays message from table manager (pack and send to client via front end).
 * @param player The player
 * @param card The card played.
 */
void CActorRemoteAuto::playerPlays(Seat player, int card)
{
    CPlayerPlaysMsg playerPlaysMsg(player, card);
    emit sendLine(playerPlaysMsg.line);
}

/**
 * @brief Dummy cards message from table manager (pack and send to client via front end).
 * @param cards Dummys cards.
 */
void CActorRemoteAuto::dummyCards(int cards[])
{
    CDummyCardsMsg dummyCardsMsg(cards);
    emit sendLine(dummyCardsMsg.line);
}

/**
 * @brief Undo bid message from table manager (pack and send to client via front end).
 * @param reBid Might start from scratch with bidding.
 */
void CActorRemoteAuto::undoBid(bool reBid)
{
    if (reBid)
    {
        CReBidMsg reBidMsg;
        emit sendLine(reBidMsg.line);
    }
    else
    {
        CUndoBidMsg undoBidMsg;
        emit sendLine(undoBidMsg.line);
    }
}

/**
 * @brief Undo trick message from table manager (pack and send to client via front end).
 * @param rePlay Might start from scratch with playing.
 */
void CActorRemoteAuto::undoTrick(bool rePlay)
{
    if (rePlay)
    {
        CRePlayMsg rePlayMsg;
        emit sendLine(rePlayMsg.line);
    }
    else
    {
        CUndoTrickMsg undoPlayMsg;
        emit sendLine(undoPlayMsg.line);
    }
}

/**
 * @brief End of session message from table manager (pack and send to client via front end).
 */
void CActorRemoteAuto::endOfSession()
{
    CEndOfSessionMsg endOfSessionMsg;
    emit sendLine(endOfSessionMsg.line);
}

/**
 * @brief Synchronization message from table manager (pack and send to client via front end).
 */
void CActorRemoteAuto::attemptSyncFromServerToClient()
{
    CAttemptSynchronizeMsg attemptSynchronizeMsg(seat);
    emit sendLine(attemptSynchronizeMsg.line);
}


/**
 * @brief Synchronization message from table manager (pack and send to client via front end).
 */
void CActorRemoteAuto::confirmSyncFromServerToClient()
{
    CConfirmSynchronizeMsg confirmSynchronizeMsg(seat);
    emit sendLine(confirmSynchronizeMsg.line);
}


/**
 * @brief Synchronization message from table manager (pack and send to client via front end).
 */
void CActorRemoteAuto::allSyncFromServerToClient()
{
    CAllSynchronizedMsg allSynchronizeMsg(seat);
    emit sendLine(allSynchronizeMsg.line);
}

/**
 * @brief Receive line from client (remote actor).
 * @param line The line.
 * @param connected If true then the initial connect message has been received.
 *
 * The method is activated via the front end. It determines the type of message received from the
 * client (remote actor), unpacks the message and emits a signal to the table manager according to
 * the type of message.
 */
void CActorRemoteAuto::receiveLine(QString line, bool connected)
{
    try
    {
    //Determine type.
    MsgType msgType = ::getMessageType(line);
    switch (msgType)
    {
    case CONNECT_MSG:
    {
        //A connect message has been received
        CConnectMsg connectMsg(line);

        //If already connected then the connect message should be discarded.
        //The message will be received when a new session is requested from the server. In this
        //case the client is not restarted completely from scratch, but the already established
        //connection is kept. So the connect message is not needed.
        if (!connected)
            emit sConnect(connectMsg.name, connectMsg.seat, connectMsg.protocol);
        break;
    }

    case RTNAMES_MSG:
    {
        //A redy for team names message has been received.
        CRTNamesMsg rTNames(line);
        emit sRTNames(rTNames.seat);
        break;
    }

    case RSBOARD_MSG:
    {
        //A ready for start of board message has been received.
        CRSBoardMsg rSBoard(line);
        emit sRSBoard(rSBoard.seat);
        break;
    }

    case RDEALINFO_MSG:
    {
        //A ready for deal info message has been received.
        CRDealInfoMsg rDealInfoMsg(line);
        emit sRDealInfo(rDealInfoMsg.seat);
        break;
    }

    case RCARDS_MSG:
    {
        //A ready for cards message has been received.
        CRCardsMsg rCardsMsg(line);
        emit sRCards(rCardsMsg.seat);
        break;
    }

    case BID_MSG:
    {
        //A bid message has been received.
        CBidMsg bidMsg(line);
        emit sBid(bidMsg.bidder, bidMsg.bid);
        break;
    }

    case RBID_MSG:
    {
        //A ready for bid message has been received.
        CRBidMsg rBidMsg(line);
        emit sRBid(rBidMsg.player, rBidMsg.bidder);
        break;
    }

    case PLAYERPLAYS_MSG:
    {
        //A player plays message has been received.
        CPlayerPlaysMsg playerPlaysMsg(line);
        emit sPlayerPlays(playerPlaysMsg.player, playerPlaysMsg.card);
        break;
    }

    case READYFORPLAYER_MSG:
    {
        //A ready for player message has been received.
        CReadyForPlayerMsg readyForPlayerMsg(line);
        emit sReadyForPlayer(readyForPlayerMsg.seat, readyForPlayerMsg.player, readyForPlayerMsg.trick);
        break;
    }

    case READYFORDUMMY_MSG:
    {
        //A ready for dummy message has been received.
        CReadyForDummyMsg readyForDummyMsg(line);
        emit sReadyForDummy(readyForDummyMsg.seat, readyForDummyMsg.trick);
        break;
    }

    case READYFORDUMMYCARDS_MSG:
    {
        //A ready for dummys cards message has been received.
        CReadyForDummyCardsMsg readyForDummyCardsMsg(line);
        emit sReadyForDummyCards(readyForDummyCardsMsg.seat);
        break;
    }

    case ATTEMPT_SYNCHRONIZE_MSG:
    {
        //An attempt synchronize message has been received.
        CAttemptSynchronizeMsg attemptSynchronize(line);
        emit sAttemptSyncFromClientToServer(attemptSynchronize.seat);
        break;
    }

    case CONFIRM_SYNCHRONIZE_MSG:
    {
        //A confirm synchronize message has been received.
        CConfirmSynchronizeMsg confirmSynchronize(line);
        emit sConfirmSyncFromClientToServer(confirmSynchronize.seat);
        break;
    }

    default:
        throw NetProtocolException("Net - Illegal message: " + line.toStdString());
        break;

    }
    }
    catch (NetProtocolException &e)
    {
        //There was an error in the message.
        QMessageBox::critical(0, tr("ZBridge"), e.what());
    }
}
