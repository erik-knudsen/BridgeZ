/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CMainFrame.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Mainframe.

  Platforms: Qt.

*/

/**
 * \file
 * Mainframe.
 */

#include <QResizeEvent>
#include <QSizePolicy>

#include "cmainframe.h"
#include "ui_cmainframe.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"
#include "cplayview.h"
#include "cfilepropertiesdialog.h"
#include "caboutdlg.h"
#include "cfilecommentsdialog.h"
#include "cstatuswnd.h"
#include "chistorywnd.h"
#include "cbidoptions.h"
#include "cdealoptionspropsheet.h"
#include "cdispoptionspropsheet.h"
#include "cgameoptionspropsheet.h"
#include "cprogramconfigwizard.h"
#include "cbiddialog.h"
#include "cseatconfiguration.h"
#include "ctblmngrserver.h"
#include "ctblmngrclient.h"

//Static pointer to mainframe singleton.
CMainFrame *CMainFrame::m_pInstance = 0;

/**
 * @brief Mainframe constructor.
 *
 * Sets up the mainframe etc.:
 *
 *   - Set up mainframe (main menu etc.).
 *   - Allocate playview. This is the view where the play is visualized.
 *   - Determine (server or client) and allocate table manager. The table manager is\n
 *     the controller of what happens.
 *   - Allocate and initialize file comment dialog (EAK).
 *   - Allocate and initialize status window (EAK).
 *   - Allocate and initialize history window (EAK).
 *
 * @param app Pointer to application.
 * @param doc Pointer to data container.
 */
CMainFrame::CMainFrame(CZBridgeApp *app, CZBridgeDoc *doc) :
    ui(new Ui::CMainFrame)
{
    this->app = app;
    this->doc = doc;

    //Set up user interface (main menu etc.)
    ui->setupUi(this);

    //Allocate and initialize play view.
    playView = new CPlayView(this);

    //Determine, allocate and initialize table manager.
    if ((doc->getSeatOptions().role == STANDALONE_ROLE) || (doc->getSeatOptions().role == SERVER_ROLE))
        tableManager = new CTblMngrServer(doc, playView, this);
    else
        tableManager = new CTblMngrClient(doc, playView, this);

    //Initialization of main frame window.
    setCentralWidget(playView);
    setWindowIcon(QIcon(":/newPrefix/resources/ZBridgeICN.bmp"));

    resize(QSize(SCENE_HOR_SIZE + 30, SCENE_VER_SIZE + 130));
    setMinimumSize(QSize(SCENE_HOR_SIZE * .75, SCENE_VER_SIZE * .75));
    setMaximumSize(QSize(SCENE_HOR_SIZE * 1.25, SCENE_VER_SIZE * 1.25));

    //File comment dialog (EAK).
    m_pFileCommentsDlg = new CFileCommentsDialog(app, doc, this);
    connect(m_pFileCommentsDlg, &CFileCommentsDialog::UpdateViewFileComments, this, &CMainFrame::OnUpdateViewFileComments);

    //Status window (EAK)
    m_pWndStatus = new CStatusWnd(app, doc, this);
    addDockWidget(Qt::BottomDockWidgetArea, m_pWndStatus);
    m_pWndStatus->hide();
    connect(m_pWndStatus, &CStatusWnd::UpdateShowAnalysis, this, &CMainFrame::OnUpdateShowAnalysis);

    //History window (EAK).
    m_pWndHistory = new CHistoryWnd(app, doc, this);
    addDockWidget(Qt::LeftDockWidgetArea, m_pWndHistory);
    m_pWndHistory->hide();
    connect(m_pWndHistory, &CHistoryWnd::UpdateViewHistory, this, &CMainFrame::OnUpdateViewHistory);
 }

CMainFrame::~CMainFrame()
{
    delete ui;
}

/**
 * @brief Get pointer to mainframe singleton instance.
 *
 * @return Pointer to mainframe singleton instance.
 */
CMainFrame* CMainFrame::Instance()
{
    return m_pInstance;
}

/**
 * @brief Handle resize of mainframe (and children) windows.
 *
 * @param resizeEvent Parameters for resize.
 */
