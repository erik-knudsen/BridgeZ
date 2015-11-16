/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CTopInfoPlay.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Shows information for top info play widget.

  Platforms: Qt.
*/

/**
 * \file
 * Shows information for top info play widget (definition).
 */

#include "Defines.h"
#include "ctopinfoplay.h"

CTopInfoPlay::CTopInfoPlay(QGraphicsWidget *parent) :
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

    declarerLabel = new QGraphicsSimpleTextItem(tr("Declarer: "), this);
    declarerLabel->setPos(0, 60);
    declarerLabel->setFont(font);
    declarerLabel->setBrush(brush);

    declarer = new QGraphicsSimpleTextItem(this);
    declarer->setPos(80, 60);
    declarer->setFont(font);
    declarer->setBrush(brush);

    contractLabel = new QGraphicsSimpleTextItem(tr("Contract: "), this);
    contractLabel->setPos(0, 80);
    contractLabel->setFont(font);
    contractLabel->setBrush(brush);

    contract = new QGraphicsPixmapItem(this);
    contract->setPos(80, 80);

    contract_dbl = new QGraphicsPixmapItem(this);
    contract_dbl->setPos(100, 80);
}

void CTopInfoPlay::setBoardId(QString board)
{
    boardId->setText(board);
}

void CTopInfoPlay::setVulnerability(Team team)
{
    vulnerabel->setText(tr(TEAM_NAMES[team]));
}

void CTopInfoPlay::setDealer(Seat seat)
{
    dealer->setText(tr(SEAT_NAMES[seat]));
}

void CTopInfoPlay::setDeclarer(Seat seat)
{
    if (seat != NO_SEAT)
        declarer->setText(tr(SEAT_NAMES[seat]));
}

void CTopInfoPlay::setContract(Bids bid, Bids dbl)
{
    if (bid != BID_NONE)
    {
        contract->setPixmap(BIDS[bid]);
        if (dbl != BID_NONE)
            contract_dbl->setPixmap(BIDS[dbl]);
        else
            contract_dbl->setPixmap(BIDS[BID_BLANK]);
    }
}
