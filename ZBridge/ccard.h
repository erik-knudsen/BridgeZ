#ifndef CCARD_H
#define CCARD_H

/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CCard.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Cards.

  Platforms: Qt.

*/

#include <QGraphicsItem>

#include "defines.h"

/**
 * \file
 * Cards (declaration).
 */

class CCard : public QGraphicsItem
{
public:
    CCard();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void setValue(int value) { this->value = value; }
    int getValue() { return value; }
    void setBackValue(int backValue) { this->backValue = backValue; }
    void setShowBack(bool showBack) { this->showBack = showBack; }
    void setCardPosition(Position cardPosition) { this->cardPosition = cardPosition; }
    Position getCardPosition() { return cardPosition; }
    void connectCard(QObject *cardSignal) { this->cardSignal = cardSignal; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Position cardPosition;
    int value, backValue;
    bool showBack;
    QObject *cardSignal;
};

#endif // CCARD_H
