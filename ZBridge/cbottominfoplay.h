/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBottomInfoPlay.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract:  Shows information for bottom info auction widget.

  Platforms: Qt.

*/

/**
 * \file
 *  Shows information for bottom info auction widget (declaration).
 */

#ifndef CBOTTOMINFOPLAY_H
#define CBOTTOMINFOPLAY_H

#include <QGraphicsWidget>

/**
 * @brief The bottom info play class shows play information in the bottom info panel.
 *
 * The class is used in the play view and the information shown is:
 *   - Number of tricks taken by North/South.
 *   - Number of tricks taken by East/West.
 */
class CBottomInfoPlay : public QGraphicsWidget
{
public:
    explicit CBottomInfoPlay(QGraphicsWidget *parent = 0);

    void showNSTricks(int tricks);
    void showEWTricks(int tricks);

private:
    QGraphicsTextItem *nsLabel;
    QGraphicsTextItem *ewLabel;
    QGraphicsTextItem *nsTricks;
    QGraphicsTextItem *ewTricks;
};

#endif // CBOTTOMINFOPLAY_H
