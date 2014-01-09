/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBottomInfo.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Bottom part of info panel.

  Platforms: Qt.

*/

/**
 * \file
 * Bottom part of info panel (definition).
 */

#include <QRectF>
#include <QBrush>
#include <QPainter>

#include "defines.h"
#include "cbottominfo.h"

CBottomInfo::CBottomInfo(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    pressed = true;
}

QRectF CBottomInfo::boundingRect() const
{
    return QRectF(0, 0, INF_HOR_SIZE, TB_CARD_VER_SIZE);
}

void CBottomInfo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void CBottomInfo::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsWidget::mousePressEvent(event);
}


void CBottomInfo::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsWidget::mouseReleaseEvent(event);
}
