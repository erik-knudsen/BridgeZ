/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CMidInfo.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Midle part of info panel.

  Platforms: Qt.

*/

/**
 * \file
 * Midle part of info panel (definition).
 */

#include <QGraphicsWidget>
#include <QRectF>
#include <QBrush>
#include <QPainter>

#include "cmidinfo.h"
#include "defines.h"

CMidInfo::CMidInfo(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    pressed = true;
}

QRectF CMidInfo::boundingRect() const
{
    return QRectF(0, 0, INF_HOR_SIZE, INF_VER_SIZE);
}

void CMidInfo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    QGraphicsWidget::paint(painter, option, widget);
*/
}

void CMidInfo::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsWidget::mousePressEvent(event);
}


void CMidInfo::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsWidget::mouseReleaseEvent(event);
}
