#include "ccards.h"

CCards::CCards(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    trumpSuit = ANY;

    clearCards();
}

void CCards::setCardValue(int cardValue)
{
    cardsValues.append(cardValue);
}

void CCards::setBackValues(int cardBack)
{
    for (int i = 0; i < 13; i++)
        cards[i].setBackValue(cardBack);
}

void CCards::setCardValues()
{
    qSort(cardsValues.begin(), cardsValues.end(), qGreater<int>());
    for (int i = 0; i < 13; i++)
        cards[i].setValue(cardsValues.at(i));
}

int CCards::getCardIndex(int cardValue)
{
    int i;

    for (i = 0; i < 13; i++)
        if (cards[i].getValue() == cardValue)
            break;

    return i;
}

void CCards::setShowBack(bool showBack)
{
    this->showBack = showBack;

    for (int i = 0; i < 13; i++)
        cards[i].setShowBack(showBack);
}

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

void CCards::setCardsPosition(Position cardsPosition)
{
    for (int i = 0; i < 13; i++)
        cards[i].setCardPosition(cardsPosition);
}

void CCards::showCards(bool show)
{
    setParent(0);
    if (!show)
        return;

    if (showBack)
        prepareToDrawBacks(this);
    else
        prepareToDrawFaces(this);

    update();
}

void CCards::connectCards(QObject *cardSignalStrategy)
{
    for (int i = 0; i < 13; i++)
        cards[i].connectCard(cardSignalStrategy);
}

void CCards::clearCard(int cardValue)
{
    int i = showBack ? notVisibleValues.size() : getCardIndex(cardValue);

    if (i < 13)
    {
        cards[i].setVisible(false);
        notVisibleValues.append(cardValue);
    }
}

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

void CCards::setParent(QGraphicsWidget *widget)
{
    for (int i = 0; i < 13; i++)
    {
        cards[i].setVisible(widget != 0);
        cards[i].setParentItem(widget);
    }
}
