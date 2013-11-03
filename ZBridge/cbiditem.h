#ifndef CBIDITEM_H
#define CBIDITEM_H

/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidItem.h
  Developers: eak

  Revision History:
  5-sep-2013 eak: Original

  Abstract: Bids.

  Platforms: Qt.

*/

#include <QGraphicsItem>

#include "defines.h"

/**
 * \file
 * Bids (declaration).
 */

class CBidItem : public QGraphicsItem
{
public:
    explicit CBidItem(Seat seat, Bids bid);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void setSeat(Seat seat) { this->seat = seat; }
    Seat getSeat() { return seat; }
    void setBid(Bids bid) { this->bid = bid; }
    Bids getBid() { return bid; }
    void connectBid(QObject *bidSignal) { this->bidSignal = bidSignal; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Seat seat;
    Bids bid;
    QObject *bidSignal;
};

#endif // CBIDITEM_H
