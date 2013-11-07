#include "CTblMngr.h"
#include "cremoteactorserver.h"
#include "cremoteprotocol.h"
#include "ZBridgeException.h"
#include "cactorremote.h"

CActorRemote::CActorRemote(Seat seat, CRemoteActorFrontEnd *frontEnd, CTblMngr *tableManager) :
    CActor(tableManager)
{
    this->seat = seat;
    this->frontEnd = frontEnd;
    this->tableManager = tableManager;

    connect(this, &CActorRemote::start, frontEnd, &CRemoteActorFrontEnd::start);
    connect(frontEnd, &CRemoteActorFrontEnd::receiveLine, this, &CActorRemote::receiveLine);
    connect(this, &CActorRemote::sendLine, frontEnd, &CRemoteActorFrontEnd::sendLine);
}

void CActorRemote::startNewSession()
{
    emit start();
}

void CActorRemote::seated(QString teamName)
{
    CSeatedMsg seatedMsg(teamName);
    emit sendLine(seatedMsg.line);
}

void CActorRemote::teamNames(QString nsTeamName, QString ewTeamName)
{
    CTeamNamesMsg teamNamesMsg(nsTeamName, ewTeamName);
    emit sendLine(teamNamesMsg.line);
}

void CActorRemote::startOfBoard()
{
    CStartOfBoardMsg startOfBoardMsg;
    emit sendLine(startOfBoardMsg.line);
}

void CActorRemote::dealInfo(int boardNumber, Seat dealer, Team vulnerability)
{
    CDealInfoMsg dealInfoMsg(boardNumber, dealer, vulnerability);
    emit sendLine(dealInfoMsg.line);
}

void CActorRemote::cards(int cards[4][13])
{
    CCardsMsg cardsMsg(seat, cards[seat]);
    emit sendLine(cardsMsg.line);
}

void CActorRemote::bidDone(Seat bidder, Bids bid)
{
    CBidMsg bidMsg(bidder, bid);
    emit sendLine(bidMsg.line);
}

void CActorRemote::playerToLead(Seat player)
{
    CPlayerToLeadMsg playerToLeadMsg(player);
    emit sendLine(playerToLeadMsg.line);
}

void CActorRemote::dummyToLead()
{
    CDummyToLeadMsg dummyToLeadMsg;
    emit sendLine(dummyToLeadMsg.line);
}

void CActorRemote::playerPlays(Seat player, int card)
{
    CPlayerPlaysMsg playerPlaysMsg(player, card);
    emit sendLine(playerPlaysMsg.line);
}

void CActorRemote::dummyCards(int cards[])
{
    CDummyCardsMsg dummyCardsMsg(cards);
    emit sendLine(dummyCardsMsg.line);
}

void CActorRemote::undoBid(bool reBid)
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

void CActorRemote::undoTrick(bool rePlay)
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

void CActorRemote::endOfSession()
{
    CEndOfSessionMsg endOfSessionMsg;
    emit sendLine(endOfSessionMsg.line);
}

void CActorRemote::receiveLine(QString line)
{
    try
    {
    MsgType msgType = ::getMessageType(line);
    switch (msgType)
    {
    case CONNECT_MSG:
    {
        CConnectMsg connectMsg(line);
        emit sConnect(connectMsg.name, connectMsg.seat, connectMsg.protocol);
        break;
    }

    case RTNAMES_MSG:
    {
        CRTNamesMsg rTNames(line);
        emit sRTNames(rTNames.seat);
        break;
    }

    case RSBOARD_MSG:
    {
        CRSBoardMsg rSBoard(line);
        emit sRSBoard(rSBoard.seat);
        break;
    }

    case RDEALINFO_MSG:
    {
        CRDealInfoMsg rDealInfoMsg(line);
        emit sRDealInfo(rDealInfoMsg.seat);
        break;
    }

    case RCARDS_MSG:
    {
        CRCardsMsg rCardsMsg(line);
        emit sRCards(rCardsMsg.seat);
        break;
    }

    case BID_MSG:
    {
        CBidMsg bidMsg(line);
        emit sBid(bidMsg.bidder, bidMsg.bid);
        break;
    }

    case RBID_MSG:
    {
        CRBidMsg rBidMsg(line);
        emit sRBid(rBidMsg.player, rBidMsg.bidder);
        break;
    }

    case PLAYERPLAYS_MSG:
    {
        CPlayerPlaysMsg playerPlaysMsg(line);
        emit sPlayerPlays(playerPlaysMsg.player, playerPlaysMsg.card);
        break;
    }

    case READYFORPLAYER_MSG:
    {
        CReadyForPlayerMsg readyForPlayerMsg(line);
        emit sReadyForPlayer(readyForPlayerMsg.seat, readyForPlayerMsg.player, readyForPlayerMsg.trick);
        break;
    }

    case READYFORDUMMY_MSG:
    {
        CReadyForDummyMsg readyForDummyMsg(line);
        emit sReadyForDummy(readyForDummyMsg.seat, readyForDummyMsg.trick);
        break;
    }

    case READYFORDUMMYCARDS_MSG:
    {
        CReadyForDummyCardsMsg readyForDummyCardsMsg(line);
        emit sReadyForDummyCards(readyForDummyCardsMsg.seat);
        break;
    }

    default:
        throw NetProtocolException("Net - Illegal message: " + line.toStdString());
        break;

    }
    }
    catch (NetProtocolException &e)
    {

    }
}
