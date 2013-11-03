#include <QRectF>
#include <QBrush>
#include <QPainter>

#include "defines.h"
#include "cmidinfoplay.h"

CMidInfoPlay::CMidInfoPlay(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    pressed = true;
}

QRectF CMidInfoPlay::boundingRect() const
{
    return QRectF(0, 0, INF_HOR_SIZE, LR_CARD_VER_SIZE);
}

void CMidInfoPlay::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
/*    QRectF rec = boundingRect();
    QBrush brush(Qt::green);
    if (pressed)
    {
        brush.setColor(Qt::darkGray);
    }
    else
    {
        brush.setColor(Qt::gray);
    }
    painter->fillRect(rec, brush);
    painter->drawRect(rec);

    QGraphicsWidget::paint(painter, option, widget);
*/
}

void CMidInfoPlay::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsWidget::mousePressEvent(event);
}


void CMidInfoPlay::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsWidget::mouseReleaseEvent(event);
}
