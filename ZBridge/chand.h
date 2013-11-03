#ifndef CHAND_H
#define CHAND_H

/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CHand.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Hand.

  Platforms: Qt.

*/

#include "ccard.h"
#include "defines.h"

/**
 * \file
 * Hand (declaration).
 */

class CHand
{
public:
    CHand();

    void setFace(int face);
    void clearCards();
    void setTrumpSuit(Suit trumpSuit) { this->trumpSuit = trumpSuit; }
    void setHandPosition(Position handPosition);
    void connectCards(QObject *cardSignalStrategy);
    void showFace(int faceValue, bool visible);
    void showBack(int backIndex, bool visible);

protected:
    void setCardFaces();
    void setCardBacks(int cardBack);
    void setParent(QGraphicsWidget *widget);
    int getCardIndex(int faceValue);

    CCard hand[13];
    QVector<int> handFaceValues;
    Position handPosition;
    Suit trumpSuit;
};

#endif // CHAND_H
