#ifndef CMIDINFOAUCTION_H
#define CMIDINFOAUCTION_H

#include <QGraphicsWidget>

#include "cbiditem.h"

class CMidInfoAuction : public QGraphicsWidget
{
public:
    explicit CMidInfoAuction(QGraphicsWidget *parent = 0);

    void showBid(Seat seat, Bids bid);
    void reset();
    void connectBids(QObject *bidSignalStrategy) {this->bidSignalStrategy = bidSignalStrategy; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QObject *bidSignalStrategy;
    bool pressed;
    QVector<CBidItem *> bidValues;
    QGraphicsSimpleTextItem *west;
    QGraphicsSimpleTextItem *north;
    QGraphicsSimpleTextItem *east;
    QGraphicsSimpleTextItem *south;

    int startX, startY, horInc, verInc;
    int line;
};

#endif // CMIDINFOAUCTION_H
