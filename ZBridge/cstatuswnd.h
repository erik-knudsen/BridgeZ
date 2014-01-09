/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CStatusWnd.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Status window.

  Platforms: Qt.
*/

/**
 * \file
 * Status window (declaration).
 */

#ifndef CSTATUSWND_H
#define CSTATUSWND_H

#include <QDockWidget>

class CZBridgeApp;
class CZBridgeDoc;

namespace Ui {
class CStatusWnd;
}

/**
 * @brief Shows the status window (will propably not be used).
 */
class CStatusWnd : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit CStatusWnd(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent = 0);
    ~CStatusWnd();

protected:
    virtual void closeEvent(QCloseEvent *);

signals:
    void UpdateShowAnalysis();

private slots:
    void on_cardLocationsObserver_activated(int index);
    void on_cardLocationsTarget_activated(int index);
    void on_playAnalysisShowWest_stateChanged(int arg1);
    void on_playAnalysisShowNorth_stateChanged(int arg1);
    void on_playAnalysisShowEast_stateChanged(int arg1);
    void on_playAnalysisShowSouth_stateChanged(int arg1);

    void on_cardLocationsObserver_currentIndexChanged(int index);

    void on_cardLocationsTarget_currentIndexChanged(int index);

private:
    Ui::CStatusWnd *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
};

#endif // CSTATUSWND_H
