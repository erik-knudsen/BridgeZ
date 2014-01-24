/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CLRCards.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Describes one hand of cards in the left or right position in the play view.

  Platforms: Qt.

*/

/**
 * \file
 * Describes one hand of cards in the left or right position in the play view (definition).
 */

#include <cassert>

#include "clrcards.h"

CLRCards::CLRCards(Position pos, QGraphicsWidget *parent) :
    CCards(parent)
{
    assert((pos == LEFT_POS) || (pos == RIGHT_POS));

    setCardsPosition(pos);
}

QRectF CLRCards::boundingRect() const
{
    return QRectF(0, 0, LR_CARD_HOR_SIZE, LR_CARD_VER_SIZE);
}

/**
 * @brief Layout how to draw faces in the left and right positions.
 */
void CLRCards::prepareToDrawFaces(QGraphicsWidget *widget)
{
    int i, j, count, dist, value;
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

    for (int m = 0, k = 0; m < 4; m++)
    {
        for (i = 0, count = 0; i < 13; i++)
        {
            value = cards[i].getValue();
            if ((value >= SUIT_INTERVAL[sequence[m]][0]) && (value <= SUIT_INTERVAL[sequence[m]][1]))
                count++;
        }
        dist = (count < 7) ? (29) : ((count < 10) ? (19) : (14));
        for (i = 0, j = 0; i < 13; i++)
        {
            value = cards[i].getValue();
            if ((value >= SUIT_INTERVAL[sequence[m]][0]) && (value <= SUIT_INTERVAL[sequence[m]][1]))
            {
                cards[i].setPos((j++) * dist, m * TB_CARD_VER_SIZE);
                cards[i].setZValue(k++);
                cards[i].setVisible(!notVisibleValues.contains(value));
            }
        }
    }

    setParent(widget);
}

/**
 * @brief Layout how to draw back of cards in the left and right positions..
 */
void CLRCards::prepareToDrawBacks(QGraphicsWidget *widget)
{
    int i, k;

    for (i = 0, k = 0; i < 4; i++)
    {
        cards[i].setPos((k++) * 30, 0 * TB_CARD_VER_SIZE);
        cards[i].setZValue(i);
    }

    for (i = 4, k = 0; i < 7; i++)
    {
        cards[i].setPos((k++) * 30, 1 * TB_CARD_VER_SIZE);
        cards[i].setZValue(i);
    }

    for (i = 7, k = 0; i < 10; i++)
    {
        cards[i].setPos((k++) * 30, 2 * TB_CARD_VER_SIZE);
        cards[i].setZValue(i);
    }

    for (i = 10, k = 0; i < 13; i++)
    {
        cards[i].setPos((k++) * 30, 3 * TB_CARD_VER_SIZE);
        cards[i].setZValue(i);
    }

    int noInvisible = notVisibleValues.size();
    for (i = 0; i < 13; i++)
        cards[i].setVisible(i >= noInvisible);

    setParent(widget);
}
