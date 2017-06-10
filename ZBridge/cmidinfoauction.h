/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CMidInfoAuction.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Shows information for mid info auction widget.

  Platforms: Qt.

*/

/**
 * \file
 * Shows information for mid info auction widget (declaration).
 */

#ifndef CMIDINFOAUCTION_H
#define CMIDINFOAUCTION_H

#include <QGraphicsWidget>

#include "cbiditem.h"

/**
 * @brief Show auction.
 *
 * This class shows in the mid info auction widget (play view) the bids as the auction progresses.
 */
class CMidInfoAuction : public QGraphicsWidget
{
public:
    explicit CMidInfoAuction(QGraphicsWidget *parent = 0);

    void showBid(Seat seat, Bids bid, QString features = "", QString alert = "");
    void undoBid(int noBid);
    void reset();
    void connectBids(QObject *bidSignalStrategy) {this->bidSignalStrategy = bidSignalStrategy; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QObject *bidSignalStrategy;
    bool pressed;
    QVector<CBidItem *> bidValues;
    QGraphicsTextItem *west;
    QGraphicsTextItem *north;
    QGraphicsTextItem *east;
    QGraphicsTextItem *south;

    int startX, startY, horInc, verInc;
    int line;
};

#endif // CMIDINFOAUCTION_H
