#include <QPainter>

#include "csquare.h"

CSquare::CSquare()
{
}

void CSquare::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::red);
    painter->fillRect(rec, brush);
    painter->drawRect(rec);

    QGraphicsRectItem::paint(painter, option, widget);
}
