#include <QGraphicsWidget>
#include <QRectF>
#include <QBrush>
#include <QPainter>

#include "crcards.h"
#include "defines.h"

CRCards::CRCards(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    hand.setHandPosition(RIGHT_POS);

    pressed = true;
}

QRectF CRCards::boundingRect() const
{
    return QRectF(0, 0, LR_CARD_HOR_SIZE, LR_CARD_VER_SIZE);
}

void CRCards::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
/*    QRectF rec = boundingRect();
    QBrush brush(Qt::gray);
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

void CRCards::setFace(int face)
{
    hand.setFace(face);
}

void CRCards::clearCards()
{
    hand.clearCards();
}

void CRCards::showFaces()
{
    hand.prepareToDrawFaces(this);
    update();
}

void CRCards::showBacks(int cardBack)
{
    hand.prepareToDrawBacks(cardBack, this);
    update();
}

void CRCards::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsWidget::mousePressEvent(event);
}


void CRCards::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsWidget::mouseReleaseEvent(event);
}
