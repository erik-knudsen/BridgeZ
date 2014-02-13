/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CCenterCards.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: The table in the play view.

  Platforms: Qt.

*/

/**
 * \file
 * The table in the play view (definition).
 */

#include <QApplication>
#include <QGraphicsWidget>
#include <QRectF>
#include <QBrush>
#include <QPainter>
#include <QFont>
#include <QGraphicsSceneMouseEvent>

#include "ccentercards.h"

CCenterCards::CCenterCards(QGraphicsWidget *parent) :
    QGraphicsWidget(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable); 

    //Cards. 
    int delta = 20;
    int yLRPos = (CENTER_VER_SIZE - CARD_VER_SIZE) / 2;
    int xTBPos = (CENTER_HOR_SIZE - CARD_HOR_SIZE) / 2;
    actorCard[LEFT_POS].setPos(delta, yLRPos);
    actorCard[TOP_POS].setPos(xTBPos, delta);
    actorCard[RIGHT_POS].setPos(CENTER_HOR_SIZE - CARD_HOR_SIZE - delta, yLRPos);
    actorCard[BOTTOM_POS].setPos(xTBPos, CENTER_VER_SIZE - CARD_VER_SIZE - delta);

    for (int i = 0; i < 4; i++)
    {
        actorCard[i].setVisible(false);
        actorCard[i].setZValue(10);
        actorCard[i].setParentItem(this);
        actorCard[i].setShowBack(false);
    }

    //Vulnerable.
    int dx, dy;
    delta = 5;
    dx = 2;
    dy = 2 * CARD_VER_SIZE;
    yLRPos = (CENTER_VER_SIZE - dy) / 2;
    xTBPos = (CENTER_HOR_SIZE - dy) / 2;
    actorVulnerable[LEFT_POS].setPos(delta, yLRPos);
    actorVulnerable[TOP_POS].setPos(xTBPos, delta);
    actorVulnerable[RIGHT_POS].setPos(CENTER_HOR_SIZE - dx - delta, yLRPos);
    actorVulnerable[BOTTOM_POS].setPos(xTBPos, CENTER_VER_SIZE - dx - delta);

    actorVulnerable[LEFT_POS].setRect(0, 0, dx, dy);
    actorVulnerable[TOP_POS].setRect(0, 0, dy, dx);
    actorVulnerable[RIGHT_POS].setRect(0, 0, dx, dy);
    actorVulnerable[BOTTOM_POS].setRect(0, 0, dy, dx);

    for (int i = 0; i < 4; i++)
    {
        actorVulnerable[i].setVisible(false);
        actorVulnerable[i].setParentItem(this);
    }

    //Text.
    delta = 20;
    int offset = 20;
    yLRPos = CENTER_VER_SIZE / 2;
    xTBPos = CENTER_HOR_SIZE / 2;
    actorText[LEFT_POS].setPos(delta + CARD_HOR_SIZE / 2 - offset, yLRPos - offset);
    actorText[TOP_POS].setPos(xTBPos - offset, delta + CARD_VER_SIZE / 2 - offset);
    actorText[RIGHT_POS].setPos(CENTER_HOR_SIZE - delta - CARD_HOR_SIZE/2 - offset, yLRPos - offset);
    actorText[BOTTOM_POS].setPos(xTBPos - offset, CENTER_VER_SIZE - delta - CARD_VER_SIZE / 2 - offset);

    QFont font("Times", 30, QFont::Bold);
    for (int i = 0; i < 4; i++)
    {
        actorText[i].setVisible(false);
        actorText[i].setFont(font);
        actorText[i].setZValue(1);
        actorText[i].setParentItem(this);
    }

    buttonSignal = 0;
}

QRectF CCenterCards::boundingRect() const
{
    return QRectF(0, 0, CENTER_HOR_SIZE, CENTER_VER_SIZE);
}

void CCenterCards::showCardOnTable(Position pos, int card)
{
    actorCard[pos].setValue(card);
    actorCard[pos].setVisible(true);
}

void CCenterCards::clearCardOnTable(Position pos)
{
    actorCard[pos].setVisible(false);
}

void CCenterCards::showVulnerable(Position pos)
{
    actorVulnerable[pos].setVisible(true);
}

void CCenterCards::clearVulnerable(Position pos)
{
    actorVulnerable[pos].setVisible(false);
}

void CCenterCards::showText(Position pos, char ewns)
{
    actorText[pos].setText(QString(ewns));
    actorText[pos].setBrush(QBrush(Qt::black));

    if (actorText[pos].isVisible())
        actorText[pos].update();
    else
        actorText[pos].setVisible(true);
}

void CCenterCards::clearText(Position pos)
{
    actorText[pos].setVisible(false);
}

void CCenterCards::showYourTurn(Position pos)
{
    for (int i = 0; i < 4; i++)
    {
        if (i != pos)
            actorText[i].setBrush(QBrush(Qt::black));
        else
            actorText[pos].setBrush(QBrush(Qt::green));

        actorText[i].update();
    }
}

void CCenterCards::clearYourTurn()
{
    for (int i = 0; i < 4; i++)
    {
        actorText[i].setBrush(QBrush(Qt::black));
        actorText[i].update();
    }
}

void CCenterCards::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mousePressEvent(event);
}


void CCenterCards::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        BUTTON_CLICKED_Event *buttonClickedEvent = new BUTTON_CLICKED_Event(BUTTON_LEADER);
        QApplication::postEvent(buttonSignal, buttonClickedEvent);
    }
    else
      QGraphicsWidget::mouseReleaseEvent(event);
}
