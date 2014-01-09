/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CLRCards.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Describes one hand of cards in the left or right position in the play view.

  Platforms: Qt.

*/

/**
 * \file
 * Describes one hand of cards in the left or right position in the play view (declaration).
 */

#ifndef CLRCARDS_H
#define CLRCARDS_H

#include <QGraphicsWidget>

#include "ccards.h"
#include "Defines.h"

/**
 * @brief One hand of cards in the left or right position in the play view.
 *
 * This class determines the layout for how to draw the cards.
 */
class CLRCards : public CCards
{
public:
    explicit CLRCards(Position pos, QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;

    void prepareToDrawFaces(QGraphicsWidget *widget);
    void prepareToDrawBacks(QGraphicsWidget *widget);
};

#endif // CLRCARDS_H
