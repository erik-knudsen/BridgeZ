/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CFileCommentsDialog.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Dialog for plsy file comments.

  Platforms: Qt.

*/

/**
 * \file
 * Dialog for play file comments (declaration).
 */

#ifndef CFILECOMMENTSDIALOG_H
#define CFILECOMMENTSDIALOG_H

#include <QDialog>

class QScrollArea;
class QVBoxLayout;
class CZBridgeApp;
class CZBridgeDoc;

namespace Ui {
class CFileCommentsDialog;
}

class CFileCommentsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CFileCommentsDialog(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent = 0);
    ~CFileCommentsDialog();

protected:
    virtual void closeEvent(QCloseEvent *);

signals:
    void UpdateViewFileComments();

private:
    virtual void customEvent(QEvent *event);
    void UpdateText(bool bUpdateVariable);

private:
    Ui::CFileCommentsDialog *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;

private slots:
    void on_autoShow_clicked();
};

#endif // CFILECOMMENTSDIALOG_H
