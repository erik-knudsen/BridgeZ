/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTBCards.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Describes one hand of cards in the top or bottom position in the play view.

  Platforms: Qt.

*/

/**
 * \file
 * Describes one hand of cards in the top or bottom position in the play view (declaration).
 */

#ifndef CTBCARDS_H
#define CTBCARDS_H

#include <QGraphicsWidget>

#include "ccards.h"
#include "Defines.h"

/**
 * @brief One hand of cards in the top or bottom position in the play view.
 *
 * This class determines the layout for how to draw the cards.
 */
class CTBCards : public CCards
{
public:
    explicit CTBCards(Position pos, QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;

    void prepareToDrawFaces(QGraphicsWidget *widget);
    void prepareToDrawBacks(QGraphicsWidget *widget);
};

#endif // CTBCARDS_H
