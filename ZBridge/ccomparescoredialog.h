#ifndef CCOMPARESCOREDIALOG_H
#define CCOMPARESCOREDIALOG_H

#include <QWidget>

namespace Ui {
class CCompareScoreDialog;
}

class CCompareScoreDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CCompareScoreDialog(QWidget *parent = 0);
    ~CCompareScoreDialog();

private:
    Ui::CCompareScoreDialog *ui;
};

#endif // CCOMPARESCOREDIALOG_H
