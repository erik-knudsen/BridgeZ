/* Erik Aagaard Knudsen.
  Copyright © 2014 - All Rights Reserved

  Project: ZBridge
  File: CRubberScoreDialog.h
  Developers: eak

  Revision History:
  9-aug-2014 eak: Original

  Abstract: Rubber score dialog class.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the rubber score dialog class.
 */

#ifndef CRUBBERSCOREDIALOG_H
#define CRUBBERSCOREDIALOG_H

#include <QDialog>

#include "Defines.h"

class CGamesDoc;

namespace Ui {
class CRubberScoreDialog;
}

/**
 * @brief The CRubberScoreDialog class shows in a drawing  the we/they rubber table for a given rubber.
 *
 * The drawing shows the well known we/they, above the line/below the line table. In addition to the results
 * for the current rubber, then there is also a ledger line showing the total result for all rubbers untill now.
 */
class CRubberScoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRubberScoreDialog(CGamesDoc *games, int index, QWidget *parent = 0);
    ~CRubberScoreDialog();

private:
    void paintEvent(QPaintEvent *);

    Ui::CRubberScoreDialog *ui;

    struct rubberPoint
    {
        bool gameDone, rubberDone;
        int board;
        Bids contract, contractModifier;
        int tricks;
        Seat declarer;
        Team vulnerable;
        int nsAbove, nsBelow, nsTotal, nsLedger;
        int ewAbove, ewBelow, ewTotal, ewLedger;
    };

    rubberPoint *rubberPoints;

    CGamesDoc *games;
    int index;
};

#endif // CRUBBERSCOREDIALOG_H
