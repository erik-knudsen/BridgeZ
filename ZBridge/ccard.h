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
 * Describes and paints one card in the play view (declaration).
 */

#ifndef CCARD_H
#define CCARD_H

#include <QGraphicsItem>
#include <QGraphicsColorizeEffect>

#include "defines.h"

/**
 * @brief This class describes one card in the play view.
 *
 * The description includes:
 *   - The value of the card.
 *   - The back value of the card (how to show the back side of the card).
 *   - Whether to show the back or the front of the card in the view.
 *   - The position of the card (West, North, East or South).
 *   - The receiver of emitted events.
 */
class CCard : public QGraphicsItem
{
public:
    CCard();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void setValue(int value) { this->value = value; }
    void setHint(bool set);
    int getValue() { return value; }
    void setBackValue(int backValue) { this->backValue = backValue; }
    void setShowBack(bool showBack) { this->showBack = showBack; }
    void setCardPosition(Position cardPosition) { this->cardPosition = cardPosition; }
    Position getCardPosition() { return cardPosition; }
    void connectCard(QObject *cardSignal) { this->cardSignal = cardSignal; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    Position cardPosition;
    int value, backValue;
    bool showBack;
    QObject *cardSignal;
    qreal zVal;
};

#endif // CCARD_H
