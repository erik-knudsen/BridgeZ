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
 * One hand of cards in the play view (definition).
 */

#include "ccards.h"

CCards::CCards(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    trumpSuit = ANY;

    clearCards();
}

/**
 * @brief Cards are set one value at a  time.
 * @param cardValue The value to set for the next card.
 */
void CCards::setCardValue(int cardValue)
{
    cardsValues.append(cardValue);
}

/**
 * @brief Card back values are set for all cards simultaneously.
 * @param cardBack The card back.
 */
void CCards::setBackValues(int cardBack)
{
    for (int i = 0; i < 13; i++)
        cards[i].setBackValue(cardBack);
}

/**
 * @brief Sort card values after all have been set.
 */
void CCards::setCardValues()
{
    qSort(cardsValues.begin(), cardsValues.end(), qGreater<int>());
    for (int i = 0; i < 13; i++)
        cards[i].setValue(cardsValues.at(i));
}

/**
 * @brief Get index in array of cards for a given card.
 * @param cardValue The card.
 * @return The index in the array.
 */
int CCards::getCardIndex(int cardValue)
{
    int i;

    for (i = 0; i < 13; i++)
        if (cards[i].getValue() == cardValue)
            break;

    return i;
}

/**
 * @brief Informs whether to show back or front of cards..
 * @param showBack If true then show back else show front.
 */
void CCards::setShowBack(bool showBack)
{
    this->showBack = showBack;

    for (int i = 0; i < 13; i++)
        cards[i].setShowBack(showBack);
}

/**
 * @brief Initialize the class.
 */
void CCards::clearCards()
{
    setParent(0);
    cardsValues.clear();
    notVisibleValues.clear();

    trumpSuit = ANY;

    for (int i = 0; i < 13; i++)
    {
        cards[i].setValue(-1);
        cards[i].setBackValue(0);
        cards[i].setShowBack(true);
    }
}

/**
 * @brief Set the position for the cards.
 * @param cardsPosition The position (West, North, East or South).
 */
void CCards::setCardsPosition(Position cardsPosition)
{
    for (int i = 0; i < 13; i++)
        cards[i].setCardPosition(cardsPosition);
}

/**
 * @brief Show the cards in the proper place in the play view.
 * @param show If true then just remove the cards from the view.
 */
void CCards::showCards(bool show)
{
    setParent(0);
    if (!show)
        return;

    //Virtual call to show cards properly for all hands.
    if (showBack)
        prepareToDrawBacks(this);
    else
        prepareToDrawFaces(this);

    update();
}

/**
 * @brief Set event receiver.
 * @param cardSignalStrategy The receiver.
 */
void CCards::connectCards(QObject *cardSignalStrategy)
{
    for (int i = 0; i < 13; i++)
        cards[i].connectCard(cardSignalStrategy);
}

/**
 * @brief Clear one card from the view (it has been played).
 * @param cardValue The card to clear.
 *
 * Also administration for being able to undo play of card.
 */
void CCards::clearCard(int cardValue)
{
    int i = showBack ? notVisibleValues.size() : getCardIndex(cardValue);

    if (i < 13)
    {
        cards[i].setVisible(false);
        notVisibleValues.append(cardValue);
    }
}

/**
 * @brief Show played card(s) again after having been cleared
 * @param noCard Number of cards to show (undo).
 */
void CCards::showClearedCard(int noCard)
{
    for (int k = 0; k < noCard; k++)
        if (!notVisibleValues.isEmpty())
        {
            int cardValue = notVisibleValues.last();
            int i = showBack ? notVisibleValues.size() : getCardIndex(cardValue);
            if ( i < 13)
            {
                cards[i].setVisible(true);
                notVisibleValues.pop_back();
            }
        }
}

/**
 * @brief Set the parent of all cards widgets.
 * @param widget If 0 then clear the card.
 */
void CCards::setParent(QGraphicsWidget *widget)
{
    for (int i = 0; i < 13; i++)
    {
        cards[i].setVisible(widget != 0);
        cards[i].setParentItem(widget);
    }
}
