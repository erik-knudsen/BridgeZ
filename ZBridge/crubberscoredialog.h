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

class CGamesDoc;

namespace Ui {
class CRubberScoreDialog;
}

class CRubberScoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRubberScoreDialog(CGamesDoc *games, int index, QWidget *parent = 0);
    ~CRubberScoreDialog();

private:
    Ui::CRubberScoreDialog *ui;

    CGamesDoc *games;
    int index;
};

#endif // CRUBBERSCOREDIALOG_H
