/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBottomInfoAuction.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Shows information for bottom info auction widget.

  Platforms: Qt.

*/

/**
 * \file
 * Shows information for bottom info auction widget (declaration).
 */

#ifndef CBOTTOMINFOAUCTION_H
#define CBOTTOMINFOAUCTION_H

#include <QGraphicsWidget>

/**
 * @brief Show information for bottom info auction widget in play view
 *
 * There is no information to show at present.
 */
class CBottomInfoAuction : public QGraphicsWidget
{
public:
    explicit CBottomInfoAuction(QGraphicsWidget *parent = 0);
};

#endif // CBOTTOMINFOAUCTION_H
