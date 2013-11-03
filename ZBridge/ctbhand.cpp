#include "ctbhand.h"

CTBHand::CTBHand()
{
}

void CTBHand::prepareToDrawFaces(QGraphicsWidget *widget)
{
    Suit sequence[4];

    setCardFaces();

    switch (trumpSuit)
    {
    case SPADES:
        sequence[0] = SPADES; sequence[1] = HEARTS; sequence[2] = CLUBS; sequence[3] = DIAMONDS;
        break;

    case HEARTS:
        sequence[0] = HEARTS; sequence[1] = SPADES; sequence[2] = DIAMONDS; sequence[3] = CLUBS;
        break;

    case CLUBS:
        sequence[0] = CLUBS; sequence[1] = HEARTS; sequence[2] = SPADES; sequence[3] = DIAMONDS;
        break;

    case DIAMONDS:
        sequence[0] = DIAMONDS; sequence[1] = SPADES; sequence[2] = HEARTS; sequence[3] = CLUBS;
        break;

    default:
        sequence[0] = SPADES; sequence[1] = HEARTS; sequence[2] = CLUBS; sequence[3] = DIAMONDS;
    }

    for (int m = 0, j = 0; m < 4; m++)
    {
        for (int i = 0; i < 13; i++)
        {
            int value = hand[i].getValue();
            if ((value >= SUIT_INTERVAL[sequence[m]][0]) && (value <= SUIT_INTERVAL[sequence[m]][1]))
            {
                hand[i].setPos(m * 50 + (j++) * 30, 0);
                hand[i].setZValue(j);
            }
        }
    }

    setParent(widget);
}

void CTBHand::prepareToDrawBacks(int cardBack, QGraphicsWidget *widget)
{
    int i;

    setCardBacks(cardBack);

    for (i = 0; i < 4; i++)
    {
        hand[i].setPos(i * 30, 0);
        hand[i].setZValue(i);
    }

    for (i = 4; i < 7; i++)
    {
        hand[i].setPos(i * 30 + 50, 0);
        hand[i].setZValue(i);
    }

    for (i = 7; i < 10; i++)
    {
        hand[i].setPos(i * 30 + 100, 0);
        hand[i].setZValue(i);
    }

    for (i = 10; i < 13; i++)
    {
        hand[i].setPos(i * 30 + 150, 0);
        hand[i].setZValue(i);
    }

    setParent(widget);
}
