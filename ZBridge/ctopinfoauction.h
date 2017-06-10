/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTopInfoAuction.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Shows information for top info auction widget.

  Platforms: Qt.
*/

/**
 * \file
 * Shows information for top info auction widget (declaration).
 */

#ifndef CTOPINFOAUCTION_H
#define CTOPINFOAUCTION_H

#include <QGraphicsWidget>

#include "defines.h"

/**
 * @brief Show relevant info for auction.
 *
 * This class shows in the top info auction widget (play view) information relevant for the auction:
 *   - Board id.
 *   - Vulnerability.
 *   - Dealer.
 */
class CTopInfoAuction : public QGraphicsWidget
{
public:
    explicit CTopInfoAuction(QGraphicsWidget *parent = 0);

    void setBoardId(QString board);
    void setVulnerability(Team team);
    void setDealer(Seat seat);

private:
    QGraphicsTextItem *boardIdLabel;
    QGraphicsTextItem *vulnerabelLabel;
    QGraphicsTextItem *dealerLabel;
    QGraphicsTextItem *boardId;
    QGraphicsTextItem *vulnerabel;
    QGraphicsTextItem *dealer;
};

#endif // CTOPINFOAUCTION_H
