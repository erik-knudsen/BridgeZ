#ifndef CPLAYSHOW_H
#define CPLAYSHOW_H

#include <QDialog>

namespace Ui {
class CPlayShow;
}

enum ReviewVal { REVIEW_FIRST, REVIEW_PREV, REVIEW_NEXT, REVIEW_LAST};

class CPlayShow : public QDialog
{
    Q_OBJECT

public:
    explicit CPlayShow(QWidget *parent = 0);
    ~CPlayShow();

    void closeEvent(QCloseEvent *event);
    void reject();

signals:
    void playValue(ReviewVal reviewVal);
    void playClose();

private slots:
    void on_reviewFirst_clicked();
    void on_reviewPrev_clicked();
    void on_reviewNext_clicked();
    void on_reviewLast_clicked();

private:
    Ui::CPlayShow *ui;
};

#endif // CPLAYSHOW_H