void CMainFrame::resizeEvent(QResizeEvent *resizeEvent)
{
    QSize size = resizeEvent->size();
    QSize oldSize = resizeEvent->oldSize();
    if (oldSize.width() > 0)
    {
       float scaleXFactor = float(size.width() - 30)/(oldSize.width() - 30);
       float scaleYFactor = float(size.height() - 130)/(oldSize.height() - 130);
       playView->scale(scaleXFactor, scaleYFactor);
    }
}

/**
 * @brief Custom event handling.
 *
 * Handles events for disabling/enabling of main menu entries.
 *
 * @param event The event type. Only update ui events are handled.
 */
void CMainFrame::customEvent(QEvent *event)
{
    if (event->type() == WMS_UPDATE_UI_ACTION)
    {
        UPDATE_UI_ACTION_Event * ev = static_cast<UPDATE_UI_ACTION_Event *>(event);

        UpdateUIAction uiAction = ev->getUIAction();
        bool param = ev->getEnable();

        switch (uiAction)
        {
            //Enable/Disable for initial main menu entries.
        case UPDATE_UI_INITIAL:
            enableUIActionsInitial(param);
            break;

            //Enable/Disable for initial main menu entries for table manager server.
        case UPDATE_UI_SERVER:
            enableUIActionsServer(param);
            break;

            //Enable/Disable for initial main menu entries for table manager client.
        case UPDATE_UI_CLIENT:
            enableUIActionsClient(param);
            break;

            //Enable/Disable new session menu entry.
        case UPDATE_UI_NEW_SESSION:
            ui->actionNew_Session->setEnabled(param);
            break;

            //Enable/Disable new deal menu entry.
        case UPDATE_UI_NEW_DEAL:
            ui->action_Deal_New_Hand->setEnabled(param);
            break;

            //Enable/Disable undo bid or trick menu entry.
        case UPDATE_UI_UNDO:
            ui->actionUndo->setEnabled(param);
            break;

            //Enable/Disable rebid menu entry.
        case UPDATE_UI_REBID:
            ui->action_Bid_Rebid->setEnabled(param);
            break;

            //Enable/Disable replay menu entry.
        case UPDATE_UI_REPLAY:
            ui->action_Restart_Hand->setEnabled(param);
            break;

            //Enable/Disable show all menu entry.
        case UPDATE_UI_SHOW_ALL:
            ui->action_Expose_All_Cards->setEnabled(param);
            break;
        }
    }
}

/**
 * @brief Enable main menu entries initially (before server or client mode is determined).
 *
 * @param advProtocol If true use advanced protocol else use basic protocol.
 */
void CMainFrame::enableUIActionsInitial(bool advProtocol)
{
    enableUIActions(INITIAL_ACTIONS, advProtocol);
}

/**
 * @brief Enable main menu entries initially for server mode.
 *
 * @param advProtocol If true use advanced protocol else use basic protocol.
 */
void CMainFrame::enableUIActionsServer(bool advProtocol)
{
    enableUIActions(SERVER_ACTIONS, advProtocol);
}

/**
 * @brief Enable main menu entries initially for client mode.
 *
 * @param advProtocol If true use advanced protocol else use basic protocol.
 */
void CMainFrame::enableUIActionsClient(bool advProtocol)
{
    enableUIActions(CLIENT_ACTIONS, advProtocol);
}

/**
 * @brief Enable/disable main menu entries.
 *
 * @param actions identifies which actions to intially enable/disable for.
 *        - INITIAL_ACTIONS before server/client mode is determined.
 *        - SERVER_ACTIONS for server mode.
 *        - CLIENT_ACTIONS for client mode.
 *
 * @param advProtocol identifies whic protocol to use (advanced or basic).
 */
