/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidItem.cpp
  Developers: eak

  Revision History:
  5-sep-2013 eak: Original

  Abstract: Bids.

  Platforms: Qt.

*/

/**
 * \file
 * Bids (definition).
 */

#include <QApplication>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "CBidItem.h"
#include "Defines.h"

CBidItem::CBidItem(Seat seat, Bids bid)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    bidSignal = 0;

    setSeat(seat);
    setBid(bid);
}

QRectF CBidItem::boundingRect() const
{
    return QRectF(0, 0, BID_HOR_SIZE, BID_VER_SIZE);
}

void CBidItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap;
    QString bidRef = BIDS[bid];
    pixmap.load(bidRef);
    painter->drawPixmap(0, 0, pixmap);
}

void CBidItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}


void CBidItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        BID_CLICKED_Event *bidClickedEvent = new BID_CLICKED_Event(seat, bid);
        QApplication::postEvent(bidSignal, bidClickedEvent);
    }
    else
      QGraphicsItem::mouseReleaseEvent(event);
}
