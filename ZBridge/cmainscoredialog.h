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

class CGamesDoc;

namespace Ui {
class CMainScoreDialog;
}

class CMainScoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMainScoreDialog(CGamesDoc *games, QWidget *parent = 0);
    ~CMainScoreDialog();

private:
    Ui::CMainScoreDialog *ui;

    CGamesDoc *games;
};

#endif // CMAINSCOREDIALOG_H
