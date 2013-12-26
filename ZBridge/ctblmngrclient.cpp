#include <QApplication>
#include <QMessageBox>

#include "../src-gen/sc_types.h"
#include "czbridgedoc.h"
#include "cplayview.h"
#include "cactorlocal.h"
#include "cremoteactorclient.h"
#include "cremoteprotocol.h"
#include "ctblmngrclient.h"


//Table Manager client.
CTblMngrClient::CTblMngrClient(CZBridgeDoc *doc, CPlayView *playView, QObject *parent) :
    CTblMngr(playView, parent)
{
    this->doc = doc;
    this->playView = playView;

    //Enable/disable relevant menu actions.
    QApplication::postEvent(parent, new UPDATE_UI_ACTION_Event(UPDATE_UI_INITIAL));

    actor = 0;

    remoteActorClient = 0;
    actor = 0;

    //Timer for supervision of continue button.
    continueButton = new QTimer(this);
    connect(continueButton, &QTimer::timeout, this, &CTblMngr::sContinuePlay);
    continueButton->setSingleShot(true);
}

CTblMngrClient::~CTblMngrClient()
{
    cleanTableManager();
}

void CTblMngrClient::cleanTableManager()
{
    playView->resetView();

    if (actor != 0)
    {
        actor->endOfSession();
        delete actor;
        actor = 0;
    }
    if (remoteActorClient != 0)
    {
        delete remoteActorClient;
        remoteActorClient = 0;
    }
}

//Method activated by user through main frame menus.
//-----------------------------------------------------------------------------
void CTblMngrClient::newSession()
{
    cleanTableManager();

    protocol = doc->getSeatOptions().protocol;

    //Enable/disable relevant menu actions.
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_CLIENT , protocol == ADVANCED_PROTOCOL));
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_NEW_SESSION , false));

    QString ewTeamName = "ewTeam";
    QString nsTeamName = "nsTeam";

    if (doc->getSeatOptions().seat == WEST_SEAT)
        actor = new CActorLocal((doc->getSeatOptions().westActor == MANUAL_ACTOR), ewTeamName, WEST_SEAT,
                protocol, doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if (doc->getSeatOptions().seat == NORTH_SEAT)
        actor = new CActorLocal((doc->getSeatOptions().northActor == MANUAL_ACTOR), nsTeamName, NORTH_SEAT,
                protocol, doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else if (doc->getSeatOptions().seat == EAST_SEAT)
        actor = new CActorLocal((doc->getSeatOptions().eastActor == MANUAL_ACTOR), ewTeamName, EAST_SEAT,
                protocol, doc->getNSBidOptions(), doc->getEWBidOptions(), this);
    else
        actor = new CActorLocal((doc->getSeatOptions().southActor == MANUAL_ACTOR), nsTeamName, SOUTH_SEAT,
                protocol, doc->getNSBidOptions(), doc->getEWBidOptions(), this);

    actor->setShowUser((actor->getActorType() == MANUAL_ACTOR) || showAll);

    remoteActorClient = new CRemoteActorClient(doc->getSeatOptions().hostClient,
                                               doc->getSeatOptions().portClient.toInt(), this);
    connect(remoteActorClient, &CRemoteActorClient::clientConnected, this, &CTblMngrClient::clientConnected);
    connect(remoteActorClient, &CRemoteActorClient::clientDisConnected, this, &CTblMngrClient::clientDisConnected, Qt::QueuedConnection);
    connect(remoteActorClient, &CRemoteActorClient::receiveLine, this, &CTblMngrClient::receiveLine);
    connect(remoteActorClient, &CRemoteActorClient::sSocketError, this, &CTblMngrClient::sSocketError, Qt::QueuedConnection);

    handle = actor->getHandle();
}

//Slots for tcp client.
//-----------------------------------------------------------------------------
void CTblMngrClient::sSocketError(QString err)
{
    QMessageBox::information(0, tr("ZBridge"), err);

    cleanTableManager();

    //Enable new session action.
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_NEW_SESSION , true));
}

void CTblMngrClient::clientConnected()
{
    //Start actor.
    actor->startNewSession();

    QMessageBox::information(0, tr("ZBridge"), tr("Client connected."));
}

void CTblMngrClient::clientDisConnected()
{
    QMessageBox::information(0, tr("ZBridge"), tr("Client disconnected."));

    cleanTableManager();

    //Enable new session action.
    QApplication::postEvent(parent(), new UPDATE_UI_ACTION_Event(UPDATE_UI_NEW_SESSION , true));
}

