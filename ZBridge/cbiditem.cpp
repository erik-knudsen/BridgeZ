/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidItem.cpp
  Developers: eak

  Revision History:
  5-sep-2013 eak: Original

  Abstract: Description of one bid (to be used in the play view)..

  Platforms: Qt.

*/

/**
 * \file
 * Description of one bid (definition).
 */

#include <QApplication>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "CBidItem.h"
#include "Defines.h"

/**
 * @brief Constructor creates the bid.
 * @param seat The seat who gave the bid.
 * @param bid The bid.
 */
CBidItem::CBidItem(Seat seat, Bids bid)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    bidSignal = 0;
    setAcceptHoverEvents(true);

    setSeat(seat);
    setBid(bid);
}

QRectF CBidItem::boundingRect() const
{
    return QRectF(0, 0, BID_HOR_SIZE, BID_VER_SIZE);
}

/**
 * @brief Paint the bid.
 */
void CBidItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap;
    QString bidRef = BIDS[bid];
    pixmap.load(bidRef);
    painter->drawPixmap(0, 0, pixmap);
}

void CBidItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (alert.size() > 0)
        popup.showPopup(alert + "\n" + features);
    else if (features.size() > 0)
        popup.showPopup(features);

    update();
}

void CBidItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    popup.close();

    update();
}

void CBidItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}


/**
 * @brief Mouse release event for the bid is sent to the event receiver.
 * @param event The event.
 *
 * In case of a left button release event the bidClickedEvent is sent to the event receiver.
 */
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
