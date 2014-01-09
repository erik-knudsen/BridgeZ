/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTopInfo.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Top part of info panel.

  Platforms: Qt.
*/

/**
 * \file
 * Top part of info panel (declaration).
 */

#ifndef CTOPINFO_H
#define CTOPINFO_H

#include <QGraphicsWidget>

/**
 * @brief Top info class.
 *
 *The Top info class is used by the play view to show, depending on context, one of:
 *   - Relevant info for auction (shown during bidding).
 *   - Relevant info for play (shown during play).
 */
class CTopInfo : public QGraphicsWidget
{
public:
    explicit CTopInfo(QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool pressed;
};

#endif // CTOPINFO_H
