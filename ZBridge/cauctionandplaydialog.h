/* Erik Aagaard Knudsen.
  Copyright © 2014 - All Rights Reserved

  Project: ZBridge
  File: CAuctionAndPlayDialog.h
  Developers: eak

  Revision History:
  9-aug-2014 eak: Original

  Abstract: Auction and Play dialog class.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the auction and play dialog class.
 */

#ifndef CAUCTIONANDPLAYDIALOG_H
#define CAUCTIONANDPLAYDIALOG_H

#include <QWidget>

namespace Ui {
class CAuctionAndPlayDialog;
}

class CAuctionAndPlayDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CAuctionAndPlayDialog(QWidget *parent = 0);
    ~CAuctionAndPlayDialog();

private:
    Ui::CAuctionAndPlayDialog *ui;
};

#endif // CAUCTIONANDPLAYDIALOG_H
