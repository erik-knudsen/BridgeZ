/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CCard.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Cards.

  Platforms: Qt.

*/

/**
 * \file
 * Cards (definition).
 */

#include <QApplication>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "ccard.h"
#include "Defines.h"

CCard::CCard()
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    cardSignal = 0;

    value = -1;
    backValue = 0;
    showBack = true;
}

QRectF CCard::boundingRect() const
{
    return QRectF(0, 0, CARD_HOR_SIZE, CARD_VER_SIZE);
}

void CCard::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap;
    QString card = ((value >= 0) && (value <= 51) && !showBack) ? (CARD_FACES[value]) : (CARD_BACK_NAME[backValue]);
    pixmap.load(card);
    painter->drawPixmap(0, 0, pixmap);
}

void CCard::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}


void CCard::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        CARD_CLICKED_Event *cardClickedEvent = new CARD_CLICKED_Event(cardPosition, value);
        QApplication::postEvent(cardSignal, cardClickedEvent);
    }
    else if (event->button() == Qt::RightButton)
    {
        HAND_CLICKED_Event *handClickedEvent = new HAND_CLICKED_Event(cardPosition);
        QApplication::postEvent(cardSignal, handClickedEvent);
    }
    else
      QGraphicsItem::mouseReleaseEvent(event);
}
