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

class CPlayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPlayDialog(CGamesDoc *games, int gameIndex, int auctionIndex, QWidget *parent = 0);
    ~CPlayDialog();

public slots:
    void playValue(ReviewVal reviewVal);
    void playClose();

protected:
    virtual void resizeEvent(QResizeEvent *resizeEvent);

private:
    Ui::CPlayDialog *ui;

    CGamesDoc *games;
    int gameIndex, auctionIndex;
    int board;
    int wCards[13], nCards[13], eCards[13], sCards[13];
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
