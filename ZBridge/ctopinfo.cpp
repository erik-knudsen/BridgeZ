#include <QRectF>
#include <QBrush>
#include <QPainter>

#include "defines.h"
#include "ctopinfo.h"

CTopInfo::CTopInfo(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    pressed = true;
}

QRectF CTopInfo::boundingRect() const
{
    return QRectF(0, 0, INF_HOR_SIZE, TB_CARD_VER_SIZE);
}

void CTopInfo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
*/
    QGraphicsWidget::paint(painter, option, widget);
}

void CTopInfo::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    pressed = false;
//    update();
    QGraphicsWidget::mousePressEvent(event);
}


void CTopInfo::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    pressed = true;
//    update();
    QGraphicsWidget::mouseReleaseEvent(event);
}
