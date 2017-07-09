/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTblMngrClient.cpp
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Table manager client.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the table manager client class.
 */

#include <QApplication>
#include <QMessageBox>

#include "../src-gen/sc_types.h"
#include "czbridgedoc.h"
#include "cgamesdoc.h"
#include "cplayview.h"
#include "cactorlocal.h"
#include "cremoteactorclient.h"
#include "cremoteprotocol.h"
#include "cddtable.h"
#include "cbidandplayengines.h"
#include "ctblmngrclient.h"

//Communication mode constants.
const int ORIGINAL_PBN_STREAM_MODE = 1;
const int PLAYED_PBN_STREAM_MODE = 2;
const int NORMAL_MODE = 3;

/**
 * @brief Constructor for table manager client.
 * @param doc Pointer to model data.
 * @param games Pointer to game data.
 * @param hostAddress Host address.
 * @param playView Pointer to play view.
 * @param parent Pointer to parent.
 *
 * The constructor initialize the table management client. It Enables/disables relevant main
 * menu entries.
 */
CTblMngrClient::CTblMngrClient(CZBridgeDoc *doc, CGamesDoc *games,
                               CBidAndPlayEngines *bidAndPlayEngines, QHostAddress hostAddress,
                               CPlayView *playView, QObject *parent) :
    CTblMngr(playView, parent)
{
    this->doc = doc;
    this->games = games;
    this->bidAndPlayEngines = bidAndPlayEngines;
    this->playView = playView;
    this->hostAddress = hostAddress;

    playView->resetView();

    //Enable/disable relevant menu actions.
    QApplication::postEvent(parent, new UPDATE_UI_ACTION_Event(UPDATE_UI_INITIAL, false));
    QApplication::postEvent(parent, new UPDATE_UI_ACTION_Event(UPDATE_UI_LAY_OUT_CARDS, false));

    bidAndPlayEngines = 0;

    actor = 0;

    remoteActorClient = 0;
    actor = 0;
}

CTblMngrClient::~CTblMngrClient()
{
    cleanTableManager();
}

/**
 * @brief Clean table manager so that it can start a new session.
 */
void CTblMngrClient::cleanTableManager()
{
    //Reset play view.
    playView->resetView();

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

//Method activated by user through main frame menus.
//-----------------------------------------------------------------------------
/**
 * @brief Start a new session (from main menu).
 *
 * This method is activated from the main menu. It starts a new session by trying to connect
 * to a table manager server. The main menu entry, which activates this method is only enabled
 * when the table manager client is not connected to a table manager server and can therefore
 * not be activated when the client is connected.
 *
 *   - Determine IP address (must be IPV4 address).
 *   - Prepare for a new session.
 *   - Enable/disable relevant main menu actions.
 *   - Set up actor (local).
 *   - Start tcp/ip interface to server and try to connect.
 *   - Make relevant connects to tcp/ip interface.
 */
void CTblMngrClient::newSession()
{
    //Prepare for new session.
    cleanTableManager();

    //Communication mode.
    comMode = NORMAL_MODE;

    //Enable/disable relevant menu actions.
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_CLIENT , true));
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_NEW_SESSION , false));

    //Set up actor.
    int bidDelay = (doc->getGameOptions().insertBiddingPause) ? (doc->getGameOptions().biddingPause * 100) : 0;
    int playDelay = (doc->getGameOptions().insertPlayPause) ? (doc->getGameOptions().playPause * 100) : 0;
    if (doc->getSeatOptions().seat == WEST_SEAT)
        actor = new CActorLocal(bidDelay, playDelay, (doc->getSeatOptions().westActor == MANUAL_ACTOR),
                                doc->getSeatOptions().westName, WEST_SEAT, bidAndPlayEngines, this);
    else if (doc->getSeatOptions().seat == NORTH_SEAT)
        actor = new CActorLocal(bidDelay, playDelay, (doc->getSeatOptions().northActor == MANUAL_ACTOR),
                                doc->getSeatOptions().northName, NORTH_SEAT, bidAndPlayEngines, this);
    else if (doc->getSeatOptions().seat == EAST_SEAT)
        actor = new CActorLocal(bidDelay, playDelay, (doc->getSeatOptions().eastActor == MANUAL_ACTOR),
                                doc->getSeatOptions().eastName, EAST_SEAT, bidAndPlayEngines, this);
    else
        actor = new CActorLocal(bidDelay, playDelay, (doc->getSeatOptions().southActor == MANUAL_ACTOR),
                                doc->getSeatOptions().southName, SOUTH_SEAT, bidAndPlayEngines, this);

    actor->setShowUser((actor->getActorType() == MANUAL_ACTOR) || showAll);
    actor->setUpdateGameInfo(true);

    //Start tcp/ip interface to server.
    remoteActorClient = new CRemoteActorClient(hostAddress.toString(),
                                               doc->getSeatOptions().portClient.toInt(), this);

    //Make relevant connects to tcp/ip interface.
    connect(remoteActorClient, &CRemoteActorClient::clientConnected, this, &CTblMngrClient::clientConnected);
    connect(remoteActorClient, &CRemoteActorClient::clientDisConnected, this, &CTblMngrClient::clientDisConnected, Qt::QueuedConnection);
    connect(remoteActorClient, &CRemoteActorClient::receiveLine, this, &CTblMngrClient::receiveLine);
    connect(remoteActorClient, &CRemoteActorClient::sSocketError, this, &CTblMngrClient::sSocketError, Qt::QueuedConnection);

    handle = actor->getHandle();
}

