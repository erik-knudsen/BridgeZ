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

#include <QCoreApplication>
//#include <QFontDatabase>

#include "Defines.h"
#include "ctopinfoplay.h"

CTopInfoPlay::CTopInfoPlay(QGraphicsWidget *parent) :
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

    declarerLabel = new QGraphicsTextItem(tr("Declarer: "), this);
    declarerLabel->setPos(0, 60);
    declarerLabel->setFont(font);
    declarerLabel->setDefaultTextColor(Qt::darkRed);

    declarer = new QGraphicsTextItem(this);
    declarer->setPos(90, 60);
    declarer->setFont(font);
    declarer->setDefaultTextColor(Qt::darkRed);

    contractLabel = new QGraphicsTextItem(tr("Contract: "), this);
    contractLabel->setPos(0, 80);
    contractLabel->setFont(font);
    contractLabel->setDefaultTextColor(Qt::darkRed);

    contract = new QGraphicsPixmapItem(this);
    contract->setPos(93, 88);

    contract_dbl = new QGraphicsPixmapItem(this);
    contract_dbl->setPos(113, 88);
}

void CTopInfoPlay::setBoardId(QString board)
{
    boardId->setPlainText(board);
}

void CTopInfoPlay::setVulnerability(Team team)
{
    vulnerabel->setPlainText(QCoreApplication::translate("defines", TEAM_NAMES[team]));
}

void CTopInfoPlay::setDealer(Seat seat)
{
    dealer->setPlainText(QCoreApplication::translate("defines", SEAT_NAMES[seat]));
}

void CTopInfoPlay::setDeclarer(Seat seat)
{
    if (seat != NO_SEAT)
        declarer->setPlainText(QCoreApplication::translate("defines", SEAT_NAMES[seat]));
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
