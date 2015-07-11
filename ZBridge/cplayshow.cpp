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

void CPlayShow::on_reviewFirst_clicked()
{
    emit playValue(REVIEW_FIRST);
}

void CPlayShow::on_reviewPrev_clicked()
{
    emit playValue(REVIEW_PREV);
}

void CPlayShow::on_reviewNext_clicked()
{
    emit playValue(REVIEW_NEXT);
}

void CPlayShow::on_reviewLast_clicked()
{
    emit playValue(REVIEW_LAST);
}
