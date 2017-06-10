/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CMidInfoAuction.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Shows information for mid info auction widget.

  Platforms: Qt.

*/

/**
 * \file
 * Shows information for mid info auction widget (definition).
 */

#include <QRectF>
#include <QBrush>
#include <QPainter>

#include "defines.h"
#include "cmidinfoauction.h"

CMidInfoAuction::CMidInfoAuction(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    bidSignalStrategy = 0;

    horInc = 2.3 * BID_HOR_SIZE;
    verInc = 1.5 * BID_VER_SIZE;
    startX = (INF_HOR_SIZE - horInc * 4) / 2;
    startY = 40;

    line = 0;

    QFont font("Times New Roman", 10, QFont::Bold);

    west = new QGraphicsTextItem(tr("W"), this);
    west->setPos(startX - 2 + 0 * horInc, 15);
    west->setFont(font);
    west->setDefaultTextColor(Qt::darkRed);

    north = new QGraphicsTextItem(tr("N"), this);
    north->setPos(startX - 2 + 1 * horInc, 15);
    north->setFont(font);
    north->setDefaultTextColor(Qt::darkRed);

    east = new QGraphicsTextItem(tr("E"), this);
    east->setPos(startX - 2 + 2 * horInc, 15);
    east->setFont(font);
    east->setDefaultTextColor(Qt::darkRed);

    south = new QGraphicsTextItem(tr("S"), this);
    south->setPos(startX - 2 + 3 * horInc, 15);
    south->setFont(font);
    south->setDefaultTextColor(Qt::darkRed);

    pressed = true;
}

/**
 * @brief Show the next bid.
 *
 * It is assumed that bids are shown in the sequence they are given. The bid BID_PLAYER indicates
 * the next bidder and must be shown before the next bid is shown.
 *
 * @param seat The seat.
 * @param bid The bid.
 * @param features Text string describing rules for the bid - might be 0.
 * @param alert Text string describing alert (empty if no alert) - might be 0.
 */
void CMidInfoAuction::showBid(Seat seat, Bids bid, QString features, QString alert)
{
    int size = bidValues.size();
    CBidItem *last = 0;
    if (size > 0) last = bidValues.last();

    if (((bid == BID_PLAYER) && (size == 0)) ||
        ((bid == BID_PLAYER) && (size > 0) && (last->getBid() != BID_PLAYER) && (((last->getSeat() + 1) & 3) == seat)))
    {
        last = new CBidItem(seat, bid);
        bidValues.append(last);
        last->connectBid(bidSignalStrategy);
        last->setParentItem(this);
        if ((seat == WEST_SEAT) && (size > 0))
            line++;
        last->setPos(startX + seat * horInc, startY + line * verInc);
    }
    else if ((bid != BID_PLAYER) &&(size > 0) && (last->getBid() == BID_PLAYER) && (last->getSeat() == seat))
    {
        last->setSeat(seat);
        last->setBid(bid);
        last->setFeatures(features);
        last->setAlert(alert);
    }
    if (last != 0) last->update();
}

void CMidInfoAuction::undoBid(int noBid)
{
    Seat nextBidder;

    if (noBid == REBID)
        while (!bidValues.empty())
        {
            nextBidder = bidValues.last()->getSeat();
            delete bidValues.last();
            bidValues.removeLast();
        }
    else
        while (bidValues.size() > (noBid + 1))
        {
            nextBidder = bidValues.last()->getSeat();
            delete bidValues.last();
            bidValues.removeLast();
        }

    line = 0;
    for (int i = 1; i < bidValues.size(); i++)
        if (bidValues.at(i)->getSeat() == WEST_SEAT)
            line++;

    showBid(nextBidder, BID_PLAYER);
}

void CMidInfoAuction::reset()
{
    for (int i = 0; i < bidValues.size(); i++)
        delete bidValues[i];

    bidValues.clear();

    line = 0;
}

QRectF CMidInfoAuction::boundingRect() const
{
    return QRectF(0, 0, INF_HOR_SIZE, LR_CARD_VER_SIZE);
}

void CMidInfoAuction::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
/*    QRectF rec = boundingRect();
    QBrush brush(Qt::green);
    if (pressed)
    {
        brush.setColor(Qt::darkGray);
    }
    else
    {
        brush.setColor(Qt::gray);
    }
    painter->fillRect(rec, brush);
    painter->drawRect(rec);

    QGraphicsWidget::paint(painter, option, widget);
*/
}

void CMidInfoAuction::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsWidget::mousePressEvent(event);
}


void CMidInfoAuction::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsWidget::mouseReleaseEvent(event);
}
