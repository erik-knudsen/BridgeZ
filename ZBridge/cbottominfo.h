/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBottomInfo.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Bottom part of info panel.

  Platforms: Qt.

*/

/**
 * \file
 * Bottom part of info panel (declaration).
 */

#ifndef CBOTTOMINFO_H
#define CBOTTOMINFO_H

#include <QGraphicsWidget>

/**
 * @brief Bottom info class.
 *
 *The bottom info class is used by the play view to show, depending on context, one of:
 *   - Auction related information.
 *   - Auction button.
 *   - Play related information.
 *   - Play button.
 */
class CBottomInfo : public QGraphicsWidget
{
public:
    explicit CBottomInfo(QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool pressed;
};

#endif // CBOTTOMINFO_H
