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
#include "crankscoredialog.h"
#include "cplaydialog.h"
#include "cgamesdoc.h"
#include "cmainscoredialog.h"
#include "ui_cmainscoredialog.h"

CMainScoreDialog::CMainScoreDialog(CGamesDoc *games, int scoringMethod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMainScoreDialog)
{
    this->scoringMethod = scoringMethod;
    this->games = games;

    ui->setupUi(this);

    if (((scoringMethod == DUPLICATE_MP) || (scoringMethod == TEAMS_IMP)) && games->practice())
            scoringMethod = PRACTICE;

    //Headline for table.
    QStringList horizontalHeader;
    if (scoringMethod == RUBBER_BRIDGE)
    {
        ui->scoreTable->setColumnCount(5);
        horizontalHeader << tr("Board") << tr("Vulnerability") << tr("Contract") <<
                            tr("Tricks") << tr("Rubber");
        ui->ScoringMethod->setText(tr("Scoring method is Rubber."));
    }
    else if (scoringMethod == PRACTICE)
    {
        ui->scoreTable->setColumnCount(5);
        horizontalHeader << tr("Board") << tr("Vulnerability") << tr("Contract") <<
                            tr("Tricks") << tr("Score");
        ui->ScoringMethod->setText(tr("Scoring method is Practice."));
    }
    else if (scoringMethod == TEAMS_IMP)
    {
        ui->scoreTable->setColumnCount(7);
        horizontalHeader << tr("Board") << tr("Vulnerability") << tr("Contract") <<
                            tr("Tricks") << tr("Score") << tr("Board") << tr("All");
        ui->ScoringMethod->setText(tr("Scoring method is IMP"));
}
    else if (scoringMethod == DUPLICATE_MP)
    {
        ui->scoreTable->setColumnCount(7);
        horizontalHeader << tr("Board") << tr("Vulnerability") << tr("Contract") <<
                            tr("Tricks") << tr("Score") << tr("Board") << tr("All");
        ui->ScoringMethod->setText(tr("Scoring method is MP"));
    }
    int noPlayed = games->getNumberPlayedGames();
    ui->scoreTable->setRowCount(noPlayed);
    if (noPlayed > 0)
    {
        QString westName, northName, eastName, southName;
        QString nsNames, ewNames;

        int playedAuctionAndPlayIndex = games->getPlayedAuctionAndPlayIndex(0);
        games->getActorNames(0, playedAuctionAndPlayIndex, &westName, &northName, &eastName, &southName);
        nsNames = northName + " / " + southName;
        ewNames = eastName + " / " + westName;
        ui->ewNames->setText(ewNames);
        ui->nsNames->setText(nsNames);
    }

//    ui->scoreTable->horizontalHeader()->stretchLastSection();
    ui->scoreTable->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->scoreTable->horizontalHeader()->setVisible(true);
    ui->scoreTable->setHorizontalHeaderLabels(horizontalHeader);
    ui->scoreTable->horizontalHeader()->setSectionsClickable(false);

    //Fill table.
    for (int gameIndex = 0; gameIndex < noPlayed; gameIndex++)
    {
        //First board, vulnerability, contract and tricks (common for all scoring methods).
        int board;
        Seat dealer;
        Team vulnerable;
        QString westName, northName, eastName, southName;
        Seat declarer;
        Bids contract, contractModifier;
        int result;

        int playedAuctionAndPlayIndex = games->getPlayedAuctionAndPlayIndex(gameIndex);
        games->getGame(gameIndex, &board, &dealer, &vulnerable);
        games->getActorNames(gameIndex, playedAuctionAndPlayIndex,
                             &westName, &northName, &eastName, &southName);
        games->getAuctionAndPlay(gameIndex, playedAuctionAndPlayIndex,
                        &declarer, &contract, &contractModifier, &result);

        //Board.
        QTableWidgetItem *boardItem = new QTableWidgetItem(tr("%1").arg(board));
        boardItem->setTextAlignment(Qt::AlignCenter);
        boardItem->setFlags(Qt::ItemIsEnabled);
        ui->scoreTable->setItem(gameIndex, 0, boardItem);

        //Vulnerability.
        QTableWidgetItem *vulnerabilityItem = new QTableWidgetItem(tr("%1").arg(TEAM_NAMES[vulnerable]));
        vulnerabilityItem->setTextAlignment(Qt::AlignCenter);
        vulnerabilityItem->setFlags(Qt::ItemIsEnabled);
        ui->scoreTable->setItem(gameIndex, 1, vulnerabilityItem);

        //Contract.
        Suit suit = BID_SUIT(contract);
        int level = BID_LEVEL(contract);
        QString cntr = QString("%1 %2%3").arg(tr(SEAT_NAMES[declarer])[0]).arg(level).arg(tr(SUIT_NAMES[suit]));
        if (contractModifier == BID_DOUBLE)
            cntr += " X";
        else if (contractModifier == BID_REDOUBLE)
            cntr += " XX";
        QTableWidgetItem *contractItem = new QTableWidgetItem(cntr);
        contractItem->setFlags(Qt::ItemIsEnabled);
        contractItem->setTextAlignment(Qt::AlignCenter);
        ui->scoreTable->setItem(gameIndex, 2, contractItem);

        //Result.
        QTableWidgetItem *resultItem = new QTableWidgetItem(tr("%1").arg(result));
        resultItem->setTextAlignment(Qt::AlignCenter);
        resultItem->setFlags(Qt::ItemIsEnabled);
        ui->scoreTable->setItem(gameIndex, 3, resultItem);

        if (scoringMethod == RUBBER_BRIDGE)
        {
            //Next fill in rubber bridge column.
            QTableWidgetItem *rubberItem = new QTableWidgetItem("Rubber");
            rubberItem->setTextAlignment(Qt::AlignCenter);
            rubberItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            ui->scoreTable->setItem(gameIndex, 4, rubberItem);
        }
        else
        {
            int score;

            //Score.
            score = games->getDuplicateScore(gameIndex, playedAuctionAndPlayIndex);
            QTableWidgetItem *scoreItem = new QTableWidgetItem(tr("%1").arg(score));
            scoreItem->setTextAlignment(Qt::AlignCenter);
            scoreItem->setFlags(Qt::ItemIsEnabled);
            ui->scoreTable->setItem(gameIndex, 4, scoreItem);

            if (scoringMethod != PRACTICE)
            {
                float result = games->getDuplicatePointBoard(gameIndex, playedAuctionAndPlayIndex,
                                             scoringMethod);
                if (scoringMethod == DUPLICATE_MP)
                    result = result * 100 / (2 * games->getNumberAuctionAndPlay(gameIndex) - 2);
                QTableWidgetItem *gameItem = new QTableWidgetItem(tr("%1").arg(result, 0, 'f', 1));
                gameItem->setTextAlignment(Qt::AlignCenter);
                gameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                ui->scoreTable->setItem(gameIndex, 5, gameItem);

                QString name1, name2;
                if ((declarer == WEST_SEAT) || (declarer == EAST_SEAT))
                {
                    name1 = westName;
                    name2 = eastName;
                }
                else
                {
                    name1 = northName;
                    name2 = southName;
                }
                result = games->getDuplicateResultAll(gameIndex, name1, name2, scoringMethod);
                QTableWidgetItem *allItem = new QTableWidgetItem(tr("%1").arg(result, 0, 'f', 1));
                allItem->setTextAlignment(Qt::AlignCenter);
                allItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                ui->scoreTable->setItem(gameIndex, 6, allItem);
            }
        }
    }
    ui->scoreTable->resizeColumnsToContents();

    //Make vertical sections clickable.
    ui->scoreTable->verticalHeader()->setVisible(true);
    ui->scoreTable->verticalHeader()->setSectionsClickable(true);

    //Respond to click of cell.
    connect(ui->scoreTable, &QTableWidget::cellClicked, this, &CMainScoreDialog::cellClicked);

    //Respond to click on row.
    connect(ui->scoreTable->verticalHeader(), &QHeaderView::sectionClicked, this, &CMainScoreDialog::rowClicked);
}

CMainScoreDialog::~CMainScoreDialog()
{
    delete ui;
}

//User clicked a cell in the score table.
void CMainScoreDialog::cellClicked(int row, int column)
{
    QTableWidgetItem *item = ui->scoreTable->item(row, column);
    item->setSelected(false);
    if ((column == 4) && (scoringMethod == RUBBER_BRIDGE))
    {
        //Show rubber score dialog.
        CRubberScoreDialog rubberScoreDialog(games, row);
        rubberScoreDialog.exec();
    }
    else if (column == 5)
    {
        //Show point score dialog (MP or IMP).
        CPointScoreDialog pointScoreDialog(games, scoringMethod, row);
        pointScoreDialog.exec();
    }
    else if (column == 6)
    {
        //Show rank dialog.
        CRankScoreDialog rankScoreDialog(games, scoringMethod, row);
        rankScoreDialog.exec();
    }
}

//User clicked a row in the table.
void CMainScoreDialog::rowClicked(int row)
{
    ui->scoreTable->verticalHeader()->reset();

    //Show play dialog.
    int playedAuctionAndPlayIndex = games->getPlayedAuctionAndPlayIndex(row);
    CPlayDialog playDialog(games, row, playedAuctionAndPlayIndex);
    playDialog.exec();
}
