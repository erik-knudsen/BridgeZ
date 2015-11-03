/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CTblMngrBase.h
  Developers: eak

  Revision History:
  9-oct-2015 eak: Original

  Abstract: Generic base strategy for table manager.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the generic base strategy for table manager
 * strategy classes (server, clients and auto).
 */

#ifndef CTBLMNGRBASE_H
#define CTBLMNGRBASE_H

#include <QObject>

#include "Defines.h"

/**
 * \brief This abstract class implements the declaration of the generic base strategy
 * for table manager classes (server, clients and auto play).
 *
 * Table manager strategy classes have a common method interface. The interface is
 * defined partly by means of virtual methods in this class. And partly by means of
 * methods, which are common for all table managers (Server, Client and auto play). These
 * methods are implemented in this class.
 */
class CTblMngrBase : public QObject
{
    Q_OBJECT

public:
    CTblMngrBase(QObject *parent);
    virtual ~CTblMngrBase();

    /** @name Main menu activated methods.
     * This method is activated via the applications main menu.
     */
    /*@{*/
    virtual void newSession(){}
    /*@}*/

signals:
    /** @name Synchronization and status text signal.
     *
     */
    /*@{*/
    void sigPlayStart();            /**< Used for synchronizing (server) ordinary and auto play. */
    void sigDisconnect();           /**< Used for relaying disconnect (client) ordinary and auto play. */
    void sStatusText(QString text); /**< Used for displaying a status text. */
    /*@}*/

public slots:   
    /** @name Actor slots.
     * These slot methods are signalled from the actors.
     */
    /*@{*/
    virtual void sConnect(QString name, Seat seat, int protocol) = 0;
    virtual void sRTNames(Seat seat) = 0;
    virtual void sRSBoard(Seat seat) = 0;
    virtual void sRDealInfo(Seat seat) = 0;
    virtual void sRCards(Seat seat) = 0;
    virtual void sRBid(Seat player, Seat bidder) = 0;
    virtual void sBid(Seat player, Bids bid) = 0;
    virtual void sPlayerPlays(Seat player, int card) = 0;
    virtual void sReadyForPlayer(Seat seat, Seat player, int trick) = 0;
    virtual void sReadyForDummy(Seat seat, int trick) = 0;
    virtual void sReadyForDummyCards(Seat seat) = 0;
    virtual void sAttemptSyncFromClientToServer(Seat syncher) = 0;
    virtual void sConfirmSyncFromClientToServer(Seat syncher) = 0;
    virtual void sUpdateGame() = 0;
    virtual void sUpdateGameToNextDeal() {}
    /*@}*/

    /** @name Synchronization, auto quit and new session slots.
     */
    /*@{*/
    virtual void sltPlayStart() {}    /**< Synchronize (server) auto and ordinary play. */
    virtual void sltDisconnect() {}   /**< Relay disconnect (client) auto and  ordinary play. */
    virtual void sAutoQuit() {}       /**< Quit the auto thread and delete the auto object (and children). */
    virtual void sNewSession() {}     /**< Start a new auto session. */
    /*@}*/

protected:
    QString teamNames[4];
    bool playContinue;           //Used for synchronizing auto play and ordinary play.
    bool playWaiting;           //Also used for synchronizing auto play and ordinary play.
};

#endif // CTBLMNGRBASE_H
