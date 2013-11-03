#include <QGraphicsWidget>
#include <QRectF>
#include <QBrush>
#include <QPainter>

#include "ctcards.h"
#include "defines.h"

CTCards::CTCards(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    hand.setHandPosition(TOP_POS);

    pressed = true;
}

QRectF CTCards::boundingRect() const
{
    return QRectF(0, 0, TB_CARD_HOR_SIZE, TB_CARD_VER_SIZE);
}

void CTCards::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void CTCards::setFace(int face)
{
    hand.setFace(face);
}

void CTCards::clearCards()
{
    hand.clearCards();
}

void CTCards::showFaces()
{
    hand.prepareToDrawFaces(this);
    update();
}

void CTCards::showBacks(int cardBack)
{
    hand.prepareToDrawBacks(cardBack, this);
    update();
}

void CTCards::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsWidget::mousePressEvent(event);
}


void CTCards::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsWidget::mouseReleaseEvent(event);
}
