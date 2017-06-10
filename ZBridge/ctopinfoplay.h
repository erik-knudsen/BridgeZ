/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTopInfoPlay.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Shows information for top info play widget.

  Platforms: Qt.
*/

/**
 * \file
 * Shows information for top info play widget (declaration).
 */

#ifndef CTOPINFOPLAY_H
#define CTOPINFOPLAY_H

#include <QGraphicsWidget>

#include "defines.h"

/**
 * @brief Show relevant info for play.
 *
 * This class shows in the top info play widget (play view) information relevant for the play:
 *   - Board id.
 *   - Vulnerability.
 *   - Dealer.
 *   - Declarer.
 *   - Contract.
 */
class CTopInfoPlay : public QGraphicsWidget
{
public:
    explicit CTopInfoPlay(QGraphicsWidget *parent = 0);

    void setBoardId(QString board);
    void setVulnerability(Team team);
    void setDealer(Seat seat);
    void setDeclarer(Seat seat);
    void setContract(Bids bid, Bids dbl);

private:
    QGraphicsTextItem *boardIdLabel;
    QGraphicsTextItem *vulnerabelLabel;
    QGraphicsTextItem *dealerLabel;
    QGraphicsTextItem *declarerLabel;
    QGraphicsTextItem *contractLabel;
    QGraphicsTextItem *vulnerabel;
    QGraphicsTextItem *boardId;
    QGraphicsTextItem *dealer;
    QGraphicsTextItem *declarer;
    QGraphicsPixmapItem *contract;
    QGraphicsPixmapItem *contract_dbl;
};

#endif // CTOPINFOPLAY_H
