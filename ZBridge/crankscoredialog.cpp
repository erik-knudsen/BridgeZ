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
#include "cplaydialog.h"
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

    //Scoring method.
    if (scoringMethod == IMP)
        ui->scoringMethod->setText(tr("Scoring method is IMP"));
    else
        ui->scoringMethod->setText(tr("Scoring method is MP"));

    //Headline for table.
    QStringList horizontalHeader;
    ui->rankScoreTable->setColumnCount(2);
    horizontalHeader << tr("Name") << tr("Point");

    ui->rankScoreTable->horizontalHeader()->setStretchLastSection(true);
    ui->rankScoreTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->rankScoreTable->horizontalHeader()->setVisible(true);
    ui->rankScoreTable->setHorizontalHeaderLabels(horizontalHeader);
    ui->rankScoreTable->horizontalHeader()->setSectionsClickable(false);

    QStringList pairWN;
    QStringList pairES;
    int noPairs = games->getPairs(index, pairWN, pairES);
    ui->rankScoreTable->setRowCount(noPairs);

    //Fill table.
    for (int pairIndex = 0; pairIndex < noPairs; pairIndex++)
    {
        //Pair.
        QString pair = QString("%1-%2").arg(pairWN[pairIndex]).arg(pairES[pairIndex]);
        QTableWidgetItem *pairItem = new QTableWidgetItem(pair);
        pairItem->setTextAlignment(Qt::AlignCenter);
        pairItem->setFlags(Qt::ItemIsEnabled);
        ui->rankScoreTable->setItem(pairIndex, 0, pairItem);

        //Result.
        float result = games->getDuplicateResultAll(index, pairWN[pairIndex], pairES[pairIndex],
                                             scoringMethod);
        QTableWidgetItem *resultItem = new QTableWidgetItem;
        result = (result > 0.) ? (((int)(result * 10. + 0.5)) / 10.) : (((int)(result * 10. - 0.5)) / 10.);
        resultItem->setData(Qt::EditRole, result);
        resultItem->setTextAlignment(Qt::AlignCenter);
        resultItem->setFlags(Qt::ItemIsEnabled);
        ui->rankScoreTable->setItem(pairIndex, 1, resultItem);
    }
    ui->rankScoreTable->setSortingEnabled(true);
    ui->rankScoreTable->sortByColumn(1);

    //Make vertical sections clickable.
    ui->rankScoreTable->verticalHeader()->setVisible(true);
    ui->rankScoreTable->verticalHeader()->setSectionsClickable(true);

    //Respond to click on row.
    connect(ui->rankScoreTable->verticalHeader(), &QHeaderView::sectionClicked, this, &CRankScoreDialog::rowClicked);
}

CRankScoreDialog::~CRankScoreDialog()
{
    delete ui;
}

//User clicked a row in the table.
void CRankScoreDialog::rowClicked(int row)
{
    ui->rankScoreTable->verticalHeader()->reset();

    //Show play dialog.
    CPlayDialog playDialog(games, index, row, this);
    playDialog.exec();
}