void CTblMngrClient::showAllCards()
{
    showAll = !showAll;

    bool showWest = showAll || ((actor->getSeat() == WEST_SEAT) && (actor->getActorType() == MANUAL_ACTOR)) ||
            (showDummy && (actor->getSeat() == dummy));
    bool showNorth = showAll || ((actor->getSeat() == NORTH_SEAT) && (actor->getActorType() == MANUAL_ACTOR)) ||
            (showDummy && (actor->getSeat() == dummy));
    bool showEast = showAll || ((actor->getSeat() == EAST_SEAT) && (actor->getActorType() == MANUAL_ACTOR)) ||
            (showDummy && (actor->getSeat() == dummy));
    bool showSouth = showAll || ((actor->getSeat() == SOUTH_SEAT) && (actor->getActorType() == MANUAL_ACTOR)) ||
            (showDummy && (actor->getSeat() == dummy));

    playView->showCards(WEST_SEAT, showWest);
    playView->showCards(NORTH_SEAT, showNorth);
    playView->showCards(EAST_SEAT, showEast);
    playView->showCards(SOUTH_SEAT, showSouth);
}

void CTblMngrClient::showDoubleDummyResults()
{
    CDDTable ddTable(currentCards, (Seat)zBridgeClientIface_get_dealer(handle),
                     (Team)zBridgeClientIface_get_vulnerability(handle));
    ddTable.exec();
}

void CTblMngrClient::hint()
{
        actor->getHint();
}

//Slots for play view.
//-----------------------------------------------------------------------------
/**
 * @brief A button was clicked in the play view.
 * @param button Identifies which button was clicked.
 */
void CTblMngrClient::buttonClicked(int button)
{
    sContinueSync();
}

/**
 * @brief A bid was given in the bid dialog.
 * @param bid The bid given.
 */
void CTblMngrClient::bidValue(Bids bid)
{
    actor->bidValue(bid);
}

/**
 * @brief A card was played in the play view.
 * @param card The card played.
 */
void CTblMngrClient::playValue(int card)
{
    actor->playValue(card);
}

void CTblMngrClient::bidClose()
{
}

void CTblMngrClient::handClicked(Seat seat)
{
}

//Slots for actor.
//-----------------------------------------------------------------------------
/**
 * @brief Actor requests connect (actor slot).
 * @param name Team name for actor.
 * @param seat Seat for actor.
 * @param protocol Protocol to use.
 *
 * The name is set in the statechart and connect is signalled to the statechart.
 */
void CTblMngrClient::sConnect(QString name, Seat seat, int protocol)
{
    CConnectMsg connectMsg(name, seat, protocol);
    remoteActorClient->sendLine(connectMsg.line);
}

