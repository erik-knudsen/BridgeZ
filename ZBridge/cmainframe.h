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

namespace Ui {
class CMainFrame;
}
class CZBridgeApp;
class CZBridgeDoc;
class CGamesDoc;
class CPlayView;
class CTblMngr;
class CFileCommentsDialog;
class CStatusWnd;
class CHistoryWnd;
class CSeatConfiguration;
class CBidDialog;

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
    void enableUIActionsInitial(bool advProtocol);  //Server/client mode not determined yet.
    void enableUIActionsServer(bool advProtocol);   //Server mode.
    void enableUIActionsClient(bool advProtocol);   //Client mode.

protected:
    virtual void resizeEvent(QResizeEvent *resizeEvent);

private:
    virtual void customEvent(QEvent *event);

    void enableUIActions(actionIndicator actions, bool advProtocol);

    Ui::CMainFrame *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
    CGamesDoc *games;
    CPlayView *playView;
    CTblMngr *tableManager;

    static CMainFrame* m_pInstance;

    CFileCommentsDialog *m_pFileCommentsDlg;
    CStatusWnd *m_pWndStatus;
    CHistoryWnd *m_pWndHistory;

    int eventIndex;
    QString fileName;

private slots:
    void OnUpdateViewFileComments();
    void OnUpdateViewHistory();
    void OnUpdateShowAnalysis();

    //Menu actions.
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionPrint_triggered();
    void on_actionPrint_PreView_triggered();
    void on_actionProperties_triggered();
    void on_actionRecent_File_triggered();
    void on_actionExit_triggered();
    void on_actionCu_t_triggered();
    void on_action_Copy_triggered();
    void on_action_Paste_triggered();
    void on_actionClear_All_triggered();
    void on_action_File_Comments_triggered();
    void on_actionBidding_Play_History_triggered();
    void on_action_Score_triggered();
    void on_action_Toolbar_triggered();
    void on_actionSecondary_Toolbar_triggered();
    void on_actionTest_Toolbar_triggered();
    void on_actionStatus_Bar_triggered();
    void on_action_Refresh_Screen_triggered();
    void on_action_Deal_New_Hand_triggered();
    void on_action_Play_Rubber_triggered();
    void on_actionUndo_triggered();
    void on_action_Bid_Rebid_triggered();
    void on_action_Restart_Hand_triggered();
    void on_actionClaim_All_triggered();
    void on_actionClaim_Contract_triggered();
    void on_actionConcede_triggered();
    void on_actionHint_triggered();
    void on_actionAuto_Hints_triggered();
    void on_actionAuto_Play_Card_triggered();
    void on_actionAuto_Play_All_Cards_triggered();
    void on_actionAuto_Play_to_Completion_triggered();
    void on_actionReview_Game_triggered();
    void on_actionAuto_Test_triggered();
    void on_action_deal_Num_bered_Hand_triggered();
    void on_actionDeal_Game_Hands_triggered();
    void on_actionDeal_Major_Suit_Game_triggered();
    void on_actionDeal_Mi_nor_Suit_Game_triggered();
    void on_actionDeal_No_Trump_Game_triggered();
    void on_actionDeal_Game_Hands_to_East_West_triggered();
    void on_actionDeal_Minor_Suit_Game_to_East_West_triggered();
    void on_actionDeal_No_Trump_Game_to_East_West_triggered();
    void on_actionDeal_Slam_Hands_triggered();
    void on_actionDeal_Sma_ll_Slam_triggered();
    void on_actionDeal_Gran_d_Slam_triggered();
    void on_actionDeal_Slam_Hands_to_East_West_triggered();
    void on_actionDeal_Small_Slam_to_East_West_triggered();
    void on_actionDeal_Grand_Slam_to_East_West_triggered();
    void on_actionWest_3_triggered();
    void on_actionNorth_3_triggered();
    void on_actionEast_3_triggered();
    void on_actionSwap_Cards_Clockwisr_triggered();
    void on_actionSwap_Cards_Coun_terclockwise_triggered();
    void on_action_Lay_Out_Cards_triggered();
    void on_action_Edit_Existing_Hands_triggered();
    void on_action_Distribute_Remaining_Cards_triggered();
    void on_action_Bidding_Options_triggered();
    void on_action_Deal_Options_triggered();
    void on_action_Di_splay_Options_triggered();
    void on_action_Game_Options_triggered();
    void on_actionConfiguration_Wizard_triggered();
    void on_action_Expose_All_Cards_triggered();
    void on_actionEnable_Analysis_Tracing_triggered();
    void on_actionShow_Player_Analyses_triggered();
    void on_actionManual_Play_triggered();
    void on_actionManual_Defend_triggered();
    void on_action_Normal_Play_Mode_triggered();
    void on_actionManual_Play_Mode_triggered();
    void on_actionManual_Defend_2_triggered();
    void on_actionAuto_Play_Mode_triggered();
    void on_actionLock_Play_Mode_triggered();
    void on_actionShow_Comment_IDs_triggered();
    void on_action_Contents_triggered();
    void on_actionIn_troduction_triggered();
    void on_action_Rules_of_the_Game_triggered();
    void on_action_Menu_Commands_triggered();
    void on_actionMi_scellaneous_Topics_triggered();
    void on_action_Glossary_of_Terms_triggered();
    void on_action_Search_for_Help_On_triggered();
    void on_actionQuick_Start_triggered();
    void on_actionFrequently_Asked_Questions_triggered();
    void on_actionTip_of_the_Day_triggered();
    void on_action_View_README_File_triggered();
    void on_action_About_ZBridge_triggered();
    void on_actionDeal_Major_Suit_Game_to_East_West_triggered();
    void on_actionManual_Bidding_triggered();
    void on_actionSeat_Configuration_triggered();
    void on_actionNew_Session_triggered();
};

#endif // CMAINFRAME_H
