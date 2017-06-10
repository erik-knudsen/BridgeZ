/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTopInfoAuction.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Shows information for top info auction widget.

  Platforms: Qt.
*/

/**
 * \file
 * Shows information for top info auction widget (definition).
 */

#include <QCoreApplication>
#include <QGraphicsSimpleTextItem>

#include "Defines.h"
#include "ctopinfoauction.h"

CTopInfoAuction::CTopInfoAuction(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    //Create widgets.
    QFont font("Times New Roman", 10, QFont::Bold);

    boardIdLabel = new QGraphicsTextItem(tr("Board Id: "), this);
    boardIdLabel->setPos(0, 0);
    boardIdLabel->setFont(font);
    boardIdLabel->setDefaultTextColor(Qt::darkRed);

    boardId = new QGraphicsTextItem(this);
    boardId->setPos(90, 0);
    boardId->setFont(font);
    boardId->setDefaultTextColor(Qt::darkRed);

    vulnerabelLabel = new QGraphicsTextItem(tr("Vulnerable: "), this);
    vulnerabelLabel->setPos(0,20);
    vulnerabelLabel->setFont(font);
    vulnerabelLabel->setDefaultTextColor(Qt::darkRed);

    vulnerabel = new QGraphicsTextItem(this);
    vulnerabel->setPos(90, 20);
    vulnerabel->setFont(font);
    vulnerabel->setDefaultTextColor(Qt::darkRed);

    dealerLabel = new QGraphicsTextItem(tr("Dealer: "), this);
    dealerLabel->setPos(0, 40);
    dealerLabel->setFont(font);
    dealerLabel->setDefaultTextColor(Qt::darkRed);

    dealer = new QGraphicsTextItem(this);
    dealer->setPos(90, 40);
    dealer->setFont(font);
    dealer->setDefaultTextColor(Qt::darkRed);
}

void CTopInfoAuction::setBoardId(QString board)
{
    boardId->setPlainText(board);
}

void CTopInfoAuction::setVulnerability(Team team)
{
    vulnerabel->setPlainText(QCoreApplication::translate("defines", TEAM_NAMES[team]));
}

void CTopInfoAuction::setDealer(Seat seat)
{
    dealer->setPlainText(QCoreApplication::translate("defines", SEAT_NAMES[seat]));
}
