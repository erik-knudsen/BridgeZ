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

#include <QGraphicsSimpleTextItem>

#include "ctopinfoauction.h"

CTopInfoAuction::CTopInfoAuction(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    //Create widgets.
    QFont font("Times", 10, QFont::Bold);
    QBrush brush(Qt::darkRed);

    boardIdLabel = new QGraphicsSimpleTextItem(tr("Board Id: "), this);
    boardIdLabel->setPos(0, 0);
    boardIdLabel->setFont(font);
    boardIdLabel->setBrush(brush);

    boardId = new QGraphicsSimpleTextItem(this);
    boardId->setPos(80, 0);
    boardId->setFont(font);
    boardId->setBrush(brush);

    vulnerabelLabel = new QGraphicsSimpleTextItem(tr("Vulnerable: "), this);
    vulnerabelLabel->setPos(0,20);
    vulnerabelLabel->setFont(font);
    vulnerabelLabel->setBrush(brush);

    vulnerabel = new QGraphicsSimpleTextItem(this);
    vulnerabel->setPos(80, 20);
    vulnerabel->setFont(font);
    vulnerabel->setBrush(brush);

    dealerLabel = new QGraphicsSimpleTextItem(tr("Dealer: "), this);
    dealerLabel->setPos(0, 40);
    dealerLabel->setFont(font);
    dealerLabel->setBrush(brush);

    dealer = new QGraphicsSimpleTextItem(this);
    dealer->setPos(80, 40);
    dealer->setFont(font);
    dealer->setBrush(brush);
}

void CTopInfoAuction::setBoardId(QString board)
{
    boardId->setText(board);
}

void CTopInfoAuction::setVulnerability(Team team)
{
    if (team == NORTH_SOUTH)
        vulnerabel->setText(tr("North-South"));
    else if (team == EAST_WEST)
        vulnerabel->setText(tr("East-West"));
    else if (team == BOTH)
        vulnerabel->setText(tr("All"));
    else
        vulnerabel->setText(tr("Neither"));
}

void CTopInfoAuction::setDealer(Seat seat)
{
    if (seat == NORTH_SEAT)
        dealer->setText(tr("North"));
    else if (seat == EAST_SEAT)
        dealer->setText(tr("East"));
    else if (seat == SOUTH_SEAT)
        dealer->setText(tr("South"));
    else
        dealer->setText(tr("West"));
}
