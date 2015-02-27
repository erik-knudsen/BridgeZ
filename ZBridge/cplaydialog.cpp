/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CPlayDialog.cpp
  Developers: eak

  Revision History:
  3-feb-2015 eak: Original

  Abstract: Show play for a given board.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of a class for showing the play
 * (auction and play) for one board.
 */

#include "Defines.h"
#include "cgamesdoc.h"
#include "cplaydialog.h"
#include "ui_cplaydialog.h"

CPlayDialog::CPlayDialog(CGamesDoc *games, int gameIndex, int auctionIndex, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPlayDialog)
{
    this->games = games;
    this->gameIndex = gameIndex;
    this->auctionIndex = auctionIndex;

    ui->setupUi(this);
}

CPlayDialog::~CPlayDialog()
{
    delete ui;
}
