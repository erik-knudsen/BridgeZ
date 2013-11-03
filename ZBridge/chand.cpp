/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CHand.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Hand.

  Platforms: Qt.

*/

/**
 * \file
 * Hand (definition).
 */

#include <QGraphicsWidget>

#include "chand.h"
#include "ccard.h"

CHand::CHand()
{
    trumpSuit = ANY;
}

void CHand::setFace(int face)
{
    handFaceValues.append(face);
}

void CHand::clearCards()
{
    setParent(0);
    handFaceValues.clear();
}

void CHand::setHandPosition(Position handPosition)
{
    for (int i = 0; i < 13; i++)
        hand[i].setCardPosition(handPosition);
}

void CHand::connectCards(QObject *cardSignalStrategy)
{
    for (int i = 0; i < 13; i++)
        hand[i].connectCard(cardSignalStrategy);
}

void CHand::showFace(int faceValue, bool visible)
{
    int i;
    i = getCardIndex(faceValue);
    if (i < 13)
        hand[i].setVisible(visible);
}

void CHand::showBack(int backIndex, bool visible)
{
    if ((backIndex < 13) && (backIndex >= 0))
        hand[backIndex].setVisible(visible);
}

void CHand::setCardFaces()
{
    qSort(handFaceValues.begin(), handFaceValues.end(), qGreater<int>());
    for (int i = 0; i < 13; i++)
        hand[i].setValue(handFaceValues.at(i));
}

void CHand::setCardBacks(int cardBack)
{
    for (int i = 0; i < 13; i++)
        hand[i].setValue(cardBack + 100);
}

void CHand::setParent(QGraphicsWidget *widget)
{
    for (int i = 0; i < 13; i++)
    {
        hand[i].setVisible(widget != 0);
        hand[i].setParentItem(widget);
    }
}

int CHand::getCardIndex(int faceValue)
{
    int i;

    for (i = 0; i < 13; i++)
        if (hand[i].getValue() == faceValue)
            break;

    return i;
}
