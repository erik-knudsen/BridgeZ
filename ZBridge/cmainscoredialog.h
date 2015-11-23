/* Erik Aagaard Knudsen.
  Copyright © 2014 - All Rights Reserved

  Project: ZBridge
  File: CMainScoreDialog.h
  Developers: eak

  Revision History:
  9-aug-2014 eak: Original

  Abstract: Main score dialog class.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the main score dialog class.
 */

#ifndef CMAINSCOREDIALOG_H
#define CMAINSCOREDIALOG_H

#include <QDialog>

#include "Defines.h"

class CGamesDoc;

namespace Ui {
class CMainScoreDialog;
}

/**
 * @brief The CMainScoreDialog class shows a table with results of games played untill now by the user.
 *
 * The table has one line for each played game (board). The content of the table depends on
 * the type of play (scoring method). The first 4 columns are common for all scoring methods. They show
 * board number, vulnerability, contract and tricks. If the user clicks a row then the game (auction and play)
 * is shown in a separate dialog. The remaining columns in the table depends on the scoring method:\n
 *   - Practice has a 1 additional column which shows the duplicate score for the game.
 *   - IMP (International Match Point) and MP (Match Point) has 3 additional columns which shows duplicate score,
 *     IMP or MP and IMP average or MP average  for games played untill now. Clicking the IMP/MP column for a game
 *     shows in a separate dialog how the game  has been played by other players. Clicking the IMP/MP average
 *     column for a game shows in a separate dialog how the ranking is for all players for games played until now.
 *   - Rubber has 2 additional columns which shows NS below the line points and EW below the line points.
 *     Below the line points are shown red if vulnerable and green for a won rubber. Clicking one of the 2
 *     additional columns for a game shows in a separate dialog a rubber score table for the current rubber.
 */
class CMainScoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMainScoreDialog(CGamesDoc *games, QWidget *parent = 0);
    ~CMainScoreDialog();

    void closeEvent(QCloseEvent *event);
    void reject();

public slots:
    void sUpdateTable();

private slots:
    void cellClicked(int row, int column);
    void rowClicked(int row);

private:
    Ui::CMainScoreDialog *ui;

    CGamesDoc *games;
    ScoringMethod scoringMethod;
};

#endif // CMAINSCOREDIALOG_H
