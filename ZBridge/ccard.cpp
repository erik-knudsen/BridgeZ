/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CCard.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Describes and paints one card in the play view.

  Platforms: Qt.

*/

/**
 * \file
 * Describes and paints one card in the play view (definition).
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

void CCard::setHint(bool set)
{
    if (set)
    {
        QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
        effect->setColor(QColor(0, 0, 255));
        setGraphicsEffect(effect);
    }
    else
        setGraphicsEffect(0);
}

void CCard::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (boundingRect().contains(event->pos()))
    {
        QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
        effect->setColor(QColor(0, 255, 0));
        setGraphicsEffect(effect);
    }

//    QGraphicsItem::mousePressEvent(event);
}


/**
 * @brief Generates user events.
 * @param event An event received.
 *
 * The method generates two types of events:
 *   - CARD_CLICKED_Event for when the user selects the card with the left mouse button.
 *   - HAND_CLICKED_Event for when the user selects the card with the right mouse button.
 */
void CCard::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setGraphicsEffect(0);

    if (boundingRect().contains(event->pos()))
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
    }
//        QGraphicsItem::mouseReleaseEvent(event);
}

void CCard::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (boundingRect().contains(event->pos()))
    {
        QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
        effect->setColor(QColor(0, 255, 0));
        setGraphicsEffect(effect);
    }
    else
        setGraphicsEffect(0);
}
