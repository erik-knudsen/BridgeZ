/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBidPageDescDialog.cpp
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid page description.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the bid page description for the bid database.
 */

#include "cbidpagedescdialog.h"
#include "ui_cbidpagedescdialog.h"

CBidPageDescDialog::CBidPageDescDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBidPageDescDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

CBidPageDescDialog::~CBidPageDescDialog()
{
    delete ui;
}

void CBidPageDescDialog::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void CBidPageDescDialog::reject()
{
    hide();
}

void CBidPageDescDialog::setPageDesc(QString &text)
{
    ui->pageDesc->setPlainText(text);
}

QString CBidPageDescDialog::getPageDesc()
{
    return ui->pageDesc->toPlainText();
}
