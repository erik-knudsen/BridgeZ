/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CStatusBar.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Status bar.

  Platforms: Qt.
*/

/**
 * \file
 * Status bar (declaration).
 */

#ifndef CSTATUSBAR_H
#define CSTATUSBAR_H

#include <QStatusBar>

class QLabel;

/**
 * @brief Control of status bar.
 *
 *   - Contract.
 *   - Declarer.
 *   - Vulnerability indication.
 *   - Mode.
 */
class CStatusBar : public QStatusBar
{
    Q_OBJECT
public:
    explicit CStatusBar(QWidget *parent = 0);

    QLabel *contract;
    QLabel *declarer;
    QLabel *vulnerable;
    QLabel *mode;

signals:
    
public slots:
    
};

#endif // CSTATUSBAR_H
