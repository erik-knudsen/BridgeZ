/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: ChistoryWnd.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: History of bid and play.

  Platforms: Qt.

*/

/**
 * \file
 * History of bid and play (definition).
 */

#include "chistorywnd.h"
#include "ui_chistorywnd.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"

CHistoryWnd::CHistoryWnd(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::CHistoryWnd)
{
    ui->setupUi(this);

    this->app = app;
    this->doc = doc;
}

CHistoryWnd::~CHistoryWnd()
{
    delete ui;
}

void CHistoryWnd::on_biddingButton_toggled(bool checked)
{
    if (checked)
        ui->bidding->show();
    else
        ui->bidding->hide();
}

void CHistoryWnd::on_playButton_toggled(bool checked)
{
    if (checked)
        ui->play->show();
    else
        ui->play->hide();
}

void CHistoryWnd::closeEvent(QCloseEvent * e)
{
    emit UpdateViewHistory();
    e->accept();
}
