/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBottomInfoButton.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Button for bottom info auction widget.

  Platforms: Qt.

*/

/**
 * \file
 * Button for bottom info auction widget (declaration).
 */

#ifndef CBOTTOMINFOAUCTIONBUTTON_H
#define CBOTTOMINFOAUCTIONBUTTON_H

#include <QGraphicsWidget>

/**
 * @brief Shows a button in the bottom info auction widget in the play view.
 *
 * The button is used as a continue button before the auction starts and as a continue
 * button after the auction and begore the play start.
 */
class CBottomInfoButton : public QGraphicsWidget
{
public:
    explicit CBottomInfoButton(int horSize, int verSize, QString text, QGraphicsWidget *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
     * @brief Connect button events to an event receiver.
     * @param buttonSignal Receiver.
     */
    void connectButton(QObject *buttonSignal) { this->buttonSignal = buttonSignal; }
    /**
     * @brief Set the id of the button.
     * @param buttonId The button id.
     */
    void setButtonId(int buttonId) { this->buttonId = buttonId; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int horSize, verSize;
    int buttonId;
    QString text;
    bool pressed;
    QObject *buttonSignal;
};

#endif // CBOTTOMINFOBUTTON_H
