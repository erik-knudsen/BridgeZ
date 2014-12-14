/* Erik Aagaard Knudsen.
  Copyright © 2014 - All Rights Reserved

  Project: ZBridge
  File: CMainScoreDialog.cpp
  Developers: eak

  Revision History:
  9-aug-2014 eak: Original

  Abstract: Main score dialog.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the main score dialog class.
 */

#include "cgameoptiondoc.h"
#include "cmainscoredialog.h"
#include "ui_cmainscoredialog.h"

CMainScoreDialog::CMainScoreDialog(CGamesDoc *games, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMainScoreDialog)
{
    ui->setupUi(this);

    this->games = games;
}

CMainScoreDialog::~CMainScoreDialog()
{
    delete ui;
}
