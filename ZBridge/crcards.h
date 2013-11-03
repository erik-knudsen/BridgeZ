#ifndef CRCARDS_H
#define CRCARDS_H

#include <QGraphicsWidget>

#include "crlhand.h"

class CRCards : public QGraphicsWidget
{
public:
    explicit CRCards(QGraphicsWidget *parent = 0);

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

    CRLHand hand;
};

#endif // CRCARDS_H
