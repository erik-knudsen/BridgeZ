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

class CGamesDoc;

namespace Ui {
class CPlayDialog;
}

class CPlayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPlayDialog(CGamesDoc *games, int gameIndex, int auctionIndex, QWidget *parent = 0);
    ~CPlayDialog();

private:
    Ui::CPlayDialog *ui;

    CGamesDoc *games;
    int gameIndex, auctionIndex;
};

#endif // CPLAYDIALOG_H
