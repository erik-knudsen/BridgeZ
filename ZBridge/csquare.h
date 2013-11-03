#ifndef CSQUARE_H
#define CSQUARE_H

#include <QGraphicsRectItem>

class CSquare : public QGraphicsRectItem
{
public:
    explicit CSquare();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
};

#endif // CSQUARE_H
