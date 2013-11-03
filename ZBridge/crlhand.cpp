#include "crlhand.h"

CRLHand::CRLHand()
{
}

void CRLHand::prepareToDrawFaces(QGraphicsWidget *widget)
{
    int i, j, count, dist, value;
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

    for (int m = 0, k = 0; m < 4; m++)
    {
        for (i = 0, count = 0; i < 13; i++)
        {
            value = hand[i].getValue();
            if ((value >= SUIT_INTERVAL[sequence[m]][0]) && (value <= SUIT_INTERVAL[sequence[m]][1]))
                count++;
        }
        dist = (count < 7) ? (29) : ((count < 10) ? (19) : (14));
        for (i = 0, j = 0; i < 13; i++)
        {
            value = hand[i].getValue();
            if ((value >= SUIT_INTERVAL[sequence[m]][0]) && (value <= SUIT_INTERVAL[sequence[m]][1]))
            {
                hand[i].setPos((j++) * dist, m * TB_CARD_VER_SIZE);
                hand[i].setZValue(k++);
            }
        }
    }

    setParent(widget);
}

void CRLHand::prepareToDrawBacks(int cardBack, QGraphicsWidget *widget)
{
    int i, k;

    setCardBacks(cardBack);

    for (i = 0, k = 0; i < 4; i++)
    {
        hand[i].setPos((k++) * 30, 0 * TB_CARD_VER_SIZE);
        hand[i].setZValue(i);
    }

    for (i = 4, k = 0; i < 7; i++)
    {
        hand[i].setPos((k++) * 30, 1 * TB_CARD_VER_SIZE);
        hand[i].setZValue(i);
    }

    for (i = 7, k = 0; i < 10; i++)
    {
        hand[i].setPos((k++) * 30, 2 * TB_CARD_VER_SIZE);
        hand[i].setZValue(i);
    }

    for (i = 10, k = 0; i < 13; i++)
    {
        hand[i].setPos((k++) * 30, 3 * TB_CARD_VER_SIZE);
        hand[i].setZValue(i);
    }

    setParent(widget);
}
