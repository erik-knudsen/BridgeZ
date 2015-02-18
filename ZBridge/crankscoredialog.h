/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CRankScoreDialog.h
  Developers: eak

  Revision History:
  3-feb-2015 eak: Original

  Abstract: Show ranking.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the rank score dialog class.
 */

#ifndef CRANKSCOREDIALOG_H
#define CRANKSCOREDIALOG_H

#include <QDialog>

class CGamesDoc;

namespace Ui {
class CRankScoreDialog;
}

class CRankScoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRankScoreDialog(CGamesDoc *games, int scoringMethod, int index, QWidget *parent = 0);
    ~CRankScoreDialog();

private:
    Ui::CRankScoreDialog *ui;

    CGamesDoc *games;
    int scoringMethod;
    int index;
};

#endif // CRANKSCOREDIALOG_H
