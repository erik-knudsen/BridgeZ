/* Erik Aagaard Knudsen.
  Copyright © 2014 - All Rights Reserved

  Project: ZBridge
  File: CPointDialog.cpp
  Developers: eak

  Revision History:
  9-aug-2014 eak: Original

  Abstract: Point (MP or IMP) score dialog.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the point score dialog class.
 */

#include "Defines.h"
#include "cgamesdoc.h"
#include "cplaydialog.h"
#include "cpointscoredialog.h"
#include "ui_cpointscoredialog.h"

CPointScoreDialog::CPointScoreDialog(CGamesDoc *games, int scoringMethod, int index, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPointScoreDialog)
{
    int board;
    Seat dealer;
    Team vulnerable;

    ui->setupUi(this);

    this->games = games;
    this->scoringMethod = scoringMethod;
    this->index = index;

    games->getGame(index, &board, &dealer, &vulnerable);

    //Board.
    ui->board->setText(tr("Board %1").arg(board));

    //Vulnerability.
    ui->vulnerability->setText(tr("Vulnerability %1").arg(TEAM_NAMES[vulnerable]));

    //Scoring method.
    if (scoringMethod == IMP)
        ui->scoringMethod->setText(tr("Scoring method is IMP"));
    else
        ui->scoringMethod->setText(tr("Scoring method is MP"));

    //Headline for table.
    QStringList horizontalHeader;
    ui->pointScoreTable->setColumnCount(5);
    horizontalHeader << tr("Name N-S / E-W") << tr("Contract") << tr("Tricks") << tr("Score") << tr("Point");

    ui->pointScoreTable->horizontalHeader()->setStretchLastSection(true);
    ui->pointScoreTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->pointScoreTable->horizontalHeader()->setVisible(true);
    ui->pointScoreTable->setHorizontalHeaderLabels(horizontalHeader);
    ui->pointScoreTable->horizontalHeader()->setSectionsClickable(false);

    //Fill table.
    int noAuctionAndPlay = games->getNumberAuctionAndPlay(index);
    ui->pointScoreTable->setRowCount(noAuctionAndPlay);
    for (int auctionAndPlayIndex = 0; auctionAndPlayIndex < noAuctionAndPlay; auctionAndPlayIndex++)
    {
        QString westName, northName, eastName, southName;
        Seat declarer;
        Bids contract, contractModifier;
        int result;

        games->getActorNames(index, auctionAndPlayIndex,
                             &westName, &northName, &eastName, &southName);
        games->getAuctionAndPlay(index, auctionAndPlayIndex,
                        &declarer, &contract, &contractModifier, &result);

        //Players.
        QString players = QString("%1-%2 / %3-%4").arg(northName).arg(southName).arg(eastName).arg(westName);
        QTableWidgetItem *playersItem = new QTableWidgetItem(players);
        playersItem->setTextAlignment(Qt::AlignCenter);
        playersItem->setFlags(Qt::ItemIsEnabled);
        ui->pointScoreTable->setItem(auctionAndPlayIndex, 0, playersItem);

        //Contract.
        Suit suit = BID_SUIT(contract);
        int level = BID_LEVEL(contract);
        QString cntr = QString("%1 %2%3").arg(tr(SEAT_NAMES[declarer])[0]).arg(level).arg(tr(SUIT_NAMES[suit]));
        if (contractModifier == BID_DOUBLE)
            cntr += " X";
        else if (contractModifier == BID_REDOUBLE)
            cntr += " XX";
        QTableWidgetItem *contractItem = new QTableWidgetItem(cntr);
        contractItem->setTextAlignment(Qt::AlignCenter);
        contractItem->setFlags(Qt::ItemIsEnabled);
        ui->pointScoreTable->setItem(auctionAndPlayIndex, 1, contractItem);

        //Result.
        QTableWidgetItem *resultItem = new QTableWidgetItem(tr("%1").arg(result));
        resultItem->setTextAlignment(Qt::AlignCenter);
        resultItem->setFlags(Qt::ItemIsEnabled);
        ui->pointScoreTable->setItem(auctionAndPlayIndex, 2, resultItem);

        //Score.
        int score = games->getDuplicateScore(index, auctionAndPlayIndex);
        QTableWidgetItem *scoreItem = new QTableWidgetItem(tr("%1").arg(score));
        scoreItem->setTextAlignment(Qt::AlignCenter);
        scoreItem->setFlags(Qt::ItemIsEnabled);
        ui->pointScoreTable->setItem(auctionAndPlayIndex, 3, scoreItem);

        //Points (MP or IMP).
        float point = games->getDuplicatePointBoard(index,auctionAndPlayIndex, scoringMethod);
        QTableWidgetItem *nsewPointItem = new QTableWidgetItem(tr("%1").arg(point, 0, 'f', 1));
        nsewPointItem->setTextAlignment(Qt::AlignCenter);
        nsewPointItem->setFlags(Qt::ItemIsEnabled);
        ui->pointScoreTable->setItem(auctionAndPlayIndex, 4, nsewPointItem);
    }

    ui->pointScoreTable->resizeColumnsToContents();

    //Make vertical sections clickable.
    ui->pointScoreTable->verticalHeader()->setVisible(true);
    ui->pointScoreTable->verticalHeader()->setSectionsClickable(true);

    //Respond to click on row.
    connect(ui->pointScoreTable->verticalHeader(), &QHeaderView::sectionClicked, this, &CPointScoreDialog::rowClicked);
}

CPointScoreDialog::~CPointScoreDialog()
{
    delete ui;
}

//User clicked a row in the table.
void CPointScoreDialog::rowClicked(int row)
{
    ui->pointScoreTable->verticalHeader()->reset();

    //Show play dialog.
    CPlayDialog playDialog(games, index, row, this);
    playDialog.exec();
}
