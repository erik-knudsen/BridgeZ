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

#include "cpointscoredialog.h"
#include "crubberscoredialog.h"
#include "crankscoredialog.h"
#include "cplaydialog.h"
#include "cgamesdoc.h"
#include "cmainscoredialog.h"
#include "ui_cmainscoredialog.h"

CMainScoreDialog::CMainScoreDialog(CGamesDoc *games, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMainScoreDialog)
{
    QString westName, northName, eastName, southName;

    this->games = games;
    scoringMethod = games->getScoringMethod();

    ui->setupUi(this);

    //Headline for table.
    QStringList horizontalHeader;
    if (scoringMethod == RUBBER)
    {
        ui->scoreTable->setColumnCount(6);
        horizontalHeader << tr("Board") << tr("Vulnerability") << tr("Contract") <<
                            tr("Tricks") << tr("NS") << tr("EW");
        ui->ScoringMethod->setText(tr("Scoring method is Rubber."));
    }
    else if (scoringMethod == PRACTICE)
    {
        ui->scoreTable->setColumnCount(5);
        horizontalHeader << tr("Board") << tr("Vulnerability") << tr("Contract") <<
                            tr("Tricks") << tr("Score");
        ui->ScoringMethod->setText(tr("Scoring method is Practice."));
    }
    else if (scoringMethod == IMP)
    {
        ui->scoreTable->setColumnCount(7);
        horizontalHeader << tr("Board") << tr("Vulnerability") << tr("Contract") <<
                            tr("Tricks") << tr("Score") << tr("Board") << tr("All");
        ui->ScoringMethod->setText(tr("Scoring method is IMP"));
}
    else if (scoringMethod == MP)
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
        QString nsNames, ewNames;

        games->getPlayerNames(PLAYED_GAME, &westName, &northName, &eastName, &southName);
        nsNames = northName + " / " + southName;
        ewNames = eastName + " / " + westName;
        ui->ewNames->setText(ewNames);
        ui->nsNames->setText(nsNames);
    }

    ui->scoreTable->horizontalHeader()->setStretchLastSection(true);
    ui->scoreTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->scoreTable->horizontalHeader()->setVisible(true);
    ui->scoreTable->setHorizontalHeaderLabels(horizontalHeader);
    ui->scoreTable->horizontalHeader()->setSectionsClickable(false);

    int belowTheLineNSGamePoint = 0, belowTheLineEWGamePoint = 0;
    int belowTheLineNSGame = 0, belowTheLineEWGame = 0;

    //Fill table.
    for (int gameIndex = 0; gameIndex < noPlayed; gameIndex++)
    {
        //First board, vulnerability, contract and tricks (common for all scoring methods).
        int board;
        Seat dealer;
        Team vulnerable;
        Seat declarer;
        Bids contract, contractModifier;
        int result;

        games->getGame(gameIndex, &board, &dealer, &vulnerable);
        int playedAuctionAndPlayIndex = games->getPlayedAuctionAndPlayIndex(gameIndex);
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
        QString cntr;
        //Has the game been played?
        if (contract == BID_NONE)
            cntr = tr("NP");
        //Was it a pass game?
        else if (contract == BID_PASS)
            cntr = tr("P");
        //It is a played game.
        else
        {
            Suit suit = BID_SUIT(contract);
            int level = BID_LEVEL(contract);
            cntr = QString("%1 %2%3").arg(tr(SEAT_NAMES[declarer])[0]).arg(level).arg(tr(SUIT_NAMES[suit]));
            if (contractModifier == BID_DOUBLE)
                cntr += " X";
            else if (contractModifier == BID_REDOUBLE)
                cntr += " XX";
        }
        QTableWidgetItem *contractItem = new QTableWidgetItem(cntr);
        contractItem->setFlags(Qt::ItemIsEnabled);
        contractItem->setTextAlignment(Qt::AlignCenter);
        ui->scoreTable->setItem(gameIndex, 2, contractItem);

        //Result.
        QString res;
        if (declarer == NO_SEAT)
            res = tr("-");
        else
            res = tr("%1").arg(result);
        QTableWidgetItem *resultItem = new QTableWidgetItem(res);
        resultItem->setTextAlignment(Qt::AlignCenter);
        resultItem->setFlags(Qt::ItemIsEnabled);
        ui->scoreTable->setItem(gameIndex, 3, resultItem);

        if (scoringMethod == RUBBER)
        {
            //Next fill in rubber bridge columns.
            int belowTheLinePoint = games->getBelowTheLine(gameIndex);

            if (belowTheLinePoint > 0)
            {
                belowTheLineNSGamePoint += belowTheLinePoint;
                QTableWidgetItem *belowTheLineNSItem = new QTableWidgetItem(tr("%1").arg(belowTheLinePoint));
                belowTheLineNSItem->setTextAlignment(Qt::AlignCenter);
                belowTheLineNSItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                if (belowTheLineNSGamePoint >= 100)
                {
                    belowTheLineNSGame++;
                    if (belowTheLineNSGame == 2)
                    {
                      belowTheLineNSItem->setForeground(QBrush(Qt::green));
                      belowTheLineNSGame = belowTheLineEWGame = 0;
                    }
                    else
                        belowTheLineNSItem->setForeground(QBrush(Qt::red));
                    belowTheLineNSGamePoint = belowTheLineEWGamePoint = 0;
                }
                ui->scoreTable->setItem(gameIndex, 4, belowTheLineNSItem);
            }
            else if (belowTheLinePoint < 0)
            {
                belowTheLineEWGamePoint -= belowTheLinePoint;
                QTableWidgetItem *belowTheLineEWItem = new QTableWidgetItem(tr("%1").arg(-belowTheLinePoint));
                belowTheLineEWItem->setTextAlignment(Qt::AlignCenter);
                belowTheLineEWItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                if (belowTheLineEWGamePoint >= 100)
                {
                    belowTheLineEWGame++;
                    if (belowTheLineEWGame == 2)
                    {
                      belowTheLineEWItem->setForeground(QBrush(Qt::green));
                      belowTheLineNSGame = belowTheLineEWGame = 0;
                    }
                    else
                        belowTheLineEWItem->setForeground(QBrush(Qt::red));
                    belowTheLineNSGamePoint = belowTheLineEWGamePoint = 0;
                }
                ui->scoreTable->setItem(gameIndex, 5, belowTheLineEWItem);
            }
        }
        else
        {
            int score;

            //Score.
            QTableWidgetItem *scoreItem;
            if (playedAuctionAndPlayIndex != -1)
            {
                score = games->getDuplicateScore(gameIndex, playedAuctionAndPlayIndex);
                scoreItem = new QTableWidgetItem(tr("%1").arg(score));
            }
            else
                scoreItem = new QTableWidgetItem(tr("-"));
            scoreItem->setTextAlignment(Qt::AlignCenter);
            scoreItem->setFlags(Qt::ItemIsEnabled);
            ui->scoreTable->setItem(gameIndex, 4, scoreItem);

            if (scoringMethod != PRACTICE)
            {
                float result = games->getDuplicatePointBoard(gameIndex, playedAuctionAndPlayIndex,
                                             scoringMethod);
                QTableWidgetItem *gameItem = new QTableWidgetItem(tr("%1").arg(result, 0, 'f', 1));
                gameItem->setTextAlignment(Qt::AlignCenter);
                gameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                ui->scoreTable->setItem(gameIndex, 5, gameItem);

                QString nameWN, nameES;
                if ((declarer == WEST_SEAT) || (declarer == EAST_SEAT))
                {
                    nameWN = westName;
                    nameES = eastName;
                }
                else
                {
                    nameWN = northName;
                    nameES = southName;
                }
                result = games->getDuplicateResultAll(gameIndex, nameWN, nameES, scoringMethod);
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
    if (item != 0)
        item->setSelected(false);
    if (((column == 4) || (column == 5)) && (scoringMethod == RUBBER))
    {
        //Show rubber score dialog.
        CRubberScoreDialog rubberScoreDialog(games, row, this);
        rubberScoreDialog.exec();
    }
    else if (column == 5)
    {
        //Show point score dialog (MP or IMP).
        CPointScoreDialog pointScoreDialog(games, scoringMethod, row, this);
        pointScoreDialog.exec();
    }
    else if (column == 6)
    {
        //Show rank dialog.
        CRankScoreDialog rankScoreDialog(games, scoringMethod, row, this);
        rankScoreDialog.exec();
    }
}

//User clicked a row in the table.
void CMainScoreDialog::rowClicked(int row)
{
    ui->scoreTable->verticalHeader()->reset();

    //Show play dialog.
    int playedAuctionAndPlayIndex = games->getPlayedAuctionAndPlayIndex(row);
    CPlayDialog playDialog(games, row, playedAuctionAndPlayIndex, this);
    playDialog.exec();
}
