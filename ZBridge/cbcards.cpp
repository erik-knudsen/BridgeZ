#include <QGraphicsWidget>
#include <QRectF>
#include <QBrush>
#include <QPainter>

#include "cbcards.h"
#include "defines.h"

CBCards::CBCards(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    hand.setHandPosition(BOTTOM_POS);

    pressed = true;
}

QRectF CBCards::boundingRect() const
{
    return QRectF(0, 0, TB_CARD_HOR_SIZE, TB_CARD_VER_SIZE);
}

void CBCards::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
/*    QRectF rec = boundingRect();
    QBrush brush(Qt::red);
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

void CBCards::setFace(int face)
{
    hand.setFace(face);
}

void CBCards::clearCards()
{
    hand.clearCards();
}

void CBCards::showFaces()
{
    hand.prepareToDrawFaces(this);
    update();
}

void CBCards::showBacks(int cardBack)
{
    hand.prepareToDrawBacks(cardBack, this);
    update();
}

void CBCards::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsWidget::mousePressEvent(event);
}


void CBCards::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsWidget::mouseReleaseEvent(event);
}
