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

#include "Defines.h"
#include "cpointscoredialog.h"
#include "crubberscoredialog.h"
#include "cgameoptiondoc.h"
#include "cmainscoredialog.h"
#include "ui_cmainscoredialog.h"

CMainScoreDialog::CMainScoreDialog(CGamesDoc *games, int scoringMethod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMainScoreDialog)
{
    ui->setupUi(this);

    //Headline for table.
    QStringList horizontalHeader;
    horizontalHeader << tr("Board") << tr("Vulnerability") << tr("Contract") << tr("By")
                     << tr("NS-Score");
    ui->scoreTable->setHorizontalHeaderLabels(horizontalHeader);
    ui->scoreTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //Fill table.

    //Make vertical sections clickable.
    ui->scoreTable->verticalHeader()->setSectionsClickable(true);

    this->games = games;
    this->scoringMethod = scoringMethod;

    //Respond to click on row.
    connect(ui->scoreTable->verticalHeader(), &QHeaderView::sectionClicked, this, &CMainScoreDialog::rowClicked);
}

CMainScoreDialog::~CMainScoreDialog()
{
    delete ui;
}

//User clicked a row in the score table.
void CMainScoreDialog::rowClicked(int index)
{
    if (scoringMethod == RUBBER_BRIDGE)
    {
        //Show rubber score dialog.
        CRubberScoreDialog rubberScoreDialog(games, index);
        rubberScoreDialog.exec();
    }
    else
    {
        //Show point score dialog (MP or IMP).
        CPointScoreDialog pointScoreDialog(games, scoringMethod, index);
        pointScoreDialog.exec();
    }
}
