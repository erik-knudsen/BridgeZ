#include "ccomparescoredialog.h"
#include "ui_ccomparescoredialog.h"

CCompareScoreDialog::CCompareScoreDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CCompareScoreDialog)
{
    ui->setupUi(this);
}

CCompareScoreDialog::~CCompareScoreDialog()
{
    delete ui;
}
