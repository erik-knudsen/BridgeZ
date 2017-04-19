#include "cpopup.h"
#include "ui_cpopup.h"

CPopup::CPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPopup)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_ShowWithoutActivating);
    setWindowFlags(
                Qt::Window | // Add if popup doesn't show up
                Qt::FramelessWindowHint | // No window border
                Qt::WindowDoesNotAcceptFocus | // No focus
                Qt::WindowStaysOnTopHint // Always on top
                );
}

CPopup::~CPopup()
{
    delete ui;
}

void CPopup::showPopup(QString description)
{
    ui->description->setText(description);


    // resize the widget, as text label may be larger than the previous size
//    setGeometry(QStyle::alignedRect(
//                    Qt::RightToLeft,
//                    Qt::AlignBottom,
//                    size(),
//                    qApp->desktop()->availableGeometry()));
    show();
}

