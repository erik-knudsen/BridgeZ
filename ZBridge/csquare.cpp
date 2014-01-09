/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CSquare.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Square for vulnerability.

  Platforms: Qt.
*/

/**
 * \file
 * Square for vulnerability (definition).
 */

#include <QPainter>

#include "csquare.h"

CSquare::CSquare()
{
}

//Paint a red square in the bounding rectangle.
void CSquare::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::red);
    painter->fillRect(rec, brush);
    painter->drawRect(rec);

    QGraphicsRectItem::paint(painter, option, widget);
}