void CTblMngrClient::receiveLine(QString line)
{
    try
    {
    MsgType msgType = ::getMessageType(line);
    switch (msgType)
    {

    case SEATED_MSG:
    {
        CSeatedMsg seatedMsg(line);
        actor->seated(seatedMsg.name);
        break;
    }

    case TEAMNAMES_MSG:
    {
        CTeamNamesMsg teamNamesMsg(line);
        actor->teamNames(teamNamesMsg.nsTeamName, teamNamesMsg.ewTeamName);
        break;
    }

    case STARTOFBOARD_MSG:
    {
        playView->resetView();

        CStartOfBoardMsg startOfBoardMsg(line);
        actor->startOfBoard();
        break;
    }

    case DEALINFO_MSG:
    {
        CDealInfoMsg dealInfoMsg(line);
        actor->dealInfo(dealInfoMsg.boardNumber, dealInfoMsg.dealer, dealInfoMsg.vulnerability);
        noHands = 0;
        break;
    }

    case CARDS_MSG:
    {
        CCardsMsg cardsMsg(line);
        if (protocol == ADVANCED_PROTOCOL)
        {
            //Initialize all hands of current cards.
            for (int i = 0; i < 13; i++)
                currentCards[cardsMsg.player][i] = cardsMsg.cards[i];
            noHands++;
        }
        else
        {
            //initialize only the clients hand of current cards.
            for (int i = 0; i < 13; i++)
                currentCards[cardsMsg.player][i] = cardsMsg.cards[i];
            noHands = 4;
        }

        if (noHands == 4)
        {
            bool hasWest, showWest, hasNorth, showNorth, hasEast, showEast, hasSouth, showSouth;

            if (protocol == ADVANCED_PROTOCOL)
            {
                hasWest = hasNorth = hasEast = hasSouth = true;

                showWest = showAll || ((actor->getSeat() == WEST_SEAT) && (actor->getActorType() == MANUAL_ACTOR));
                showNorth = showAll || ((actor->getSeat() == NORTH_SEAT) && (actor->getActorType() == MANUAL_ACTOR));
                showEast = showAll || ((actor->getSeat() == EAST_SEAT) && (actor->getActorType() == MANUAL_ACTOR));
                showSouth = showAll || ((actor->getSeat() == SOUTH_SEAT) && (actor->getActorType() == MANUAL_ACTOR));
            }
            else
            {
                hasWest = showWest = (actor->getSeat() == WEST_SEAT) && (actor->getActorType() == MANUAL_ACTOR);
                hasNorth = showNorth = (actor->getSeat() == NORTH_SEAT) && (actor->getActorType() == MANUAL_ACTOR);
                hasEast = showEast = (actor->getSeat() == EAST_SEAT) && (actor->getActorType() == MANUAL_ACTOR);
                hasSouth = showSouth = (actor->getSeat() == SOUTH_SEAT) && (actor->getActorType() == MANUAL_ACTOR);

            }

            playView->setAndShowAllCards(hasWest, showWest, currentCards[WEST_SEAT], hasNorth, showNorth, currentCards[NORTH_SEAT],
                       hasEast, showEast, currentCards[EAST_SEAT], hasSouth, showSouth, currentCards[SOUTH_SEAT]);

            actor->cards(currentCards);
        }
        break;
    }

    case BID_MSG:
    {
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
        CPlayerToLeadMsg playerToLeadMsg(line);
        actor->playerToLead(playerToLeadMsg.player);
        break;
    }

    case DUMMYTOLEAD_MSG:
    {
        CDummyToLeadMsg dummyToLeadMsg(line);
        actor->dummyToLead();
        break;
    }

    case PLAYERPLAYS_MSG:
    {
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
        CDummyCardsMsg dummyCardsMsg(line);
        actor->dummyCards(dummyCardsMsg.cards);
        break;
    }

    case UNDOBID_MSG:
    {
        CUndoBidMsg undoBidMsg(line);
        actor->undoBid(false);
        break;
    }

    case  UNDOTRICK_MSG:
    {
        CUndoTrickMsg undoTrickMsg(line);
        actor->undoTrick(false);
    }

    case REBID_MSG:
    {
        CReBidMsg reBidMsg(line);
        actor->undoBid(true);
        break;
    }

    case REPLAY_MSG:
    {
        CRePlayMsg rePlayMsg(line);
        actor->undoTrick(true);
        break;
    }

    case ENDOFSESSION_MSG:
    {
        CEndOfSessionMsg endOfSession(line);
        actor->endOfSession();
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
    }
}

//Slots for actor.
//-----------------------------------------------------------------------------
void CTblMngrClient::sConnect(QString name, Seat seat, int protocol)
{
    CConnectMsg connectMsg(name, seat, protocol);
    remoteActorClient->sendLine(connectMsg.line);
}

void CTblMngrClient::sRTNames(Seat seat)
{
    CRTNamesMsg rTNamesMsg(seat);
    remoteActorClient->sendLine(rTNamesMsg.line);
}

void CTblMngrClient::sRSBoard(Seat seat)
{
    CRSBoardMsg rSBoardMsg(seat);
    remoteActorClient->sendLine(rSBoardMsg.line);
}

void CTblMngrClient::sRDealInfo(Seat seat)
{
    CRDealInfoMsg rDealInfoMsg(seat);
    remoteActorClient->sendLine(rDealInfoMsg.line);
}

void CTblMngrClient::sRCards(Seat seat)
{
    CRCardsMsg rCardsMsg(seat);
    remoteActorClient->sendLine(rCardsMsg.line);
}

void CTblMngrClient::sRBid(Seat player, Seat bidder)
{
    CRBidMsg rBidMsg(player, bidder);
    remoteActorClient->sendLine(rBidMsg.line);
}

void CTblMngrClient::sBid(Seat bidder, Bids bid)
{
    playView->showBid(bidder, bid);
    playView->showBid((Seat)((bidder + 1) & 3), BID_PLAYER);

    CBidMsg bidMsg(bidder, bid);
    remoteActorClient->sendLine(bidMsg.line);
}

void CTblMngrClient::sPlayerPlays(Seat player, int card)
{
    CPlayerPlaysMsg playerPlaysMsg(player, card);
    remoteActorClient->sendLine(playerPlaysMsg.line);
}

void CTblMngrClient::sReadyForPlayer(Seat seat, Seat player, int trick)
{
    CReadyForPlayerMsg readyForPlayerMsg(seat, player, trick);
    remoteActorClient->sendLine(readyForPlayerMsg.line);
}

void CTblMngrClient::sReadyForDummy(Seat seat, int trick)
{
    CReadyForDummyMsg readyForDummyMsg(seat, trick);
    remoteActorClient->sendLine(readyForDummyMsg.line);
}

void CTblMngrClient::sReadyForDummyCards(Seat seat)
{
    CReadyForDummyCardsMsg readyForDummyCardsMsg(seat);
    remoteActorClient->sendLine(readyForDummyCardsMsg.line);
}

void CTblMngrClient::sShowAuction()
{
    playView->setParams(doc->getSeatOptions().seat, doc->getDisplayOptions().cardBack);

    QString str;
    str.setNum(zBridgeClientIface_get_boardNumber(handle));
    playView->setInfoAuction(str, (Team)zBridgeClientIface_get_vulnerability(handle), (Seat)zBridgeClientIface_get_dealer(handle));
    playView->showInfoAuction(true);
}

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

void CTblMngrClient::sEnableContinue()
{
    //Waiting time must be less than one second. This is the maximum time the server waits
    //for the clients to be ready.
    //This is a requirement of the protocol.
    continueButton->start(700);

    //Also show and enable continue button.
    playView->enableContinueOnTable();
}

void CTblMngrClient::sDisableContinue()
{
    playView->disableContinueOnTable();
}

void CTblMngrClient::sContinuePlay()
{
    continueButton->stop();

    actor->continuePlay();
}

//Slots for play view.
//-----------------------------------------------------------------------------
void CTblMngrClient::buttonClicked(int button)
{
    if (button == BUTTON_CONTINUE)
        sContinuePlay();
}

void CTblMngrClient::bidValue(Bids bid)
{
    actor->bidValue(bid);
}

void CTblMngrClient::playValue(int card)
{
    actor->playValue(card);
}

void CTblMngrClient::bidBackup()
{
}

void CTblMngrClient::bidHint()
{
}

void CTblMngrClient::bidRestart()
{
}

void CTblMngrClient::bidClose()
{
}

void CTblMngrClient::handClicked(Seat seat)
{
}
