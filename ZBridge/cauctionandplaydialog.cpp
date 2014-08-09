/* Erik Aagaard Knudsen.
  Copyright © 2014 - All Rights Reserved

  Project: ZBridge
  File: CAuctionAndPlayDialog.cpp
  Developers: eak

  Revision History:
  9-aug-2014 eak: Original

  Abstract: Auction and play dialog.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the auction and play dialog class.
 */

#include "cauctionandplaydialog.h"
#include "ui_cauctionandplaydialog.h"

CAuctionAndPlayDialog::CAuctionAndPlayDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CAuctionAndPlayDialog)
{
    ui->setupUi(this);
}

CAuctionAndPlayDialog::~CAuctionAndPlayDialog()
{
    delete ui;
}
