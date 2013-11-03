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
    if (team == NORTH_SOUTH)
        vulnerabel->setText(tr("North-South"));
    else if (team == EAST_WEST)
        vulnerabel->setText(tr("East-West"));
    else
        vulnerabel->setText(tr("None"));
}

void CTopInfoPlay::setDealer(Seat seat)
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

void CTopInfoPlay::setDeclarer(Seat seat)
{
    if (seat == NORTH_SEAT)
        declarer->setText(tr("North"));
    else if (seat == EAST_SEAT)
        declarer->setText(tr("East"));
    else if (seat == SOUTH_SEAT)
        declarer->setText(tr("South"));
    else
        declarer->setText(tr("West"));
}

void CTopInfoPlay::setContract(Bids bid, Bids dbl)
{
    contract->setPixmap(BIDS[bid]);
    if (dbl != BID_NONE)
        contract_dbl->setPixmap(BIDS[dbl]);
    else
        contract_dbl->setPixmap(BIDS[BID_BLANK]);
}
