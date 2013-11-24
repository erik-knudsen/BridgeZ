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
    lCard.setVisible(false);
    tCard.setVisible(false);
    rCard.setVisible(false);
    bCard.setVisible(false);

    int delta = 20;
    int yLRPos = (CENTER_VER_SIZE - CARD_VER_SIZE) / 2;
    int xTBPos = (CENTER_HOR_SIZE - CARD_HOR_SIZE) / 2;

    lCard.setPos(delta, yLRPos);
    tCard.setPos(xTBPos, delta);
    rCard.setPos(CENTER_HOR_SIZE - CARD_HOR_SIZE - delta, yLRPos);
    bCard.setPos(xTBPos, CENTER_VER_SIZE - CARD_VER_SIZE - delta);

    lCard.setZValue(10);
    tCard.setZValue(10);
    rCard.setZValue(10);
    bCard.setZValue(10);

    lCard.setParentItem(this);
    tCard.setParentItem(this);
    rCard.setParentItem(this);
    bCard.setParentItem(this);

    lCard.setShowBack(false);
    tCard.setShowBack(false);
    rCard.setShowBack(false);
    bCard.setShowBack(false);

    //Vulnerable.
    lVulnerable.setVisible(false);
    tVulnerable.setVisible(false);
    rVulnerable.setVisible(false);
    bVulnerable.setVisible(false);

    int dx, dy;
    delta = 5;
    dx = 2;
    dy = 2 * CARD_VER_SIZE;
    yLRPos = (CENTER_VER_SIZE - dy) / 2;
    xTBPos = (CENTER_HOR_SIZE - dy) / 2;
    lVulnerable.setPos(delta, yLRPos);
    tVulnerable.setPos(xTBPos, delta);
    rVulnerable.setPos(CENTER_HOR_SIZE - dx - delta, yLRPos);
    bVulnerable.setPos(xTBPos, CENTER_VER_SIZE - dx - delta);

    lVulnerable.setRect(0, 0, dx, dy);
    tVulnerable.setRect(0, 0, dy, dx);
    rVulnerable.setRect(0, 0, dx, dy);
    bVulnerable.setRect(0, 0, dy, dx);

    lVulnerable.setParentItem(this);
    tVulnerable.setParentItem(this);
    rVulnerable.setParentItem(this);
    bVulnerable.setParentItem(this);

    //Text.
    lText.setVisible(false);
    tText.setVisible(false);
    rText.setVisible(false);
    bText.setVisible(false);

    QFont font("Times", 30, QFont::Bold);
    lText.setFont(font);
    tText.setFont(font);
    rText.setFont(font);
    bText.setFont(font);

    delta = 20;
    int offset = 20;
    yLRPos = CENTER_VER_SIZE / 2;
    xTBPos = CENTER_HOR_SIZE / 2;
    lText.setPos(delta + CARD_HOR_SIZE / 2 - offset, yLRPos - offset);
    tText.setPos(xTBPos - offset, delta + CARD_VER_SIZE / 2 - offset);
    rText.setPos(CENTER_HOR_SIZE - delta - CARD_HOR_SIZE/2 - offset, yLRPos - offset);
    bText.setPos(xTBPos - offset, CENTER_VER_SIZE - delta - CARD_VER_SIZE / 2 - offset);

    lText.setZValue(1);
    tText.setZValue(1);
    rText.setZValue(1);
    bText.setZValue(1);

    lText.setParentItem(this);
    tText.setParentItem(this);
    rText.setParentItem(this);
    bText.setParentItem(this);

    buttonSignal = 0;
}

QRectF CCenterCards::boundingRect() const
{
    return QRectF(0, 0, CENTER_HOR_SIZE, CENTER_VER_SIZE);
}

void CCenterCards::showCardOnTable(Position pos, int card)
{
    if (pos == LEFT_POS)
    {
        lCard.setValue(card);
        lCard.setVisible(true);
    }
    else if (pos == TOP_POS)
    {
        tCard.setValue(card);
        tCard.setVisible(true);
    }
    else if (pos == RIGHT_POS)
    {
        rCard.setValue(card);
        rCard.setVisible(true);
    }
    else if (pos == BOTTOM_POS)
    {
        bCard.setValue(card);
        bCard.setVisible(true);
    }
}

