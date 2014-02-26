/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidDialog.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Bid dialog

  Platforms: Qt.

*/

/**
 * @file
 * Bid dialog (declaration).
 */

#ifndef CBIDDIALOG_H
#define CBIDDIALOG_H

#include <QDialog>
#include <QToolButton>

#include "defines.h"

namespace Ui {
class CBidDialog;
}

/**
 * @brief The CBidDialog class
 *
 * Implements the bid dialog where the player can select his next bid.
 */
class CBidDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CBidDialog(QWidget *parent = 0);
    ~CBidDialog();

    void closeEvent(QCloseEvent *event);
    void enableBidder(Seat seat, Bids lastBid, Bids doubleBid);
    void disableBidder();

signals:
    void bidValue(Bids bid);
    void bidBackup();
    void bidHint();
    void bidRestart();
    void bidClose();

private:
    void bidClicked(Bids nBid);

private slots:
    void on_bid1C_clicked();
    void on_bid1D_clicked();
    void on_bid1H_clicked();
    void on_bid1S_clicked();
    void on_bid1NT_clicked();
    void on_bid2C_clicked();
    void on_bid2D_clicked();
    void on_bid2H_clicked();
    void on_bid2S_clicked();
    void on_bid2NT_clicked();
    void on_bid3C_clicked();
    void on_bid3D_clicked();
    void on_bid3H_clicked();
    void on_bid3S_clicked();
    void on_bid3NT_clicked();
    void on_bid4C_clicked();
    void on_bid4D_clicked();
    void on_bid4H_clicked();
    void on_bid4S_clicked();
    void on_bid4NT_clicked();
    void on_bid5C_clicked();
    void on_bid5D_clicked();
    void on_bid5H_clicked();
    void on_bid5S_clicked();
    void on_bid5NT_clicked();
    void on_bid6C_clicked();
    void on_bid6D_clicked();
    void on_bid6H_clicked();
    void on_bid6S_clicked();
    void on_bid6NT_clicked();
    void on_bid7C_clicked();
    void on_bid7D_clicked();
    void on_bid7H_clicked();
    void on_bid7S_clicked();
    void on_bid7NT_clicked();
    void on_bidPass_clicked();
    void on_bidDouble_clicked();
    void on_bidRedouble_clicked();
    void on_bidBackup_clicked();
    void on_bidHint_clicked();
    void on_bidRestart_clicked();

private:
    Ui::CBidDialog *ui;
    QToolButton *m_pBidButtons[38];
};

#endif // CBIDDIALOG_H
