/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTBCards.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Describes one hand of cards in the top or bottom position in the play view.

  Platforms: Qt.

*/

/**
 * \file
 * Describes one hand of cards in the top or bottom position in the play view (definition).
 */

#include <cassert>

#include "ctbcards.h"

CTBCards::CTBCards(Position pos, QGraphicsWidget * parent) :
    CCards(parent)
{
    assert((pos == TOP_POS) || (pos == BOTTOM_POS));

    setCardsPosition(pos);
}

QRectF CTBCards::boundingRect() const
{
    return QRectF(0, 0, TB_CARD_HOR_SIZE, TB_CARD_VER_SIZE);
}

/**
 * @brief Layout card faces to be drawn in top and bottom positions.
 */
void CTBCards::prepareToDrawFaces(QGraphicsWidget *widget)
{
    Suit sequence[4];

    setCardValues();

    switch (trumpSuit)
    {
    case SPADES:
        sequence[0] = SPADES; sequence[1] = HEARTS; sequence[2] = DIAMONDS; sequence[3] = CLUBS;
        break;

    case HEARTS:
        sequence[0] = HEARTS; sequence[1] = SPADES; sequence[2] = DIAMONDS; sequence[3] = CLUBS;
        break;

    case CLUBS:
        sequence[0] = CLUBS; sequence[1] = SPADES; sequence[2] = HEARTS; sequence[3] = DIAMONDS;
        break;

    case DIAMONDS:
        sequence[0] = DIAMONDS; sequence[1] = SPADES; sequence[2] = HEARTS; sequence[3] = CLUBS;
        break;

    default:
        sequence[0] = SPADES; sequence[1] = HEARTS; sequence[2] = DIAMONDS; sequence[3] = CLUBS;
    }

    for (int m = 0, j = 0; m < 4; m++)
    {
        for (int i = 0; i < 13; i++)
        {
            int value = cards[i].getValue();
            if ((value >= SUIT_INTERVAL[sequence[m]][0]) && (value <= SUIT_INTERVAL[sequence[m]][1]))
            {
                cards[i].setPos(m * 50 + (j++) * 30, 0);
                cards[i].setZValue(j);
                cards[i].setVisible(!notVisibleValues.contains(value));
            }
        }
    }

    setParent(widget);
}

/**
 * @brief Layout card backs to be drawn in the top and bottom positions.
 */
void CTBCards::prepareToDrawBacks(QGraphicsWidget *widget)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        cards[i].setPos(i * 30, 0);
        cards[i].setZValue(i);
    }

    for (i = 4; i < 7; i++)
    {
        cards[i].setPos(i * 30 + 50, 0);
        cards[i].setZValue(i);
    }

    for (i = 7; i < 10; i++)
    {
        cards[i].setPos(i * 30 + 100, 0);
        cards[i].setZValue(i);
    }

    for (i = 10; i < 13; i++)
    {
        cards[i].setPos(i * 30 + 150, 0);
        cards[i].setZValue(i);
    }

    int noInvisible = notVisibleValues.size();
    for (i = 0; i < 13; i++)
        cards[i].setVisible(i >= noInvisible);

    setParent(widget);
}
