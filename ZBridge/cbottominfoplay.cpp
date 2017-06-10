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
    nsLabel = new QGraphicsTextItem(tr("N-S"), this);
    nsLabel->setPos(25, 0);
    nsLabel->setFont(QFont("Times New Roman", 15, QFont::Bold));
    nsLabel->setDefaultTextColor(Qt::darkRed);

    ewLabel = new QGraphicsTextItem(tr("E-W"), this);
    ewLabel->setPos(75, 0);
    ewLabel->setFont(QFont("Times New Roman", 15, QFont::Bold));
    ewLabel->setDefaultTextColor(Qt::darkRed);

    nsTricks = new QGraphicsTextItem("8", this);
    nsTricks->setPos(43, 30);
    nsTricks->setFont(QFont("Times New Roman", 15, QFont::Bold));
    nsTricks->setDefaultTextColor(Qt::darkRed);

    ewTricks = new QGraphicsTextItem("4", this);
    ewTricks->setPos(93, 30);
    ewTricks->setFont(QFont("Times New Roman", 15, QFont::Bold));
    ewTricks->setDefaultTextColor(Qt::darkRed);
}

void CBottomInfoPlay::showNSTricks(int tricks)
{
    QString txt;
    txt.setNum(tricks);
    nsTricks->setPlainText(txt);
}

void CBottomInfoPlay::showEWTricks(int tricks)
{
    QString txt;
    txt.setNum(tricks);
    ewTricks->setPlainText(txt);
}
