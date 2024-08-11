#ifndef CPOPUP_H
#define CPOPUP_H

#include <QDialog>
//#include <QDesktopWidget>
#include <QMouseEvent>

namespace Ui {
class CPopup;
}

class CPopup : public QDialog
{
    Q_OBJECT

public:
    explicit CPopup(QWidget *parent = 0);
    ~CPopup();

    void showPopup(QString description);

private:
    Ui::CPopup *ui;
};

#endif // CPOPUP_H