/**
 * @brief Actor is ready for team names (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrClient::sRTNames(Seat seat)
{
    CRTNamesMsg rTNamesMsg(seat);
    remoteActorClient->sendLine(rTNamesMsg.line);
}

/**
 * @brief Actor is ready for start of board (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrClient::sRSBoard(Seat seat)
{
    CRSBoardMsg rSBoardMsg(seat);
    remoteActorClient->sendLine(rSBoardMsg.line);
}

/**
 * @brief Actor is ready for deal info (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrClient::sRDealInfo(Seat seat)
{
    CRDealInfoMsg rDealInfoMsg(seat);
    remoteActorClient->sendLine(rDealInfoMsg.line);
}

/**
 * @brief Actor is ready for cards (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrClient::sRCards(Seat seat)
{
    CRCardsMsg rCardsMsg(seat);
    remoteActorClient->sendLine(rCardsMsg.line);
}

/**
 * @brief Player is ready for bid (actor slot).
 * @param player The players seat.
 * @param bidder The bidders seat.
 */
void CTblMngrClient::sRBid(Seat player, Seat bidder)
{
    CRBidMsg rBidMsg(player, bidder);
    remoteActorClient->sendLine(rBidMsg.line);
}

/**
 * @brief Bidders bid (actor slot).
 * @param bidder The bidders seat.
 * @param bid The bid.
 */
void CTblMngrClient::sBid(Seat bidder, Bids bid)
{
//    playView->showBid(bidder, bid);
//    playView->showBid((Seat)((bidder + 1) & 3), BID_PLAYER);

    CBidMsg bidMsg(bidder, bid);
    remoteActorClient->sendLine(bidMsg.line);
}

/**
 * @brief Players play (actor slot).
 * @param player The players play.
 * @param card The card played.
 */
void CTblMngrClient::sPlayerPlays(Seat player, int card)
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
void CTblMngrClient::sReadyForPlayer(Seat seat, Seat player, int trick)
{
    CReadyForPlayerMsg readyForPlayerMsg(seat, player, trick);
    remoteActorClient->sendLine(readyForPlayerMsg.line);
}

/**
 * @brief Actor is ready for dummy (actor slot).
 * @param seat Actors seat.
 * @param trick The trick.
 */
void CTblMngrClient::sReadyForDummy(Seat seat, int trick)
{
    CReadyForDummyMsg readyForDummyMsg(seat, trick);
    remoteActorClient->sendLine(readyForDummyMsg.line);
}

/**
 * @brief Actor is ready for dummys cards (actor slot).
 * @param seat The actors seat.
 */
void CTblMngrClient::sReadyForDummyCards(Seat seat)
{
    CReadyForDummyCardsMsg readyForDummyCardsMsg(seat);
    remoteActorClient->sendLine(readyForDummyCardsMsg.line);
}

/**
 * @brief Synchronization signal from client to the server.
 * @param syncher The clients seat.
 */
void CTblMngrClient::sAttemptSyncFromClientToServer(Seat syncher)
{
    CAttemptSynchronizeMsg attemptSynchronizeMsg(syncher);
    remoteActorClient->sendLine(attemptSynchronizeMsg.line);
}

/**
 * @brief Synchronization signal from client to the server.
 * @param syncher The clients seat.
 */
void CTblMngrClient::sConfirmSyncFromClientToServer(Seat syncher)
{
    CConfirmSynchronizeMsg confirmSynchronizeMsg(syncher);
    remoteActorClient->sendLine(confirmSynchronizeMsg.line);
}

/**
 * @brief Update game info.
 */
void CTblMngrClient::sUpdateGame()
{
    //Update game info.
    games->setPlayedResult(actor->getBidHistory(), actor->getPlayHistory(), teamNames[WEST_SEAT],
                           teamNames[NORTH_SEAT], teamNames[EAST_SEAT], teamNames[SOUTH_SEAT]);
}

/**
 * @brief Prepare game info for next deal.
 */
void CTblMngrClient::sUpdateGameToNextDeal()
{
    //Prepare for next game.
    games->prepNextDeal();
}

/**
 * @brief Show auction info widgets in play view (actor slot).
 */
void CTblMngrClient::sShowAuction()
{
    playView->setParams(doc->getSeatOptions().seat, doc->getDisplayOptions().cardBack);

    QString str;
    str.setNum(zBridgeClientIface_get_boardNumber(handle));
    playView->setInfoAuction(str, (Team)zBridgeClientIface_get_vulnerability(handle), (Seat)zBridgeClientIface_get_dealer(handle));
    playView->showInfoAuction(true);
}