void CCenterCards::clearCardOnTable(Position pos)
{
    if (pos == LEFT_POS)
        lCard.setVisible(false);
    else if (pos == TOP_POS)
        tCard.setVisible(false);
    else if (pos == RIGHT_POS)
        rCard.setVisible(false);
    else if (pos == BOTTOM_POS)
        bCard.setVisible(false);
}

void CCenterCards::showVulnerable(Position pos)
{
    if (pos == LEFT_POS)
        lVulnerable.setVisible(true);
    else if (pos == TOP_POS)
        tVulnerable.setVisible(true);
    else if (pos == RIGHT_POS)
        rVulnerable.setVisible(true);
    else if (pos == BOTTOM_POS)
        bVulnerable.setVisible(true);
}

void CCenterCards::clearVulnerable(Position pos)
{
    if (pos == LEFT_POS)
        lVulnerable.setVisible(false);
    else if (pos == TOP_POS)
        tVulnerable.setVisible(false);
    else if (pos == RIGHT_POS)
        rVulnerable.setVisible(false);
    else if (pos == BOTTOM_POS)
        bVulnerable.setVisible(false);
}

void CCenterCards::showText(Position pos, char ewns)
{
    if (pos == LEFT_POS)
    {
        lText.setText(QString(ewns));
        lText.setBrush(QBrush(Qt::black));

        if (lText.isVisible())
            lText.update();
        else
            lText.setVisible(true);
    }
    else if (pos == TOP_POS)
    {
        tText.setText(QString(ewns));
        tText.setBrush(QBrush(Qt::black));

        if (tText.isVisible())
            tText.update();
        else
            tText.setVisible(true);
    }
    else if (pos == RIGHT_POS)
    {
        rText.setText(QString(ewns));
        rText.setBrush(QBrush(Qt::black));

        if (rText.isVisible())
            rText.update();
        else
            rText.setVisible(true);
    }
    else if (pos == BOTTOM_POS)
    {
        bText.setText(QString(ewns));
        bText.setBrush(QBrush(Qt::black));

        if (bText.isVisible())
            bText.update();
        else
            bText.setVisible(true);
    }
}

void CCenterCards::clearText(Position pos)
{
    if (pos == LEFT_POS)
         lText.setVisible(false);
    else if (pos == TOP_POS)
        tText.setVisible(false);
    else if (pos == RIGHT_POS)
        rText.setVisible(false);
    else if (pos == BOTTOM_POS)
        bText.setVisible(false);
}

void CCenterCards::showYourTurn(Position pos)
{
    lText.setBrush(QBrush(Qt::black));
    tText.setBrush(QBrush(Qt::black));
    rText.setBrush(QBrush(Qt::black));
    bText.setBrush(QBrush(Qt::black));

    if (pos == LEFT_POS)
        lText.setBrush(QBrush(Qt::green));
    else if (pos == TOP_POS)
        tText.setBrush(QBrush(Qt::green));
   else if (pos == RIGHT_POS)
        rText.setBrush(QBrush(Qt::green));
    else if (pos == BOTTOM_POS)
        bText.setBrush(QBrush(Qt::green));

    lText.update();
    tText.update();
    rText.update();
    bText.update();
}

void CCenterCards::clearYourTurn()
{
    lText.setBrush(QBrush(Qt::black));
    tText.setBrush(QBrush(Qt::black));
    rText.setBrush(QBrush(Qt::black));
    bText.setBrush(QBrush(Qt::black));

    lText.update();
    tText.update();
    rText.update();
    bText.update();
}

void CCenterCards::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsWidget::mousePressEvent(event);
}


void CCenterCards::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        BUTTON_CLICKED_Event *buttonClickedEvent = new BUTTON_CLICKED_Event(BUTTON_CONTINUE);
        QApplication::postEvent(buttonSignal, buttonClickedEvent);
    }
    else
      QGraphicsWidget::mouseReleaseEvent(event);
}
