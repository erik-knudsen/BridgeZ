/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CPlayShow.h
  Developers: eak

  Revision History:
  13-jul-2015 eak: Original

  Abstract: Dialog for traversing card play.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of a class for traversing the play for one board.
 */

#ifndef CPLAYSHOW_H
#define CPLAYSHOW_H

#include <QDialog>

namespace Ui {
class CPlayShow;
}

enum ReviewVal { REVIEW_PREV, REVIEW_NEXT};

/**
 * @brief This class implements a dialog with two buttons for traversing a card play.
 *
 * The class has:
 *   - one button for forward move. When clicked a next signal is emitted.
 *   - Another button for backward move. When clicked a previous signal is emitted.
  */
class CPlayShow : public QDialog
{
    Q_OBJECT

public:
    explicit CPlayShow(QWidget *parent = 0);
    ~CPlayShow();

    void setEnabled(ReviewVal reviewVal, bool enabled);
    bool isEnabled(ReviewVal reviewVal);

    void closeEvent(QCloseEvent *event);
    void reject();

signals:
    void playValue(ReviewVal reviewVal);
    void playClose();

private slots:
    void on_reviewPrev_clicked();
    void on_reviewNext_clicked();

private:
    Ui::CPlayShow *ui;
};

#endif // CPLAYSHOW_H
