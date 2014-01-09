/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CMidInfoPlay.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Shows information for mid info play widget.

  Platforms: Qt.
*/

/**
 * \file
 * Shows information for mid info play widget (declaration).
 */

#ifndef CMIDINFOPLAY_H
#define CMIDINFOPLAY_H

#include <QGraphicsWidget>

/**
 * @brief Show auction.
 *
 * This class shows in the mid info play widget (play view) the played cards as the play progresses.
 */
class CMidInfoPlay : public QGraphicsWidget
{
public:
    explicit CMidInfoPlay(QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool pressed;
};

#endif // CMIDINFOPLAY_H
