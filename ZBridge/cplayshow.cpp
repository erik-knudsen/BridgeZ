/* Erik Aagaard Knudsen.
  Copyright © 2015 - All Rights Reserved

  Project: ZBridge
  File: CPlayShow.cpp
  Developers: eak

  Revision History:
  13-jul-2015 eak: Original

  Abstract: Dialog for traversing card play.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of a class for traversing the play for one board.
 */

#include "cplayshow.h"
#include "ui_cplayshow.h"

CPlayShow::CPlayShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPlayShow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

CPlayShow::~CPlayShow()
{
    delete ui;
}

/**
 * @brief Enable or disable one of the two buttons.
 */
void CPlayShow::setEnabled(ReviewVal reviewVal, bool enabled)
{
    if (reviewVal == REVIEW_PREV)
        ui->reviewPrev->setEnabled(enabled);
    else if (reviewVal == REVIEW_NEXT)
        ui->reviewNext->setEnabled(enabled);
}

/**
 * @brief Get enable/disable status of one of the two buttons.
 */
bool CPlayShow::isEnabled(ReviewVal reviewVal)
{
    if (reviewVal == REVIEW_PREV)
        return ui->reviewPrev->isEnabled();
    else
        return ui->reviewNext->isEnabled();
}

/**
 * @brief Close event for the dialog.
 * @param event The close event.
 *
 * Emits a bidClose signal and then closes the dialog.
 */
void CPlayShow::closeEvent(QCloseEvent *event)
{
    emit playClose();

    QDialog::closeEvent(event);
}

void CPlayShow::reject()
{
    close();
}

/**
 * @brief Backward button was clicked. Emit REVIEW_PREV signal.
 */
void CPlayShow::on_reviewPrev_clicked()
{
    emit playValue(REVIEW_PREV);
}

/**
 * @brief Forward button was clicked. Emit REVIEW_NEXT signal.
 */
void CPlayShow::on_reviewNext_clicked()
{
    emit playValue(REVIEW_NEXT);
}
