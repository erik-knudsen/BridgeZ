/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CCenterCards.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: The table in the play view.

  Platforms: Qt.

*/

/**
 * \file
 * The table in the play view (declaration).
 */

#ifndef CCENTERCARDS_H
#define CCENTERCARDS_H

#include <QGraphicsWidget>
#include <QGraphicsSimpleTextItem>

#include "Defines.h"
#include "ccard.h"
#include "csquare.h"

/**
 * @brief This class shows the table in the play view.
 *
 * Shows the cards trick by trick as the play progresses. It also shows vulnerability and who is to
 * play next.
 */
class CCenterCards : public QGraphicsWidget
{
public:
    explicit CCenterCards(QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;

    void connectButton(QObject *buttonSignal) { this->buttonSignal = buttonSignal; }

    void showCardOnTable(Position pos, int card);
    void clearCardOnTable(Position pos);
    void showVulnerable(Position pos);
    void clearVulnerable(Position pos);
    void showText(Position pos, char ewns);
    void clearText(Position pos);
    void showYourTurn(Position pos);
    void clearYourTurn();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QObject *buttonSignal;

    CCard actorCard[4];
    CSquare actorVulnerable[4];
    QGraphicsSimpleTextItem actorText[4];
};

#endif // CCENTERCARDS_H
