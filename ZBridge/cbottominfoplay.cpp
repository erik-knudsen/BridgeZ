/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBottomInfoPlay.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract:  Shows information for bottom info auction widget.

  Platforms: Qt.

*/

/**
 * \file
 *  Shows information for bottom info auction widget (definition).
 */

#include "defines.h"
#include "cbottominfoplay.h"

CBottomInfoPlay::CBottomInfoPlay(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    nsLabel = new QGraphicsSimpleTextItem(tr("N-S"), this);
    nsLabel->setPos(25, 0);
    nsLabel->setFont(QFont("Times", 15, QFont::Bold));
    nsLabel->setBrush(QBrush(Qt::darkRed));

    ewLabel = new QGraphicsSimpleTextItem(tr("E-W"), this);
    ewLabel->setPos(75, 0);
    ewLabel->setFont(QFont("Times", 15, QFont::Bold));
    ewLabel->setBrush(QBrush(Qt::darkRed));

    nsTricks = new QGraphicsSimpleTextItem("8", this);
    nsTricks->setPos(43, 30);
    nsTricks->setFont(QFont("Times", 15, QFont::Bold));
    nsTricks->setBrush(QBrush(Qt::darkRed));

    ewTricks = new QGraphicsSimpleTextItem("4", this);
    ewTricks->setPos(93, 30);
    ewTricks->setFont(QFont("Times", 15, QFont::Bold));
    ewTricks->setBrush(QBrush(Qt::darkRed));
}

void CBottomInfoPlay::showNSTricks(int tricks)
{
    QString txt;
    txt.setNum(tricks);
    nsTricks->setText(txt);
}

void CBottomInfoPlay::showEWTricks(int tricks)
{
    QString txt;
    txt.setNum(tricks);
    ewTricks->setText(txt);
}
