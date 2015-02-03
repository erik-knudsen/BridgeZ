/* Erik Aagaard Knudsen.
  Copyright © 2014 - All Rights Reserved

  Project: ZBridge
  File: CRubberScoreDialog.cpp
  Developers: eak

  Revision History:
  9-aug-2014 eak: Original

  Abstract: Rubber score dialog.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the rubber score dialog class.
 */

#include "cgamesdoc.h"
#include "crubberscoredialog.h"
#include "ui_crubberscoredialog.h"

CRubberScoreDialog::CRubberScoreDialog(CGamesDoc *games, int index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRubberScoreDialog)
{
    ui->setupUi(this);

    this->games = games;
    this->index = index;
}

CRubberScoreDialog::~CRubberScoreDialog()
{
    delete ui;
}
