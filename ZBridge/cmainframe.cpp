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
 * Mainframe (definition).
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

CMainFrame *CMainFrame::m_pInstance = 0;

CMainFrame::CMainFrame(CZBridgeApp *app, CZBridgeDoc *doc) :
    ui(new Ui::CMainFrame)
{
    this->app = app;
    this->doc = doc;

    ui->setupUi(this);

    playView = new CPlayView(this);
    if (ROLES[doc->getSeatOptions().role] == SERVER_ROLE)
        tableManager = new CTblMngrServer(doc, playView, this);
    else
        tableManager = new CTblMngrClient(doc, playView, this);

    setCentralWidget(playView);
    setWindowIcon(QIcon(":/newPrefix/resources/ZBridgeICN.bmp"));

    resize(QSize(SCENE_HOR_SIZE + 30, SCENE_VER_SIZE + 130));
    setMinimumSize(QSize(SCENE_HOR_SIZE * .75, SCENE_VER_SIZE * .75));
    setMaximumSize(QSize(SCENE_HOR_SIZE * 1.25, SCENE_VER_SIZE * 1.25));

    m_pFileCommentsDlg = new CFileCommentsDialog(app, doc, this);
    connect(m_pFileCommentsDlg, &CFileCommentsDialog::UpdateViewFileComments, this, &CMainFrame::OnUpdateViewFileComments);

    m_pWndStatus = new CStatusWnd(app, doc, this);
    addDockWidget(Qt::BottomDockWidgetArea, m_pWndStatus);
    m_pWndStatus->hide();
    connect(m_pWndStatus, &CStatusWnd::UpdateShowAnalysis, this, &CMainFrame::OnUpdateShowAnalysis);

    m_pWndHistory = new CHistoryWnd(app, doc, this);
    addDockWidget(Qt::LeftDockWidgetArea, m_pWndHistory);
    m_pWndHistory->hide();
    connect(m_pWndHistory, &CHistoryWnd::UpdateViewHistory, this, &CMainFrame::OnUpdateViewHistory);
 }

CMainFrame::~CMainFrame()
{
    delete ui;
}

CMainFrame* CMainFrame::Instance()
{
    return m_pInstance;
}

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

void CMainFrame::OnUpdateViewFileComments()
{
    ui->action_File_Comments->setChecked(false);
}

void CMainFrame::OnUpdateViewHistory()
{
    ui->actionBidding_Play_History->setChecked(false);
}

void CMainFrame::OnUpdateShowAnalysis()
{
    ui->actionShow_Player_Analyses->setChecked(false);
}

QMenu *CMainFrame::createPopupMenu()
{
    return 0;
}

void CMainFrame::SetStatusText(QString text)
{
    ui->statusBar->showMessage(text);
}

void CMainFrame::ResetStatusText()
{
    ui->statusBar->clearMessage();
}

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

void CMainFrame::on_actionProperties_triggered()
{
    CFilePropertiesDialog properties(app, doc);
    properties.exec();
}

void CMainFrame::on_actionRecent_File_triggered()
{

}

void CMainFrame::on_actionExit_triggered()
{

}

void CMainFrame::on_action_Undo_Trick_triggered()
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

void CMainFrame::on_action_File_Comments_triggered()
{
    if (m_pFileCommentsDlg->isVisible())
        m_pFileCommentsDlg->hide();
    else
        m_pFileCommentsDlg->show();
}

void CMainFrame::on_actionBidding_Play_History_triggered()
{
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

void CMainFrame::on_actionNew_Session_triggered()
{
    tableManager->newSession();
}

void CMainFrame::on_action_Deal_New_Hand_triggered()
{
    tableManager->newDeal();
}

void CMainFrame::on_action_Play_Rubber_triggered()
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

void CMainFrame::on_actionSeat_Configuration_triggered()
{
    CSeatConfiguration seatConfiguration(app, doc, this);
    if (seatConfiguration.exec() == QDialog::Accepted)
    {
        doc->WriteSeatOptions();

        delete tableManager;
        if (ROLES[doc->getSeatOptions().role] == SERVER_ROLE)
            tableManager = new CTblMngrServer(doc, playView, this);
        else
            tableManager = new CTblMngrClient(doc, playView, this);
    }

}

void CMainFrame::on_action_Bidding_Options_triggered()
{
    CBidOptions bidOptsDialog(app, doc, this);
    if (bidOptsDialog.exec() == QDialog::Accepted)
        doc->WriteSettings();
}

void CMainFrame::on_action_Deal_Options_triggered()
{
    CDealOptionsPropSheet dealOptsDialog(app, doc, this);
    if (dealOptsDialog.exec() == QDialog::Accepted)
        doc->WriteDealOptions();
}

void CMainFrame::on_action_Di_splay_Options_triggered()
{
    CDispOptionsPropSheet dispOptsDialog(app, doc, this);
    if (dispOptsDialog.exec() == QDialog::Accepted)
        doc->WriteDisplayOptions();
}

void CMainFrame::on_action_Game_Options_triggered()
{
    CGameOptionsPropSheet gameOptsDialog(app, doc, this);
    if (gameOptsDialog.exec() == QDialog::Accepted)
        doc->WriteGameOptions();
}

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