/**
 * @brief Show play info widgets in play view (actor slot).
 */
void CTblMngrClient::sShowPlay()
{
    playView->showInfoAuction(false);

    QString str;
    str.setNum(zBridgeClientIface_get_boardNumber(handle));
    playView->setInfoPlay(str, (Team)zBridgeClientIface_get_vulnerability(handle),
                          (Seat)zBridgeClientIface_get_dealer(handle),
                          (Seat)zBridgeClientIface_get_declarer(handle),
                          (Bids)zBridgeClientIface_get_lastBid(handle),
                          (Bids)zBridgeClientIface_get_bidDouble(handle));

    playView->showNSTricks(0);
    playView->showEWTricks(0);

    playView->showInfoPlay(true);
}

/**
 * @brief Enable auction, play, leader or next deal button.
 * @param syncState Identifies the button to enable.
 */
void CTblMngrClient::sEnableContinueSync(int syncState)
{
    switch (syncState)
    {
    case BUTTON_AUCTION:
        playView->showInfoAuctionButton(true, BUTTON_AUCTION);
        break;

    case BUTTON_PLAY:
        playView->showInfoPlayButton(true, BUTTON_PLAY);
        break;

    case BUTTON_LEADER:
        playView->enableLeaderOnTable();
        break;

    case BUTTON_DEAL:
        //Disable Show All and Double Dummy Results menu actions.
        QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_SHOW_ALL , false));
        QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_PAR , false));

        emit sShowScore();

        playView->showInfoNextButton(true, BUTTON_DEAL);
        break;

    default:
        ;
    }
}

/**
 * @brief Disable auction, play, leader or next deal button.
 * @param syncState Identifies the button to disable.
 */
void CTblMngrClient::sDisableContinueSync(int syncState)
{
    switch (syncState)
    {
    case BUTTON_AUCTION:
        playView->showInfoAuctionButton(false, BUTTON_AUCTION);
        break;

    case BUTTON_PLAY:
        playView->showInfoPlayButton(false, BUTTON_PLAY);
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

/**
 * @brief Continue after button.
 */
void CTblMngrClient::sContinueSync()
{
    actor->continueSync();
}

//Slots for tcp client.
//-----------------------------------------------------------------------------
/**
 * @brief There was a socket error on the tcp/ip connection.
 * @param err
 *
 *  Reset table manager and enable new session action.
 */
void CTblMngrClient::sSocketError(QString err)
{
    QMessageBox::information(0, tr("ZBridge"), err);

    cleanTableManager();
    emit sigDisconnect();

    //Enable new session action.
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_NEW_SESSION , true));
}

/**
 * @brief Client is now connected to the server.
 *
 * Start a new session for the actor.
 */
void CTblMngrClient::clientConnected()
{
    //Start actor.
    actor->startNewSession();

    QMessageBox::information(0, tr("ZBridge"), tr("Client connected."));
}

/**
 * @brief Client was disconnected from the server.
 *
 * Start a new session for the actor.
 */
void CTblMngrClient::clientDisConnected()
{
    QMessageBox::information(0, tr("ZBridge"), tr("Client disconnected."));

    cleanTableManager();
    emit sigDisconnect();

    //Enable new session action.
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_NEW_SESSION , true));
}

/**
 * @brief Receive a line from the server.
 * @param line The line.
 *
 * The line is dechiffered, the message from the server identified and relevant action is taken. Most
 * of the messages are just sent to the local actor. To understand the messages check the
 * @ref protocol.
 */
