#ifndef CBCARDS_H
#define CBCARDS_H

#include <QGraphicsWidget>

#include "ctbhand.h"

class CBCards : public QGraphicsWidget
{
public:
    explicit CBCards(QGraphicsWidget *parent = 0);
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setFace(int face);
    void clearCards();
    void showFaces();
    void showBacks(int cardBack);
    void setTrumpSuit(Suit trumpSuit) { hand.setTrumpSuit(trumpSuit);}
    void connectCards(QObject *cardSignalStrategy) { hand.connectCards(cardSignalStrategy); }
    void showFace(int faceValue, bool visible) { hand.showFace(faceValue, visible); }
    void showBack(int backIndex, bool visible) { hand.showBack(backIndex, visible); }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool pressed;

    CTBHand hand;
};

#endif // CBCARDS_H