void CMainFrame::enableUIActions(actionIndicator actions, bool advProtocol)
{
    ui->actionOpen->setEnabled((actions == INITIAL_ACTIONS) || (actions == SERVER_ACTIONS));
    ui->actionRecent_File->setEnabled((actions == INITIAL_ACTIONS) || (actions == SERVER_ACTIONS));
    ui->actionClear_All->setEnabled((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS));
    ui->actionBidding_Play_History->setEnabled((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS));
    ui->actionClaim_All->setEnabled(((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS)) && advProtocol);
    ui->actionClaim_Contract->setEnabled(((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS)) && advProtocol);
    ui->actionConcede->setEnabled(((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS)) && advProtocol);
    ui->actionHint->setEnabled(((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS)) && advProtocol);
    ui->actionAuto_Hints->setEnabled(((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS)) && advProtocol);
    ui->actionAuto_Play_Card->setEnabled((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS));
    ui->actionAuto_Play_All_Cards->setEnabled((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS));
    ui->actionAuto_Play_to_Completion->setEnabled((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS));


    ui->actionNew_Session->setEnabled(true);
    ui->action_Deal_New_Hand->setEnabled(false);

    ui->action_Bid_Rebid->setEnabled(false);
    ui->action_Restart_Hand->setEnabled(false);
    ui->actionUndo->setEnabled(false);

    ui->action_Expose_All_Cards->setEnabled(false);
}

/**
 * @brief CMainFrame::OnUpdateViewFileComments
 * EAK
 */
void CMainFrame::OnUpdateViewFileComments()
{
    ui->action_File_Comments->setChecked(false);
}

/**
 * @brief CMainFrame::OnUpdateViewHistory
 * EAK
 */
void CMainFrame::OnUpdateViewHistory()
{
    ui->actionBidding_Play_History->setChecked(false);
}

/**
 * @brief CMainFrame::OnUpdateShowAnalysis
 * EAK
 */
void CMainFrame::OnUpdateShowAnalysis()
{
    ui->actionShow_Player_Analyses->setChecked(false);
}

/**
 * @brief CMainFrame::createPopupMenu
 *
 * Not used yet.
 *
 * @return
 */
QMenu *CMainFrame::createPopupMenu()
{
    return 0;
}

/**
 * @brief Set text in status line.
 *
 * @param text is the text to set.
 */
void CMainFrame::SetStatusText(QString text)
{
    ui->statusBar->showMessage(text);
}

/**
 * @brief Clear text in status line.
 */
void CMainFrame::ResetStatusText()
{
    ui->statusBar->clearMessage();
}

//Following methods are automatically generated main menu actions.
//----------------------------------------------------------------
void CMainFrame::on_actionOpen_triggered()
{

}

void CMainFrame::on_actionSave_triggered()
{

}

void CMainFrame::on_actionSave_As_triggered()
{

}

void CMainFrame::on_actionPrint_triggered()
{

}

void CMainFrame::on_actionPrint_PreView_triggered()
{

}

/**
 * @brief Main menu properties triggered.
 */
void CMainFrame::on_actionProperties_triggered()
{
    //Show properties dialog.
    CFilePropertiesDialog properties(app, doc);
    properties.exec();
}

void CMainFrame::on_actionRecent_File_triggered()
{

}

void CMainFrame::on_actionExit_triggered()
{

}

void CMainFrame::on_actionCu_t_triggered()
{

}

void CMainFrame::on_action_Copy_triggered()
{

}

void CMainFrame::on_action_Paste_triggered()
{

}

void CMainFrame::on_actionClear_All_triggered()
{

}

/**
 * @brief Main menu file comments triggered.
 */
void CMainFrame::on_action_File_Comments_triggered()
{
    //Togle hide and show.
    if (m_pFileCommentsDlg->isVisible())
        m_pFileCommentsDlg->hide();
    else
        m_pFileCommentsDlg->show();
}

/**
 * @brief Main menu bidding play history triggered.
 */
void CMainFrame::on_actionBidding_Play_History_triggered()
{
    //Togle hide and show.
    if (m_pWndHistory->isVisible())
        m_pWndHistory->hide();
    else
        m_pWndHistory->show();
}

void CMainFrame::on_actionRubber_Score_triggered()
{

}

void CMainFrame::on_action_Toolbar_triggered()
{

}

void CMainFrame::on_actionSecondary_Toolbar_triggered()
{

}

