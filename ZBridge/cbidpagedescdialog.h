/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBidPageDescDialog.h
  Developers: eak

  Revision History:
  26-apr-2016 eak: Original

  Abstract: Bid page description.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the bid page description for the bid database.
 */

#ifndef CBIDPAGEDESCDIALOG_H
#define CBIDPAGEDESCDIALOG_H

#include <QDialog>

namespace Ui {
class CBidPageDescDialog;
}

class CBidPageDescDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBidPageDescDialog(QWidget *parent = 0);
    ~CBidPageDescDialog();

    void closeEvent(QCloseEvent *event);
    void reject();

    void setPageDesc(QString &text);
    QString getPageDesc();

private:
    Ui::CBidPageDescDialog *ui;
};

#endif // CBIDPAGEDESCDIALOG_H
