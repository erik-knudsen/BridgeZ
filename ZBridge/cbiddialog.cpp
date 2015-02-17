/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidDialog.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Bid dialog

  Platforms: Qt.

*/

/**
 * @file
 * Bid dialog (definition).
 */

#include "cbiddialog.h"
#include "ui_cbiddialog.h"

/**
 * @brief Bid dialog
 * @param parent Pointer to parent.
 *
 * The constructor initalizes the bid dialog:
 *   - Set up user interface.
 *   - Initially disable the dialog.
 *   - Map bid user interface bid buttons.
 */
CBidDialog::CBidDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBidDialog)
{
    //Show dialog.
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint| Qt::WindowSystemMenuHint);

    setEnabled(false);

    //Initialize array for identification of bid buttons.
    m_pBidButtons[BID_1C] = ui->bid1C;
    m_pBidButtons[BID_1D] = ui->bid1D;
    m_pBidButtons[BID_1H] = ui->bid1H;
    m_pBidButtons[BID_1S] = ui->bid1S;
    m_pBidButtons[BID_1NT] = ui->bid1NT;
    m_pBidButtons[BID_2C] = ui->bid2C;
    m_pBidButtons[BID_2D] = ui->bid2D;
    m_pBidButtons[BID_2H] = ui->bid2H;
    m_pBidButtons[BID_2S] = ui->bid2S;
    m_pBidButtons[BID_2NT] = ui->bid2NT;
    m_pBidButtons[BID_3C] = ui->bid3C;
    m_pBidButtons[BID_3D] = ui->bid3D;
    m_pBidButtons[BID_3H] = ui->bid3H;
    m_pBidButtons[BID_3S] = ui->bid3S;
    m_pBidButtons[BID_3NT] = ui->bid3NT;
    m_pBidButtons[BID_4C] = ui->bid4C;
    m_pBidButtons[BID_4D] = ui->bid4D;
    m_pBidButtons[BID_4H] = ui->bid4H;
    m_pBidButtons[BID_4S] = ui->bid4S;
    m_pBidButtons[BID_4NT] = ui->bid4NT;
    m_pBidButtons[BID_5C] = ui->bid5C;
    m_pBidButtons[BID_5D] = ui->bid5D;
    m_pBidButtons[BID_5H] = ui->bid5H;
    m_pBidButtons[BID_5S] = ui->bid5S;
    m_pBidButtons[BID_5NT] = ui->bid5NT;
    m_pBidButtons[BID_6C] = ui->bid6C;
    m_pBidButtons[BID_6D] = ui->bid6D;
    m_pBidButtons[BID_6H] = ui->bid6H;
    m_pBidButtons[BID_6S] = ui->bid6S;
    m_pBidButtons[BID_6NT] = ui->bid6NT;
    m_pBidButtons[BID_7C] = ui->bid7C;
    m_pBidButtons[BID_7D] = ui->bid7D;
    m_pBidButtons[BID_7H] = ui->bid7H;
    m_pBidButtons[BID_7S] = ui->bid7S;
    m_pBidButtons[BID_7NT] = ui->bid7NT;
    m_pBidButtons[BID_PASS] = ui->bidPass;
    m_pBidButtons[BID_DOUBLE] = ui->bidDouble;
    m_pBidButtons[BID_REDOUBLE] = ui->bidRedouble;
}

CBidDialog::~CBidDialog()
{
    delete ui;
}

/**
 * @brief Close event for the dialog.
 * @param event The close event.
 *
 * Emits a bidClose signal and then closes the dialog.
 */
void CBidDialog::closeEvent(QCloseEvent *event)
{
    emit bidClose();
    QDialog::closeEvent(event);
}

//--------------------------------------------------------------------------
//Click of bid buttons.
void CBidDialog::on_bid1C_clicked()
{
    bidClicked(BID_1C);
}

void CBidDialog::on_bid1D_clicked()
{
    bidClicked(BID_1D);
}

void CBidDialog::on_bid1H_clicked()
{
    bidClicked(BID_1H);
}

void CBidDialog::on_bid1S_clicked()
{
    bidClicked(BID_1S);
}

void CBidDialog::on_bid1NT_clicked()
{
    bidClicked(BID_1NT);
}

void CBidDialog::on_bid2C_clicked()
{
    bidClicked(BID_2C);
}

void CBidDialog::on_bid2D_clicked()
{
    bidClicked(BID_2D);
}

void CBidDialog::on_bid2H_clicked()
{
    bidClicked(BID_2H);
}

