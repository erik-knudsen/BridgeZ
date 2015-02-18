/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CRankScoreDialog.cpp
  Developers: eak

  Revision History:
  3-feb-2015 eak: Original

  Abstract: Rank score dialog.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the rank score dialog class.
 */

#include "Defines.h"
#include "cgamesdoc.h"
#include "crankscoredialog.h"
#include "ui_crankscoredialog.h"

CRankScoreDialog::CRankScoreDialog(CGamesDoc *games, int scoringMethod, int index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRankScoreDialog)
{
    this->scoringMethod = scoringMethod;
    this->games = games;
    this->index = index;

    ui->setupUi(this);
}

CRankScoreDialog::~CRankScoreDialog()
{
    delete ui;
}
