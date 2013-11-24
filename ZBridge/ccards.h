#ifndef CCARDS_H
#define CCARDS_H

#include <QGraphicsWidget>

#include "ccard.h"
#include "defines.h"

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
    void showClearedCard(int noCard);

protected:
    void setCardValues();
    int getCardIndex(int cardValue);
    void setParent(QGraphicsWidget *widget);

    CCard cards[13];
    QVector<int> cardsValues;
    QVector<int> notVisibleValues;
    Suit trumpSuit;
    bool showBack;
};

#endif // CCARDS_H
