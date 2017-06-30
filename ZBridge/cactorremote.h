/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CActorRemote.h
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Actor remote class.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the actor remote class.
 */

#ifndef CACTORREMOTE_H
#define CACTORREMOTE_H

#include <QObject>

#include "Defines.h"
#include "cactor.h"

class CRemoteActorFrontEnd;
class CTblMngr;

/**
 * @brief This class implements the actor remote class.
 *
 * The class implements the server part of a remote actor (player). The class is controlled from the
 * table management server and communicates via a remote actor front end with the client part of the
 * remote actor. Communication is by means of tcp/ip in the remote actor front end. The client part
 * is on another computer, where  it is implemented as a actor local class.\n
 * The class packs/unpacks the messages exchanged via tcp/ip, and reacts in the proper way on the
 * messages.
 */
class CActorRemote : public CActor
{
    Q_OBJECT
public:
    CActorRemote(Seat seat, CRemoteActorFrontEnd *frontEnd, CTblMngr *tableManager);

    void xmitPBNFiles(QTextStream &originalStream, QTextStream &playedStream, ScoringMethod scoringMethod);

    void startNewSession();

    void seated(QString teamName);
    void teamNames(QString nsTeamName, QString ewTeamName);
    void startOfBoard();
    void dealInfo(int boardNumber, Seat dealer, Team vulnerability);
    void cards(int cards[4][13]);
    void bidDone(Seat bidder, Bids bid);
    void playerToLead(Seat player);
    void dummyToLead();
    void playerPlays(Seat player, int card);
    void dummyCards(int cards[13]);
    void undoBid(bool reBid);
    void undoTrick(bool rePlay);
    void endOfSession();

    void attemptSyncFromServerToClient();
    void confirmSyncFromServerToClient();
    void allSyncFromServerToClient();

    Actor getActorType() { return REMOTE_ACTOR; }
    Seat getSeat() { return seat; }

public slots:
    void receiveLine(QString line, bool connected);     /**< Receive a line from the client. */

signals:
    void start();
    void sendLine(QString line);        /**< Send a line  to the client. */

private:
    Seat seat;
    CRemoteActorFrontEnd *frontEnd;     /**< The front end. */
    CTblMngr * tableManager;            /**< The table manager. */
};

#endif // CACTORREMOTE_H
