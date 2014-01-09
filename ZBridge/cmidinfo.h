/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CMidInfo.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Midle part of info panel.

  Platforms: Qt.

*/

/**
 * \file
 * Midle part of info panel (declaration).
 */

#ifndef CMIDINFO_H
#define CMIDINFO_H

#include <QGraphicsWidget>

/**
 * @brief Mid info class.
 *
 *The Mid info class is used by the play view to show, depending on context, one of:
 *   - Bids as auction progresses.
 *   - Played cards as play progresses.
 */
class CMidInfo : public QGraphicsWidget
{
public:
    explicit CMidInfo(QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool pressed;
};

#endif // CMIDINFO_H
