/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CCard.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: One hand of cards in the play view.

  Platforms: Qt.

*/

/**
 * \file
 * One hand of cards in the play view (declaration).
 */

#ifndef CCARDS_H
#define CCARDS_H

#include <QGraphicsWidget>

#include "ccard.h"
#include "defines.h"

/**
 * @brief One hand of cards.
 *
 * The class holds one hand of cards (13 cards). It has methods for how to show the hand in the play view.
 * It also has support for undoing of play.
 */
class CCards : public QGraphicsWidget
{
public:
    explicit CCards(QGraphicsWidget *parent);

    virtual void prepareToDrawFaces(QGraphicsWidget *widget) = 0;
    virtual void prepareToDrawBacks(QGraphicsWidget *widget) = 0;

    void setCardValue(int cardValue);
    void setBackValues(int cardBack);
    void setShowBack(bool showBack);
    void clearCards();
    void setCardsPosition(Position cardsPosition);
    void showCards(bool show);
    void setTrumpSuit(Suit trumpSuit) { this->trumpSuit = trumpSuit;}
    void connectCards(QObject *cardSignalStrategy);
    void clearCard(int cardValue);
    void setHint(int cardValue, bool set);
    void showClearedCard(int noTrick);
    void showTopClearedCard();
    int getNotVisibleValuesSize() { return notVisibleValues.size(); }
    int getTopCard() { return notVisibleValues.last(); }

protected:
    void setCardValues();
    int getCardIndex(int cardValue);
    void setParent(QGraphicsWidget *widget);

    CCard cards[13];                /**< The hand of 13 cards (sorted). */
    QVector<int> cardsValues;       /**< Cards initially unsorted. */
    QVector<int> notVisibleValues;  /**< Played cards. */
    Suit trumpSuit;                 /**< The trump suit. */
    bool showBack;                  /**< Whether to show fornt or back of cards. */
};

#endif // CCARDS_H