void CMainFrame::on_actionTest_Toolbar_triggered()
{

}

void CMainFrame::on_actionStatus_Bar_triggered()
{

}

void CMainFrame::on_action_Refresh_Screen_triggered()
{

}

/**
 * @brief Main menu new session triggered.
 */
void CMainFrame::on_actionNew_Session_triggered()
{
    tableManager->newSession();
}

/**
 * @brief Main menu new deal triggered.
 */
void CMainFrame::on_action_Deal_New_Hand_triggered()
{
    tableManager->newDeal();
}

void CMainFrame::on_action_Play_Rubber_triggered()
{

}

void CMainFrame::on_actionUndo_triggered()
{

}

void CMainFrame::on_action_Bid_Rebid_triggered()
{

}

void CMainFrame::on_action_Restart_Hand_triggered()
{

}

void CMainFrame::on_actionClaim_All_triggered()
{

}

void CMainFrame::on_actionClaim_Contract_triggered()
{

}

void CMainFrame::on_actionConcede_triggered()
{

}

void CMainFrame::on_actionHint_triggered()
{

}

void CMainFrame::on_actionAuto_Hints_triggered()
{

}

void CMainFrame::on_actionAuto_Play_Card_triggered()
{

}

void CMainFrame::on_actionAuto_Play_All_Cards_triggered()
{

}

void CMainFrame::on_actionAuto_Play_to_Completion_triggered()
{

}

void CMainFrame::on_actionReview_Game_triggered()
{

}

void CMainFrame::on_actionAuto_Test_triggered()
{

}

void CMainFrame::on_action_deal_Num_bered_Hand_triggered()
{

}

void CMainFrame::on_actionDeal_Game_Hands_triggered()
{

}

void CMainFrame::on_actionDeal_Major_Suit_Game_triggered()
{

}

void CMainFrame::on_actionDeal_Mi_nor_Suit_Game_triggered()
{

}

void CMainFrame::on_actionDeal_No_Trump_Game_triggered()
{

}

void CMainFrame::on_actionDeal_Game_Hands_to_East_West_triggered()
{

}

void CMainFrame::on_actionDeal_Minor_Suit_Game_to_East_West_triggered()
{

}

void CMainFrame::on_actionDeal_No_Trump_Game_to_East_West_triggered()
{

}

void CMainFrame::on_actionDeal_Slam_Hands_triggered()
{

}

void CMainFrame::on_actionDeal_Sma_ll_Slam_triggered()
{

}

void CMainFrame::on_actionDeal_Gran_d_Slam_triggered()
{

}

void CMainFrame::on_actionDeal_Slam_Hands_to_East_West_triggered()
{

}

void CMainFrame::on_actionDeal_Small_Slam_to_East_West_triggered()
{

}

void CMainFrame::on_actionDeal_Grand_Slam_to_East_West_triggered()
{

}

void CMainFrame::on_actionWest_3_triggered()
{

}

void CMainFrame::on_actionNorth_3_triggered()
{

}

void CMainFrame::on_actionEast_3_triggered()
{

}

void CMainFrame::on_actionSwap_Cards_Clockwisr_triggered()
{

}

void CMainFrame::on_actionSwap_Cards_Coun_terclockwise_triggered()
{

}

void CMainFrame::on_action_Lay_Out_Cards_triggered()
{

}

void CMainFrame::on_action_Edit_Existing_Hands_triggered()
{

}

void CMainFrame::on_action_Distribute_Remaining_Cards_triggered()
{

}

/**
 * @brief Main menu seat configuration triggered.
 *
 * Activate seat configuration dialog
 */
void CMainFrame::on_actionSeat_Configuration_triggered()
{
    CSeatConfiguration seatConfiguration(app, doc, this);
    if (seatConfiguration.exec() == QDialog::Accepted)
    {
        //Seat configuration has changed. The table manager is deleted
        //and a new table manager (server or client) is allocated and initialized.
        doc->WriteSeatOptions();

        delete tableManager;
        if ((doc->getSeatOptions().role == STANDALONE_ROLE) || (doc->getSeatOptions().role == SERVER_ROLE))
            tableManager = new CTblMngrServer(doc, playView, this);
        else
            tableManager = new CTblMngrClient(doc, playView, this);
    }

}