void CTblMngrClient::receiveLine(QString line)
{
    try
    {
    //Get the message type.
    MsgType msgType;
    if (comMode == NORMAL_MODE)
        msgType = ::getMessageType(line);
    else if (line.contains("Escape PBN Stream", Qt::CaseInsensitive))
        msgType = ESCAPE_MSG;
    else
        msgType = PBN_LINE_MSG;

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
        //Start of board message was received.
        playView->resetView();

        CStartOfBoardMsg startOfBoardMsg(line);
        actor->startOfBoard();
        break;
    }

    case DEALINFO_MSG:
    {
        //Deal info message was received.
        CDealInfoMsg dealInfoMsg(line);
        actor->dealInfo(dealInfoMsg.boardNumber, dealInfoMsg.dealer, dealInfoMsg.vulnerability);
        games->setNextDeal(dealInfoMsg.boardNumber, dealInfoMsg.dealer, dealInfoMsg.vulnerability);
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
            //Determine which cards to show and also save cards.
            bool hasWest, showWest, hasNorth, showNorth, hasEast, showEast, hasSouth, showSouth;

            hasWest = hasNorth = hasEast = hasSouth = true;

            showDummy = false;

            showWest = showAll || ((actor->getSeat() == WEST_SEAT) && (actor->getActorType() == MANUAL_ACTOR));
            showNorth = showAll || ((actor->getSeat() == NORTH_SEAT) && (actor->getActorType() == MANUAL_ACTOR));
            showEast = showAll || ((actor->getSeat() == EAST_SEAT) && (actor->getActorType() == MANUAL_ACTOR));
            showSouth = showAll || ((actor->getSeat() == SOUTH_SEAT) && (actor->getActorType() == MANUAL_ACTOR));

            games->setNextDeal(currentCards);

            //Show cards in play view.
            playView->setAndShowAllCards(hasWest, showWest, currentCards[WEST_SEAT], hasNorth, showNorth, currentCards[NORTH_SEAT],
                       hasEast, showEast, currentCards[EAST_SEAT], hasSouth, showSouth, currentCards[SOUTH_SEAT]);

            //Enable Show All and Double Dummy Results menu actions.
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_SHOW_ALL , true));
            QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_PAR , true));

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

    case ORIGINAL_PBN_START_MSG:
    {
        //Start of original PBN stream (comes always before played PBN stream).
        COriginalPBNStartMsg originalPBNStartMsg(line);
        games->clearGames(originalPBNStartMsg.scoringMethod);
        bidAndPlayEngines->setScoringMethod(originalPBNStartMsg.scoringMethod);

        originalBytes.open(QIODevice::ReadWrite);
        originalStream.setDevice(&originalBytes);
        comMode = ORIGINAL_PBN_STREAM_MODE;
        break;
    }

    case PLAYED_PBN_START_MSG:
    {
        //Start of played PBN stream (comes allways after original PBN stream).
        playedBytes.open(QIODevice::ReadWrite);
        playedStream.setDevice(&playedBytes);
        comMode = PLAYED_PBN_STREAM_MODE;
        break;

    }

    case PBN_LINE_MSG:
    {
        //Receiving a PBN file.
        if (comMode == ORIGINAL_PBN_STREAM_MODE)
            originalStream << line;
        else if (comMode == PLAYED_PBN_STREAM_MODE)
            playedStream << line;
        break;
    }

    case ESCAPE_MSG:
    {
        if (comMode == PLAYED_PBN_STREAM_MODE)
        {
            //Has now received Original and Played pbn data.
            originalStream.flush();
            playedStream.flush();
            originalStream.seek(0);
            playedStream.seek(0);

            if (!originalStream.atEnd() || !playedStream.atEnd())
            {
                //Determine event (only one event might be present).
                QStringList strLines;
                games->determineEvents(originalStream, strLines);
                originalStream.seek(0);
                QString event;
                if (strLines.size() != 0)
                    event = strLines.at(0);

                //Read games.
                try
                {
                    games->readGames(originalStream, playedStream, event, NOSCORE);
                }
                catch (PlayException &e)
                {
                    //Close and clear buffers.
                    originalBytes.buffer().clear();
                    originalBytes.close();
                    playedBytes.buffer().clear();
                    playedBytes.close();

                    //There was an error in processing of pbn data.
                    throw NetProtocolException("Net - PBN data error: " + QString(e.what()).toStdString());
                }

                //Close and clear buffers.
                originalBytes.buffer().clear();
                originalBytes.close();
                playedBytes.buffer().clear();
                playedBytes.close();
            }
        }
        comMode = NORMAL_MODE;
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

        //Enable new session action.
        QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_NEW_SESSION , true));
    }
}

void CTblMngrClient::sltDisconnect()
{
    cleanTableManager();
}

