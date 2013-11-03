/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CFileCommentsDialog.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Dialog for play file comments.

  Platforms: Qt.

*/

/**
 * \file
 * Dialog for play file comments (definition).
 */

#include <QtGui>

#include "Defines.h"
#include "cfilecommentsdialog.h"
#include "ui_cfilecommentsdialog.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"

CFileCommentsDialog::CFileCommentsDialog(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFileCommentsDialog)
{
    ui->setupUi(this);

    this->app = app;
    this->doc = doc;

    UpdateText(false);
}

CFileCommentsDialog::~CFileCommentsDialog()
{
    delete ui;
}

void CFileCommentsDialog::on_autoShow_clicked()
{
    doc->setShowCommentsUponOpen(ui->autoShow->isChecked());
}

void CFileCommentsDialog::customEvent(QEvent * event)
{
    if (event->type() == WMS_UPDATE_TEXT)
    {
        WMS_UPDATE_TEXT_Event * ev = static_cast<WMS_UPDATE_TEXT_Event *>(event);
        UpdateText(ev->getget());
    }
}

void CFileCommentsDialog::UpdateText(bool bUpdateVariable)
{
    if (bUpdateVariable)
    {
        // Update main document string.
        QString strText;
        strText = ui->strText->toPlainText();
        doc->setFileComments(strText);
    }
    else
    {
        // Update window text display:
        ui->strText->setPlainText(doc->getFileComments());
        ui->autoShow->setChecked(doc->getShowCommentsUponOpen());
    }
}

void CFileCommentsDialog::closeEvent(QCloseEvent * e)
{
    emit UpdateViewFileComments();
    e->accept();
}
