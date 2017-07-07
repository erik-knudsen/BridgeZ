/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CMainFrame.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Mainframe

  Platforms: Qt.

*/

/**
 * \file
 * Mainframe.
 */

#ifndef CMAINFRAME_H
#define CMAINFRAME_H

#include <QMainWindow>
#include <QList>
#include <QHostAddress>
#include <QSemaphore>

namespace Ui {
class CMainFrame;
}
class CZBridgeApp;
class CZBridgeDoc;
class CGamesDoc;
class CPlayView;
class CTblMngr;
class CTblMngrBase;
class CSeatConfiguration;
class CBidDialog;
class CMainScoreDialog;
class CBidAndPlayEngines;

//Enums for initial enable/disable of menu items.
enum actionIndicator { INITIAL_ACTIONS, SERVER_ACTIONS, CLIENT_ACTIONS };


/**
 * @brief Main frame window. All other windows are controlled from this window.
 *
 */
class CMainFrame : public QMainWindow
{
    Q_OBJECT
    
public:
    static CMainFrame* Instance();
    explicit CMainFrame(CZBridgeApp *app, CZBridgeDoc *doc, CGamesDoc *games);
    ~CMainFrame();

    virtual QMenu *createPopupMenu();

    //Text for status line of mainframe window.
    void SetStatusText(QString text);
    void ResetStatusText();

    //Initial enable/disable for main menu.
    void enableUIActionsInitial();  //Server/client mode not determined yet.
    void enableUIActionsServer();   //Server mode.
    void enableUIActionsClient();   //Client mode.

signals:
    void sAutoQuit();
    void sNewSession();

public slots:
    void sStatusText(QString text);

protected:
    virtual void resizeEvent(QResizeEvent *resizeEvent);

private:
    QHostAddress getHostAddress(QString host);
    virtual void customEvent(QEvent *event);

    void enableUIActions(actionIndicator actions);
    void resetPlay();
    void open(QString &originalFileName);    
    void updateRecentActionList();
    void adjustForCurrentFile(const QString& filePath, bool add);

    Ui::CMainFrame *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
    CGamesDoc *games;
    CPlayView *playView;
    CTblMngr *tableManager;
    CTblMngrBase *tableManagerAuto;
    static CMainFrame* m_pInstance;

    CMainScoreDialog *mainScoreDialog;
    CBidAndPlayEngines *bidAndPlayEngines;

    int eventIndex;             //PBN event index.
    QString fileName;           //PBN file name.
    QHostAddress hostAddress;
    QSemaphore sem1, sem2;

    QList<QAction *> recentFileActionList;

private slots:
    //Menu actions.
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionDelete_triggered();
    void on_actionPrint_triggered();
    void openRecent();
    void on_actionExit_triggered();

    void on_action_Lay_Out_Cards_triggered();

    void on_action_Expose_All_Cards_triggered();
    void on_action_Score_triggered();
    void on_actionDouble_Dummy_Results_triggered();
    void on_action_Toolbar_triggered();
    void on_actionStatus_Bar_triggered();

    void on_actionNew_Session_triggered();
    void on_action_Deal_New_Hand_triggered();
    void on_actionActivate_Deal_Profile_triggered();
    void on_actionUndo_triggered();
    void on_action_Bid_Rebid_triggered();
    void on_action_Restart_Hand_triggered();
    void on_actionClaim_All_triggered();
    void on_actionClaim_Contract_triggered();
    void on_actionConcede_triggered();

    void on_actionSeat_Configuration_triggered();
    void on_action_Bidding_Options_triggered();
    void on_action_Deal_Options_triggered();
    void on_action_Di_splay_Options_triggered();
    void on_action_Game_Options_triggered();

    void on_actionSelect_Bid_Database_triggered();
    void on_actionEdit_Bid_Database_triggered();

    void on_action_Contents_triggered();
    void on_action_About_ZBridge_triggered();
};

#endif // CMAINFRAME_H
