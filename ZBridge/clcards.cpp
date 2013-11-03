#include <QGraphicsWidget>
#include <QRectF>
#include <QBrush>
#include <QPainter>

#include "clcards.h"
#include "defines.h"

CLCards::CLCards(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    hand.setHandPosition(LEFT_POS);

    pressed = true;
}

QRectF CLCards::boundingRect() const
{
    return QRectF(0, 0, LR_CARD_HOR_SIZE, LR_CARD_VER_SIZE);
}

void CLCards::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
/*    QRectF rec = boundingRect();
    QBrush brush(Qt::blue);
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

void CLCards::setFace(int face)
{
    hand.setFace(face);
}

void CLCards::clearCards()
{
    hand.clearCards();
}

void CLCards::showFaces()
{
    hand.prepareToDrawFaces(this);
    update();
}

void CLCards::showBacks(int cardBack)
{
    hand.prepareToDrawBacks(cardBack, this);
    update();
}

void CLCards::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsWidget::mousePressEvent(event);
}


void CLCards::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsWidget::mouseReleaseEvent(event);
}
