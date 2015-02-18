/* Erik Aagaard Knudsen.
  Copyright © 2014 - All Rights Reserved

  Project: ZBridge
  File: CPointScoreDialog.h
  Developers: eak

  Revision History:
  9-aug-2014 eak: Original

  Abstract: Point (MP or IMP) score dialog class.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the point score dialog class.
 */

#ifndef CPOINTSCOREDIALOG_H
#define CPOINTSCOREDIALOG_H

#include <QDialog>

class CGamesDoc;

namespace Ui {
class CPointScoreDialog;
}

class CPointScoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPointScoreDialog(CGamesDoc *games, int scoringMethod, int index, QWidget *parent = 0);
    ~CPointScoreDialog();

private slots:
    void rowClicked(int row);

private:
    Ui::CPointScoreDialog *ui;

    CGamesDoc *games;
    int scoringMethod;
    int index;
};

#endif // CPOINTSCOREDIALOG_H
