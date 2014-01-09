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
 * Square for vulnerability (declaration).
 */

#ifndef CSQUARE_H
#define CSQUARE_H

#include <QGraphicsRectItem>

/**
 * @brief Helper class for drawing vulnerabilty indication (red square)
 */
class CSquare : public QGraphicsRectItem
{
public:
    explicit CSquare();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
};

#endif // CSQUARE_H
