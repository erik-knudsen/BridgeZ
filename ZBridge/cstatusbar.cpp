/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CStatusBar.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Status bar.

  Platforms: Qt.
*/

/**
 * \file
 * Status bar (definition).
 */

#include <QLabel>

#include "cstatusbar.h"

CStatusBar::CStatusBar(QWidget *parent) :
    QStatusBar(parent)
{
    contract = new QLabel(this);
    contract->setFixedWidth(30);
    declarer = new QLabel(this);
    declarer->setFixedWidth(30);
    vulnerable = new QLabel(this);
    vulnerable->setFixedWidth(30);
    mode = new QLabel(this);
    mode->setFixedWidth(30);

    addPermanentWidget(mode);
    addPermanentWidget(vulnerable);
    addPermanentWidget(declarer);
    addPermanentWidget(contract);
}
