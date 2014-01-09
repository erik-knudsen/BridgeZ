/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CHistoryWnd.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: History of bid and play.

  Platforms: Qt.

*/

/**
 * \file
 * History of bid and play (declaration).
 */

#ifndef CHISTORYWND_H
#define CHISTORYWND_H

#include <QDockWidget>

class CZBridgeApp;
class CZBridgeDoc;

namespace Ui {
class CHistoryWnd;
}

/**
 * @brief History window for bid and play.
 *
 * This class will propably not be used.
 */
class CHistoryWnd : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit CHistoryWnd(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent = 0);
    ~CHistoryWnd();

protected:
    virtual void closeEvent(QCloseEvent *);

signals:
    void UpdateViewHistory();

private slots:
    void on_biddingButton_toggled(bool checked);
    void on_playButton_toggled(bool checked);

private:
    Ui::CHistoryWnd *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
};

#endif // CHISTORYWND_H
