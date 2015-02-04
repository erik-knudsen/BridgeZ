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

    //Headline for table.
    QStringList horizontalHeader;
    if (scoringMethod == RUBBER_BRIDGE)
    {
        ui->scoreTable->setColumnCount(5);
        horizontalHeader << tr("Board") << tr("Vulnerability") << tr("Contract") <<
                            tr("Tricks") << tr("Rubber");
    }
    else if (scoringMethod == TEAMS_IMP)
    {
        ui->scoreTable->setColumnCount(6);
        horizontalHeader << tr("Board") << tr("Vulnerability") << tr("Contract") <<
                            tr("Tricks") << tr("% IMP Board") << tr("% IMP All");
    }
    else if (scoringMethod == DUPLICATE_MP)
    {
        ui->scoreTable->setColumnCount(6);
        horizontalHeader << tr("Board") << tr("Vulnerability") << tr("Contract") <<
                            tr("Tricks") << tr("% MP Board") << tr("% MP All");
    }
    ui->scoreTable->setHorizontalHeaderLabels(horizontalHeader);
    ui->scoreTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //Fill table.
    int noPlayed = games->getNumberPlayedGames();
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
        games->getAuctionAndPlay(gameIndex, playedAuctionAndPlayIndex,
                        &westName, &northName, &eastName, &southName,
                        &declarer, &contract, &contractModifier, &result);

        if (scoringMethod == RUBBER_BRIDGE)
        {
            //Next fill in rubber bridge column.
        }
        else
        {
            //Next fill in points columns (IMP or MP).
        }
    }

    //Respond to click of cell.
    connect(ui->scoreTable, &QTableWidget::cellClicked, this, &CMainScoreDialog::cellClicked);
}

CMainScoreDialog::~CMainScoreDialog()
{
    delete ui;
}

//User clicked a cell in the score table.
void CMainScoreDialog::cellClicked(int row, int column)
{
    if (column <= 4)
    {
        //Show play dialog.
        int playedAuctionAndPlayIndex = games->getPlayedAuctionAndPlayIndex(row);
        CPlayDialog playDialog(games, row, playedAuctionAndPlayIndex);
        playDialog.exec();
    }
    else if ((column == 5) && (scoringMethod == RUBBER_BRIDGE))
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
    else
    {
        //Show rank dialog.
        CRankScoreDialog rankScoreDialog(games, scoringMethod, row);
        rankScoreDialog.exec();
    }
}
