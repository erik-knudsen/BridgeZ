/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CPlayDialog.h
  Developers: eak

  Revision History:
  4-feb-2015 eak: Original

  Abstract: Show play for a given board.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of a class for showing the play
 * (auction and play) for one board.
 */

#ifndef CPLAYDIALOG_H
#define CPLAYDIALOG_H

#include <QDialog>
#include <QResizeEvent>

#include "cbidhistory.h"
#include "cplayhistory.h"
#include "cplayshow.h"

class CGamesDoc;
class CPlayView;
class CPlayShow;

namespace Ui {
class CPlayDialog;
}

/**
 * @brief One hand of cards.
 *
 * The class controls the display of one board (auction and play). The CPlayView class is used for the
 * actual display and the CPlayShow class is used for traversing the play.
 */
class CPlayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPlayDialog(CGamesDoc *games, int gameIndex, int auctionIndex, QWidget *parent = 0);
    ~CPlayDialog();

public slots:
    void playValue(ReviewVal reviewVal);
    void playClose();
    void buttonClicked(int button);

protected:
    virtual void resizeEvent(QResizeEvent *resizeEvent);

private:
    Ui::CPlayDialog *ui;

    CGamesDoc *games;
    int gameIndex, auctionIndex;
    int board;
    int cards[4][13];
    Seat dealer, declarer;
    Team vulnerable;
    Bids contract, dbl;
    int result;
    CBidHistory bidHistory;
    CPlayHistory playHistory;

    CPlayView *playView;
    CPlayShow *playShow;

    Seat player;
    int playNo;
    int trickNo;
    int trick[4];
};

#endif // CPLAYDIALOG_H