/**
 * @brief main menu bidding options triggered.
 *
 * Activate bid options dialog.
 */
void CMainFrame::on_action_Bidding_Options_triggered()
{
    CBidOptions bidOptsDialog(app, doc, this);
    if (bidOptsDialog.exec() == QDialog::Accepted)
        doc->WriteSettings();
}

/**
 * @brief main menu deal options triggered.
 *
 * Activate deal options dialog.
 */
void CMainFrame::on_action_Deal_Options_triggered()
{
    CDealOptionsPropSheet dealOptsDialog(app, doc, this);
    if (dealOptsDialog.exec() == QDialog::Accepted)
        doc->WriteDealOptions();
}

/**
 * @brief main menu display options triggered.
 *
 * Activate display options dialog.
 */
void CMainFrame::on_action_Di_splay_Options_triggered()
{
    CDispOptionsPropSheet dispOptsDialog(app, doc, this);
    if (dispOptsDialog.exec() == QDialog::Accepted)
        doc->WriteDisplayOptions();
}

/**
 * @brief main menu game options triggered.
 *
 * Activate game options dialog.
 */
void CMainFrame::on_action_Game_Options_triggered()
{
    CGameOptionsPropSheet gameOptsDialog(app, doc, this);
    if (gameOptsDialog.exec() == QDialog::Accepted)
        doc->WriteGameOptions();
}

/**
 * @brief main menu configuration wizard triggered.
 *
 * Activate configuration wizard.
 */
void CMainFrame::on_actionConfiguration_Wizard_triggered()
{
    CProgramConfigWizard programConfigWizard(app, doc, this);
    if (programConfigWizard.exec() == QDialog::Accepted)
    {
        doc->WriteWizardOptions();
        doc->synchronizeOptions(true);
    }
}

void CMainFrame::on_action_Expose_All_Cards_triggered()
{
    tableManager->showAllCards();
}

void CMainFrame::on_actionEnable_Analysis_Tracing_triggered()
{

}

void CMainFrame::on_actionShow_Player_Analyses_triggered()
{
    if (m_pWndStatus->isVisible())
        m_pWndStatus->hide();
    else
        m_pWndStatus->show();
}

void CMainFrame::on_actionManual_Play_triggered()
{

}

void CMainFrame::on_actionManual_Defend_triggered()
{

}

void CMainFrame::on_action_Normal_Play_Mode_triggered()
{

}

void CMainFrame::on_actionManual_Play_Mode_triggered()
{

}

void CMainFrame::on_actionManual_Defend_2_triggered()
{

}

void CMainFrame::on_actionAuto_Play_Mode_triggered()
{

}

void CMainFrame::on_actionLock_Play_Mode_triggered()
{

}

void CMainFrame::on_actionShow_Comment_IDs_triggered()
{

}

void CMainFrame::on_action_Contents_triggered()
{

}

void CMainFrame::on_actionIn_troduction_triggered()
{

}

void CMainFrame::on_action_Rules_of_the_Game_triggered()
{

}

void CMainFrame::on_action_Menu_Commands_triggered()
{

}

void CMainFrame::on_actionMi_scellaneous_Topics_triggered()
{

}

void CMainFrame::on_action_Glossary_of_Terms_triggered()
{

}

void CMainFrame::on_action_Search_for_Help_On_triggered()
{

}

void CMainFrame::on_actionQuick_Start_triggered()
{

}

void CMainFrame::on_actionFrequently_Asked_Questions_triggered()
{

}

void CMainFrame::on_actionTip_of_the_Day_triggered()
{

}

void CMainFrame::on_action_View_README_File_triggered()
{

}

void CMainFrame::on_action_About_ZBridge_triggered()
{
    CAboutDlg about(app, doc, this);
    about.exec();
}

void CMainFrame::on_actionDeal_Major_Suit_Game_to_East_West_triggered()
{

}

void CMainFrame::on_actionManual_Bidding_triggered()
{

}