void CBidDialog::on_bid2S_clicked()
{
    bidClicked(BID_2S);
}

void CBidDialog::on_bid2NT_clicked()
{
    bidClicked(BID_2NT);
}

void CBidDialog::on_bid3C_clicked()
{
    bidClicked(BID_3C);
}

void CBidDialog::on_bid3D_clicked()
{
    bidClicked(BID_3D);
}

void CBidDialog::on_bid3H_clicked()
{
    bidClicked(BID_3H);
}

void CBidDialog::on_bid3S_clicked()
{
    bidClicked(BID_3S);
}

void CBidDialog::on_bid3NT_clicked()
{
    bidClicked(BID_3NT);
}

void CBidDialog::on_bid4C_clicked()
{
    bidClicked(BID_4C);
}

void CBidDialog::on_bid4D_clicked()
{
    bidClicked(BID_4D);
}

void CBidDialog::on_bid4H_clicked()
{
    bidClicked(BID_4H);
}

void CBidDialog::on_bid4S_clicked()
{
    bidClicked(BID_4S);
}

void CBidDialog::on_bid4NT_clicked()
{
    bidClicked(BID_4NT);
}

void CBidDialog::on_bid5C_clicked()
{
    bidClicked(BID_5C);
}

void CBidDialog::on_bid5D_clicked()
{
    bidClicked(BID_5D);
}

void CBidDialog::on_bid5H_clicked()
{
    bidClicked(BID_5H);
}

void CBidDialog::on_bid5S_clicked()
{
    bidClicked(BID_5S);
}

void CBidDialog::on_bid5NT_clicked()
{
    bidClicked(BID_5NT);
}

void CBidDialog::on_bid6C_clicked()
{
    bidClicked(BID_6C);
}

void CBidDialog::on_bid6D_clicked()
{
    bidClicked(BID_6D);
}

void CBidDialog::on_bid6H_clicked()
{
    bidClicked(BID_6H);
}

void CBidDialog::on_bid6S_clicked()
{
    bidClicked(BID_6S);
}

void CBidDialog::on_bid6NT_clicked()
{
    bidClicked(BID_6NT);
}

void CBidDialog::on_bid7C_clicked()
{
    bidClicked(BID_7C);
}

void CBidDialog::on_bid7D_clicked()
{
    bidClicked(BID_7D);
}

void CBidDialog::on_bid7H_clicked()
{
    bidClicked(BID_7H);
}

void CBidDialog::on_bid7S_clicked()
{
    bidClicked(BID_7S);
}

void CBidDialog::on_bid7NT_clicked()
{
    bidClicked(BID_7NT);
}

void CBidDialog::on_bidPass_clicked()
{
    bidClicked(BID_PASS);
}

void CBidDialog::on_bidDouble_clicked()
{
    bidClicked(BID_DOUBLE);
}

void CBidDialog::on_bidRedouble_clicked()
{
    bidClicked(BID_REDOUBLE);
}

/**
 * @brief Catches eventually the bid clicked by the user of the dialog.
 * @param nBid The bid.
 *
 * Emits a bidValue signal.
 */
void CBidDialog::bidClicked(Bids nBid)
{
    emit bidValue(nBid);
}

/**
 * @brief Enable the bid dialog.
 * @param seat The seat to enable for.
 * @param lastBid The last proper bid given.
 * @param doubleBid Double bid (if any).
 *
 * Sets the dialog window title. Enables relevant bid buttons. Enables the dialog.
 */
void CBidDialog::enableBidder(Seat seat, Bids lastBid, Bids doubleBid)
{
    int i;

    //Set window title.
    setWindowTitle(tr("Bidder: ") + tr(SEAT_NAMES[seat]));

    //Enable proper bid buttons.
    for (i = BID_1C; i <= BID_7NT; i++)
        m_pBidButtons[i]->setEnabled(i > lastBid);

    //Enable proper double/redouble buttons.
    if (lastBid == BID_NONE)
        doubleBid = BID_NONE;

    m_pBidButtons[BID_DOUBLE]->setEnabled(doubleBid == BID_DOUBLE);
    m_pBidButtons[BID_REDOUBLE]->setEnabled(doubleBid == BID_REDOUBLE);

    //Enable the dialog.
    setEnabled(true);
}

/**
 * @brief Disable the bid dialog.
 */
void CBidDialog::disableBidder()
{  
    //Erase window title.
    setWindowTitle(tr(""));

    setEnabled(false);
}
