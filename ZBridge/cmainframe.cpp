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
#include <QSettings>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QHostInfo>
#include <QStringList>
#include <QString>
#include <QDataStream>
#include <QTextStream>
#include <QInputDialog>
#include <QMessageBox>
#include <QThread>
#include <QHelpContentWidget>
#include <QHelpIndexWidget>
#include <QHelpSearchQueryWidget>
#include <QTableWidget>

#include "ZBridgeException.h"
#include "cmainframe.h"
#include "ui_cmainframe.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"
#include "cgamesdoc.h"
#include "cplayview.h"
#include "caboutdlg.h"
#include "cbidoptions.h"
#include "cdealoptionspropsheet.h"
#include "cdispoptionspropsheet.h"
#include "cgameoptionspropsheet.h"
#include "cbiddialog.h"
#include "cseatconfiguration.h"
#include "ctblmngrserver.h"
#include "ctblmngrserverauto.h"
#include "ctblmngrclient.h"
#include "ctblmngrclientauto.h"
#include "cmainscoredialog.h"
#include "crubberscoredialog.h"
#include "clayoutcardsdialog.h"
#include "cbiddb.h"
#include "cbiddesc.h"
#include "cbiddingsystemdialog.h"
#include "cbidandplayengines.h"
#include "chelpbrowser.h"
#include "cassistant.h"

const int MAX_RECENT_FILES = 4;

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
 *
 * @param app Pointer to application.
 * @param doc Pointer to data container.
 * @param games Pointer to game data.
 */
CMainFrame::CMainFrame(CZBridgeApp *app, CZBridgeDoc *doc, CGamesDoc *games) :
    ui(new Ui::CMainFrame)
{
    this->app = app;
    this->doc = doc;
    this->games = games;

    tableManager = 0;
    tableManagerAuto = 0;

    //Set up user interface (main menu etc.)
    ui->setupUi(this);

    //Recent files actions and connections.
    QAction *recentFileAction = 0;
    for (int i = 0; i < MAX_RECENT_FILES; i++)
    {
        recentFileAction = new QAction(this);
        recentFileAction->setVisible(true);
        QObject::connect(recentFileAction, &QAction::triggered, this, CMainFrame::openRecent);
        recentFileActionList.append(recentFileAction);
    }

    //Recent files menu.
    for (int i = 0; i < MAX_RECENT_FILES; i++)
        ui->menuRecent_Files->addAction(recentFileActionList.at(i));

    updateRecentActionList();

    //Allocate and initialize play view.
    playView = new CPlayView(this);

    //Main score dialog.
    mainScoreDialog = new CMainScoreDialog(games, this);

    //Bid and play engines.
    bidAndPlayEngines = new CBidAndPlayEngines();

    //Reset play.
    resetPlay();

    //Initialization of main frame window.
    setCentralWidget(playView);
    setWindowIcon(QIcon(":/resources/ZBridge.ico"));

    resize(QSize(SCENE_HOR_SIZE + 50, SCENE_VER_SIZE + 175));
    setMinimumSize(QSize((SCENE_HOR_SIZE + 50) * .75, (SCENE_VER_SIZE + 175) * .75));
    setMaximumSize(QSize((SCENE_HOR_SIZE + 50) * 1.25, (SCENE_VER_SIZE + 175) * 1.25));

    //Help.
    createHelpWindow();

    assistant = new CAssistant;
}

CMainFrame::~CMainFrame()
{
    delete ui;
    delete bidAndPlayEngines;
    delete assistant;
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
 * @brief Update action list for recent open files.
 */
void CMainFrame::updateRecentActionList()
{
    QSettings settings("ZBridge settings", "recent");

    QStringList recentFilePaths =
            settings.value("recentFiles").toStringList();

    auto itEnd = 0;
    if(recentFilePaths.size() <= MAX_RECENT_FILES)
        itEnd = recentFilePaths.size();
    else
        itEnd = MAX_RECENT_FILES;

    for (auto i = 0; i < itEnd; ++i)
    {
        QString strippedName = QFileInfo(recentFilePaths.at(i)).fileName();
        recentFileActionList.at(i)->setText(strippedName);
        recentFileActionList.at(i)->setData(recentFilePaths.at(i));
        recentFileActionList.at(i)->setVisible(true);
    }

    for (auto i = itEnd; i < MAX_RECENT_FILES; ++i)
        recentFileActionList.at(i)->setVisible(false);
}

/**
 * @brief Adjust recent open files for the current file.
 * @param filePath Current file.
 * @param add if true the add the file to the list, otherwise remove the file from the list.
 */
void CMainFrame::adjustForCurrentFile(const QString &filePath, bool add)
{
    QSettings settings("ZBridge settings", "recent");

    QStringList recentFilePaths =
            settings.value("recentFiles").toStringList();
    recentFilePaths.removeAll(filePath);
    if (add)
        recentFilePaths.prepend(filePath);
    while (recentFilePaths.size() > MAX_RECENT_FILES)
        recentFilePaths.removeLast();
    settings.setValue("recentFiles", recentFilePaths);

    updateRecentActionList();
}

/**
 * @brief Create Help window.
 *
 *  Help Window is created as a 2 split screen window with contents, index and search
 *  tabs on the left window. And browser with forward and backward buttons in the
 *  right windows.
 */
void CMainFrame::createHelpWindow()
{
    //Help file is located in same folder as exe file.
    QString qhcPath = QApplication::applicationDirPath() + "/zbridge.qhc";
    helpEngine = new QHelpEngine(qhcPath);
    helpEngine->setupData();

    //Tabs for contents, index and search (uses build in widgets).
    QTabWidget* tWidget = new QTabWidget;
    tWidget->setMaximumWidth(300);
    tWidget->addTab(helpEngine->contentWidget(), "Contents");
    tWidget->addTab(helpEngine->indexWidget(), "Index");
    QVBoxLayout *searchBox = new QVBoxLayout;
    searchBox->addWidget(helpEngine->searchEngine()->queryWidget());
    searchBox->addWidget((QWidget *)(helpEngine->searchEngine()->resultWidget()));
    QWidget *searchWidget = new QWidget;
    searchWidget->setLayout(searchBox);
    tWidget->addTab(searchWidget, "Search");

    //Browser with backward and forward buttons.
    CHelpBrowser *textViewer = new CHelpBrowser(helpEngine);
    textViewer->setSource(QUrl("qthelp://erik.aagaard.knudsen/doc/index.html"));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QToolButton *leftButton = new QToolButton;
    leftButton->setArrowType(Qt::LeftArrow);
    leftButton->setDisabled(true);
    QToolButton *rightButton = new QToolButton;
    rightButton->setArrowType(Qt::RightArrow);
    rightButton->setDisabled(true);
    buttonLayout->addWidget(leftButton);
    buttonLayout->addWidget(rightButton);
    buttonLayout->addStretch(1);
    QWidget *buttonWidget = new QWidget;
    buttonWidget->setLayout(buttonLayout);
    mainLayout->addWidget(buttonWidget);
    mainLayout->addWidget(textViewer);
    QWidget *browserWidget = new QWidget;
    browserWidget->setLayout(mainLayout);

    //Conect contents, index and search to browser.
    connect(helpEngine->contentWidget(),
            SIGNAL(linkActivated(QUrl)),
            textViewer, SLOT(setSource(QUrl)));
    connect(helpEngine->indexWidget(),
            SIGNAL(linkActivated(QUrl, QString)),
            textViewer, SLOT(setSource(QUrl)));
    connect((QWidget *)(helpEngine->searchEngine()->resultWidget()),
            SIGNAL(requestShowLink(QUrl)),
            textViewer, SLOT(setSource(QUrl)));

    //Connect search query to search slot.
    connect(helpEngine->searchEngine()->queryWidget(),
            SIGNAL(search()),
            this, SLOT(search()));

    //Connect back and forward buttons to browser.
    connect(leftButton, SIGNAL(clicked()), textViewer, SLOT(backward()));
    connect(rightButton, SIGNAL(clicked()), textViewer, SLOT(forward()));
    connect(textViewer, SIGNAL(backwardAvailable(bool)), leftButton, SLOT(setEnabled(bool)));
    connect(textViewer, SIGNAL(forwardAvailable(bool)), rightButton, SLOT(setEnabled(bool)));

/*  For debugging search for help.
    connect(helpEngine->searchEngine(),
            SIGNAL(indexingStarted()),
            this, SLOT(inxStarted()));
    connect(helpEngine->searchEngine(),
            SIGNAL(indexingFinished()),
            this, SLOT(inxFinished()));
    connect(helpEngine->searchEngine(),
            SIGNAL(searchingStarted()),
            this, SLOT(searchStarted()));
    connect(helpEngine->searchEngine(),
            SIGNAL(searchingFinished(int hits)),
            this, SLOT(searchFinished(int hits)));
*/
    //Must index documentation.
    helpEngine->searchEngine()->reindexDocumentation();

    //Set up help window.
    helpWindow = new QSplitter(Qt::Horizontal);
    helpWindow->setWindowTitle(tr("Help"));

    helpWindow->insertWidget(0, tWidget);
    helpWindow->insertWidget(1, browserWidget);
    helpWindow->resize(1000, 600);

    helpWindow->hide();
}

/*For debugging search for help.
void CMainFrame::inxStarted()
{
    ;
}

void CMainFrame::inxFinished()
{
    ;
}

void CMainFrame::searchStarted()
{
    ;
}

void CMainFrame::searchFinished(int hits)
{
    ;
}
*/

/**
 * @brief Get help search query and start search.
 */
void CMainFrame::search()
{
    QList<QHelpSearchQuery> query = helpEngine->searchEngine()->queryWidget()->query();
    helpEngine->searchEngine()->search(query);
}

/**
 * @brief Get host address.
 *
 * @param host The name of the host.
 * @return the host address.
 */
QHostAddress CMainFrame::getHostAddress(QString host)
{
    QHostAddress hostAddress;

    //Determine IP address.
    QHostInfo hostInfo = QHostInfo::fromName(host);
    if (!hostInfo.addresses().isEmpty())
    {
        QList<QHostAddress> hostAddresses = hostInfo.addresses();
        int hostInx;
        for (hostInx = 0; hostInx < hostAddresses.size(); hostInx++)
        {
            if (hostAddresses.at(hostInx).protocol() == QAbstractSocket::IPv4Protocol)
                break;
        }
        if (hostInx < hostAddresses.size())
            hostAddress = hostAddresses.at(hostInx);
    }
    return hostAddress;
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
            enableUIActionsInitial();
            break;

            //Enable/Disable for initial main menu entries for table manager server.
        case UPDATE_UI_SERVER:
            enableUIActionsServer();
            break;

            //Enable/Disable for initial main menu entries for table manager client.
        case UPDATE_UI_CLIENT:
            enableUIActionsClient();
            break;

            //Enable/Disable save menu entry.
        case UPDATE_UI_SAVE:
            ui->actionSave->setEnabled(param);
            break;

            //Enable/Disable save as menu entry.
        case UPDATE_UI_SAVEAS:
            ui->actionSave_As->setEnabled(param);
            break;

            //Enable/Disable delete menu entry.
        case UPDATE_UI_DELETE:
            ui->actionDelete->setEnabled(param);
            break;

            //Enable/Disable new session menu entry.
        case UPDATE_UI_NEW_SESSION:
            ui->actionNew_Session->setEnabled(param);
            break;

            //Enable/Disable new deal menu entry.
        case UPDATE_UI_NEW_DEAL:
            if (!games->getComputerPlays())
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

            //Enable/Disable Double Dummy Results.
        case UPDATE_UI_PAR:
            ui->actionDouble_Dummy_Results->setEnabled(param);
            break;

            //Enable/Disable score dialogs.
        case UPDATE_UI_SCORE:
            ui->action_Score->setEnabled(param);
            break;

            //Enable/Disable score dialogs.
        case UPDATE_UI_LAY_OUT_CARDS:
            ui->action_Lay_Out_Cards->setEnabled(param);
            break;

            //Enable/Disable hint.
        case UPDATE_UI_HINT:
            ui->actionHint->setEnabled(param);
            break;
        }
    }
}

/**
 * @brief Enable main menu entries initially (before server or client mode is determined).
 */
void CMainFrame::enableUIActionsInitial()
{
    enableUIActions(INITIAL_ACTIONS);
}

/**
 * @brief Enable main menu entries initially for server mode.
 */
void CMainFrame::enableUIActionsServer()
{
    enableUIActions(SERVER_ACTIONS);
}

/**
 * @brief Enable main menu entries initially for client mode.
 */
void CMainFrame::enableUIActionsClient()
{
    enableUIActions(CLIENT_ACTIONS);
}

/**
 * @brief Set or clear status text in main window.
 *
 * @param text The text to set.
 */
void CMainFrame::sStatusText(QString text)
{
    if (text.size() > 0)
        SetStatusText(text);
    else
        ResetStatusText();
}

/**
 * @brief Enable/disable main menu entries.
 *
 * @param actions identifies which actions to intially enable/disable for.
 *        - INITIAL_ACTIONS before server/client mode is determined.
 *        - SERVER_ACTIONS for server mode.
 *        - CLIENT_ACTIONS for client mode.
 */
void CMainFrame::enableUIActions(actionIndicator actions)
{
    ui->actionOpen->setEnabled((actions == INITIAL_ACTIONS) || (actions == SERVER_ACTIONS));

    ui->action_Lay_Out_Cards->setEnabled((actions == INITIAL_ACTIONS) || (actions == SERVER_ACTIONS));
    ui->actionClaim_All->setEnabled(((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS)));
    ui->actionClaim_Contract->setEnabled(((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS)));
    ui->actionConcede->setEnabled(((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS)));
    ui->actionHint->setEnabled(false);

    ui->actionSave->setEnabled(false);
    ui->actionSave_As->setEnabled(false);
    ui->actionDelete->setEnabled(false);

    ui->actionNew_Session->setEnabled(true);
    ui->action_Deal_New_Hand->setEnabled(false);

    ui->action_Bid_Rebid->setEnabled(false);
    ui->action_Restart_Hand->setEnabled(false);
    ui->actionUndo->setEnabled(false);

    ui->action_Expose_All_Cards->setEnabled(false);
    ui->actionDouble_Dummy_Results->setEnabled(false);

    ui->action_Score->setEnabled((actions == SERVER_ACTIONS) || (actions == CLIENT_ACTIONS));
}

/**
 * @brief Reset play configuration (manual play, auto play etc.)
 */
void CMainFrame::resetPlay()
{
    if (tableManager != 0)
        delete tableManager;
    tableManager = 0;
    if (tableManagerAuto != 0)
        emit sAutoQuit();
    tableManagerAuto = 0;

    hostAddress.clear();
    if ((doc->getSeatOptions().role == SERVER_ROLE) || (doc->getSeatOptions().role == CLIENT_ROLE))
    {
        QString host = (doc->getSeatOptions().role == SERVER_ROLE) ? (doc->getSeatOptions().hostServer) :
                                                                     (doc->getSeatOptions().hostClient);
        hostAddress = getHostAddress(host);
        if (hostAddress.isNull())
            QMessageBox::warning(0, tr("ZBridge"), tr("Could not determine IP address."));
    }

    //Table manager server?
    if((doc->getSeatOptions().role == SERVER_ROLE) && !hostAddress.isNull())
    {
        try
        {
            tableManager = new CTblMngrServer(doc, games, bidAndPlayEngines, hostAddress, playView, this);
            tableManagerAuto = new CTblMngrServerAuto(doc, games, bidAndPlayEngines, hostAddress, 0);
        }
        catch (NetProtocolException &e)
        {
            QMessageBox::warning(0, tr("ZBridge"), e.what());

            if (tableManager != 0)
                delete tableManager;
            hostAddress.clear();
        }
    }

    //Table manager client?
    else if((doc->getSeatOptions().role == CLIENT_ROLE) && !hostAddress.isNull())
    {
        tableManager = new CTblMngrClient(doc, games, bidAndPlayEngines, hostAddress, playView, this);
        tableManagerAuto = new CTblMngrClientAuto(doc, games, bidAndPlayEngines, hostAddress, 0);
        connect(tableManager, &CTblMngrBase::sigDisconnect, tableManagerAuto, &CTblMngrBase::sltDisconnect, Qt::QueuedConnection);
        connect(tableManagerAuto, &CTblMngrBase::sigDisconnect, tableManager, &CTblMngrBase::sltDisconnect, Qt::QueuedConnection);
    }

    //Table manager standalone?
    if (hostAddress.isNull())
    {
        tableManager = new CTblMngrServer(doc, games, bidAndPlayEngines, hostAddress, playView, this);
        tableManagerAuto = new CTblMngrServerAuto(doc, games, bidAndPlayEngines, hostAddress, 0);
    }

    QThread *thread = new QThread();
    tableManagerAuto->moveToThread(thread);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(this, &CMainFrame::sAutoQuit, tableManagerAuto, &CTblMngrBase::sAutoQuit, Qt::QueuedConnection);
    connect(this, &CMainFrame::sNewSession, tableManagerAuto, &CTblMngrBase::sNewSession, Qt::QueuedConnection);
    connect(tableManager, &CTblMngrBase::sStatusText , this, &CMainFrame::sStatusText);
    thread->start();

    connect(tableManager, &CTblMngr::sShowScore, this, &CMainFrame::on_action_Score_triggered);

    //If non saved game then ask if it should be saved (save can only be enabled on server).
    if ((ui->actionSave->isEnabled()) &&
            (QMessageBox::question(0, tr("ZBridge"),
                                   tr("Do you want to save played games?")) == QMessageBox::Yes))
        on_actionSave_triggered();

    //Hide score table in case it is shown and clear games.
    mainScoreDialog->hide();

    //Clear expose all cards.
    ui->action_Expose_All_Cards->setChecked(false);

    //No current file.
    fileName = "";
    eventIndex = 0;

    setWindowTitle("");
}

/**
 * @brief Open Portable Bridge Notation (pbn) file for play etc.
 * @param originalFileName Name of the pbn file to open.
 */
void CMainFrame::open(QString &originalFileName)
{
    try
    {
    QFile originalFile(originalFileName);
    if (!originalFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream original(&originalFile);

    QTextStream played;
    QString event;

    //Determine events.
    QStringList strLines;
    games->determineEvents(original, strLines);

    if (strLines.size() > 1)
    {
        bool ok;
        event = QInputDialog::getItem(this, tr("Select Event"), tr("Event"), strLines, 0, false, &ok);
        if (!ok)
            return;
    }
    else if (strLines.size() == 1)
        event = strLines.at(0);

    //Determine event index.
    int inx;
    for (inx = 0; inx < strLines.size(); inx++)
        if (strLines.at(inx) == event)
            break;

    //Open file with already (in this program) played games.
    QString playedFilename = originalFileName.left(originalFileName.indexOf(".pbn", 0, Qt::CaseInsensitive)) +
            "_" + QString::number(inx) + ".zbr";
    QFile playedFile(playedFilename);

    //None might be played.
    if (playedFile.open(QIODevice::ReadOnly | QIODevice::Text))
        played.setDevice(&playedFile);

    emit sNewSession();     //Start new session for table manager auto.

    //Read games.
    original.seek(0);
    games->readGames(original, played, event, doc->getGameOptions().scoringMethod);

    originalFile.close();

    //Save info about pbn file.
    eventIndex = inx;
    fileName = originalFileName;

    //Initialize bid and play engines.
    bidAndPlayEngines->initialize(doc->getGameOptions().computerLevel, doc->getBidDB(), doc->getBidDesc(),
                                  doc->getNSBidOptions(), doc->getEWBidOptions(),
                                  games->getScoringMethod());

    //Set up synchronization (auto play or not).
    //Open is not available on client, i.e table manager is either server or standalone.
    disconnect(tableManager, &CTblMngrBase::sigPlayStart, 0, 0);
    disconnect(tableManagerAuto, &CTblMngrBase::sigPlayStart, 0, 0);
    if (!games->getComputerPlays())
        connect(tableManager, &CTblMngrBase::sigPlayStart, tableManager, &CTblMngrBase::sltPlayStart);
    else
    {
        connect(tableManager, &CTblMngrBase::sigPlayStart, tableManagerAuto, &CTblMngrBase::sltPlayStart, Qt::QueuedConnection);
        connect(tableManagerAuto, &CTblMngrBase::sigPlayStart, tableManager, &CTblMngrBase::sltPlayStart, Qt::QueuedConnection);
    }

    //Start new session for table manager.
    tableManager->newSession();

    if (played.device() != 0)
    {
        playedFile.close();
        QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_DELETE , true));
    }

    //Update recent open files list.
    adjustForCurrentFile(originalFileName, true);

    setWindowTitle(originalFileName);

    //Can now be saved with another name.
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_SAVEAS , true));
    }
    catch (PlayException &e)
    {
        //There was an error in processing of pbn file.
        QMessageBox::critical(0, tr("ZBridge"), e.what());
    }
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
/**
 * @brief Open pbn file. Use the games in the file(s) for playing.
 *
 * The original games are read from the pbn file. Played games are read from a companion to the original
 * pbn file with a filename derived from the original pbn filename and the event index. With the extension zbr.
 */
void CMainFrame::on_actionOpen_triggered()
{
    //If non saved game then ask if it should be saved (save can only be enabled on server).
    if ((ui->actionSave->isEnabled()) &&
        (QMessageBox::question(0, tr("ZBridge"),
               tr("Do you want to save played games?")) == QMessageBox::Yes))
        on_actionSave_triggered();

    //Determine pbn file to read games from.
    QString originalFileName = QFileDialog::getOpenFileName(this,
        tr("Open Portable Bridge Notation file"), "", tr("Portable Bridge Notation (*.pbn)"));
    if (originalFileName.size() == 0)
        return;

    //Open file and prepare for play.
    open(originalFileName);
}

/**
 * @brief Save the current games in pbn file format.
 *
 * Played games are saved as companion to the original pbn file with a filename derived from the original
 * pbn filename and the event index. With the extension zbr.
 */
void CMainFrame::on_actionSave_triggered()
{
    //Save (in this program) played games.
    //Might be (by this program) random generated games.
    if (fileName.size() == 0)
        on_actionSave_As_triggered();
    else
    {
        //Save (by this program) played games.
        QString playedFilename = fileName.left(fileName.indexOf(".pbn", 0, Qt::CaseInsensitive)) +
                "_" + QString::number(eventIndex) + ".zbr";
        QFile playedFile(playedFilename);
        playedFile.open(QIODevice::ReadWrite | QIODevice::Text);
        playedFile.resize(0);
        QTextStream played(&playedFile);

        games->writePlayedGames(played);

        playedFile.close();

        QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_SAVE , false));
        QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_DELETE , true));
    }
}

/**
 * @brief Save the current games in pbn file format, but with a new name.
 *
 * The games from the original pbn file with the chosen event are saved in a new pbn file. In case of
 * random generated games (no original pbn file) then the file will be empty. But it will still be existing.
 * This to allow it to be chosen in the open action. Played games are saved as companion to the new pbn file
 * with a filename derived from the new pbn filename and the event index zero (0). With the extension zbr.
 */
void CMainFrame::on_actionSave_As_triggered()
{
    //Save games with a new filename.
    QString originalFileName;

    //Determine pbn file to write to.
    originalFileName = QFileDialog::getSaveFileName(this,
        tr("Save Portable Bridge Notation file"), "", tr("Portable Bridge Notation (*.pbn)"));
    if (originalFileName.size() == 0)
        return;

    if (originalFileName.indexOf(".pbn", Qt::CaseInsensitive) == -1)
        originalFileName += ".pbn";

    QFile originalFile(originalFileName);
    originalFile.open(QIODevice::ReadWrite | QIODevice::Text);
    originalFile.resize(0);
    QTextStream original(&originalFile);

    //Save current file name and event index.
    fileName = originalFileName;
    eventIndex = 0;

    //Determine file with (by this program) played games.
    QString playedFilename = fileName.left(fileName.indexOf(".pbn", 0, Qt::CaseInsensitive)) +
            "_" + QString::number(eventIndex) + ".zbr";
    QFile playedFile(playedFilename);
    playedFile.open(QIODevice::ReadWrite | QIODevice::Text);
    playedFile.resize(0);
    QTextStream played(&playedFile);

    //Save original games (in case of random generated games there are no original games).
    games->writeOriginalGames(original);

    //Save (by this program) played games.
    games->writePlayedGames((played));

    originalFile.close();
    playedFile.close();

    //Update recent open files list.
    adjustForCurrentFile(originalFileName, true);
    setWindowTitle(originalFileName);

    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_SAVE , false));
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_DELETE , true));
}

/**
 * @brief Delete the current games (only played part - not original).
 */
void CMainFrame::on_actionDelete_triggered()
{
    if (QMessageBox::question(0, tr("ZBridge"),
           tr("Do you want to delete played games?")) == QMessageBox::Yes)
    {
        //Delete (by this program) played games.
        QString playedFilename = fileName.left(fileName.indexOf(".pbn", 0, Qt::CaseInsensitive)) +
                "_" + QString::number(eventIndex) + ".zbr";
        QFile playedFile(playedFilename);
        playedFile.remove();

        QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_DELETE , false));
    }
}

void CMainFrame::on_actionPrint_triggered()
{

}

/**
 * @brief Open recent files.
 */
void CMainFrame::openRecent()
{
    //Get action pointer of signal.
    QAction *action = qobject_cast<QAction *>(sender());

    if (action)
    {
        //If non saved game then ask if it should be saved (save can only be enabled on server).
        if ((ui->actionSave->isEnabled()) &&
            (QMessageBox::question(0, tr("ZBridge"),
                   tr("Do you want to save played games?")) == QMessageBox::Yes))
            on_actionSave_triggered();

        //Open file and prepare for play.
        QString fileName = action->data().toString();
        open(fileName);
    }
}

/**
 * @brief Exit from the program.
 */
void CMainFrame::on_actionExit_triggered()
{
    exit(0);
}

/**
 * @brief Layout cards. User defines bridge hands to play.
 */
void CMainFrame::on_action_Lay_Out_Cards_triggered()
{
    //Check if there are any original or random non saved games played in the current game deal.
    if ((games->anyOriginalPlayed() || (games->getDealType() == RANDOM_DEAL)))
    {
        if (ui->actionSave->isEnabled())
        {
            if (QMessageBox::question(0, tr("ZBridge"),
                   tr("Current game set will be cleared if you continue.\n You will be asked if you want to save played games.\n Do you want to continue?")) == QMessageBox::No)
                return;

            //Do you want to save played games?
            if (QMessageBox::question(0, tr("ZBridge"),
                   tr("Do you want to save played games?")) == QMessageBox::Yes)
                on_actionSave_triggered();
        }
        games->clearGames(doc->getGameOptions().scoringMethod);
    }

    //Reset play.
    resetPlay();

    //Show Lay Out Cards dialog.
    CLayoutCardsDialog layOutCards(games);
    if ((layOutCards.exec() == QDialog::Accepted))
        on_actionSave_As_triggered();

    if (fileName.size() != 0)
        open(fileName);
    else
        games->clearGames(doc->getGameOptions().scoringMethod);    
}

/**
 * @brief Show all four hands.
 */
void CMainFrame::on_action_Expose_All_Cards_triggered()
{
    tableManager->showAllCards();
}

/**
* @brief Show score dialog.
*/
void CMainFrame::on_action_Score_triggered()
{
    mainScoreDialog->show();
}

/**
 * @brief Show double dummy results.
 */
void CMainFrame::on_actionDouble_Dummy_Results_triggered()
{
    tableManager->showDoubleDummyResults();
}

void CMainFrame::on_action_Toolbar_triggered()
{

}

void CMainFrame::on_actionStatus_Bar_triggered()
{

}

/**
 * @brief Main menu new (random) session triggered.
 */
void CMainFrame::on_actionNew_Session_triggered()
{
    emit sNewSession();     //Start new session for table manager auto.

    //If non saved game then ask if it should be saved (save can only be enabled on server).
    if ((ui->actionSave->isEnabled()) &&
        (QMessageBox::question(0, tr("ZBridge"),
               tr("Do you want to save played games?")) == QMessageBox::Yes))
        on_actionSave_triggered();

    games->clearGames(doc->getGameOptions().scoringMethod);
    if (ui->actionActivate_Deal_Profile->isChecked())
        games->setDealOptions(doc->getDealOptions());
    else
        games->clearDealOptions();
    fileName.clear();
    eventIndex = 0;

    setWindowTitle("");

    //Initialize bid and play engines.
    bidAndPlayEngines->initialize(doc->getGameOptions().computerLevel, doc->getBidDB(), doc->getBidDesc(),
                                  doc->getNSBidOptions(), doc->getEWBidOptions(),
                                  doc->getGameOptions().scoringMethod);

    //Set up synchronization (auto play or not).
    //Implicit standalone or server session.
    disconnect(tableManager, &CTblMngrBase::sigPlayStart, 0, 0);
    disconnect(tableManagerAuto, &CTblMngrBase::sigPlayStart, 0, 0);
    if (!games->getComputerPlays())
        connect(tableManager, &CTblMngrBase::sigPlayStart, tableManager, &CTblMngrBase::sltPlayStart);
    else
    {
        connect(tableManager, &CTblMngrBase::sigPlayStart, tableManagerAuto, &CTblMngrBase::sltPlayStart);
        connect(tableManagerAuto, &CTblMngrBase::sigPlayStart, tableManager, &CTblMngrBase::sltPlayStart);
    }

    //Start new session (client or server).
    tableManager->newSession();
}

/**
 * @brief Main menu new deal triggered.
 */
void CMainFrame::on_action_Deal_New_Hand_triggered()
{
    tableManager->newDeal();
}

/**
 * @brief Activate/deactivate deal profile.
 */
void CMainFrame::on_actionActivate_Deal_Profile_triggered()
{
    if (ui->actionActivate_Deal_Profile->isChecked())
        games->setDealOptions(doc->getDealOptions());
    else
        games->clearDealOptions();
}

/**
 * @brief Undo bid or play.
 */
void CMainFrame::on_actionUndo_triggered()
{
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_UNDO , false));
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_REBID , false));
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_REPLAY , false));
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_HINT , false));
    tableManager->undo();
}

/**
 * @brief Start bidding from beginning.
 */
void CMainFrame::on_action_Bid_Rebid_triggered()
{
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_UNDO , false));
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_REBID , false));
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_REPLAY , false));
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_HINT , false));
    tableManager->reBid();
}

/**
 * @brief Start playing from beginning.
 */
void CMainFrame::on_action_Restart_Hand_triggered()
{
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_UNDO , false));
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_REBID , false));
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_REPLAY , false));
    QApplication::postEvent(this, new UPDATE_UI_ACTION_Event(UPDATE_UI_HINT , false));
    tableManager->rePlay();
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
    tableManager->hint();
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

        //Reset play.
        resetPlay();
        games->clearGames(doc->getGameOptions().scoringMethod);
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
 * @brief Select bidding system database.
 */
void CMainFrame::on_actionSelect_Bid_Database_triggered()
{
    //Determine Bidding System file to read.
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Read Bidding System"), doc->getBidDBFileName(), tr("Bidding System Files (*.bsf)"));
    if (fileName.size() != 0)
    {
        doc->setBidDBFileName(fileName);
        doc->WriteBidDBFileName();
        doc->LoadBidDB();
    }
}

/**
 * @brief Edit/create bidding system database.
 */
void CMainFrame::on_actionEdit_Bid_Database_triggered()
{
        CBiddingSystemDialog biddingSystemDialog(doc->getBidDB(), doc->getBidDesc(), this);
        if (biddingSystemDialog.exec() == QDialog::Accepted)
        {
            QString fileName;
            if ((fileName = doc->getBidDBFileName()).size() != 0)
            {
                int ret = QMessageBox::warning(this, tr("ZBridge"),
                                               tr("Do you want to overwrite the existing Bidding System file?"),
                                               QMessageBox::Yes | QMessageBox::No,
                                               QMessageBox::No);
                if (ret == QMessageBox::No)
                    fileName = "";
            }

            if (fileName.size()== 0)
            {
                while ((fileName = QFileDialog::getSaveFileName(this,
                          tr("Save Bidding System"), "", tr("Bidding System File (*.bsf)"))).size() == 0)
                    ;
                doc->setBidDBFileName(fileName);
                doc->WriteBidDBFileName();
            }
            doc->SaveBidDB();
        }
        else
            doc->LoadBidDB();
}

void CMainFrame::on_action_Contents_triggered()
{
    helpWindow->show();
}

void CMainFrame::on_actionAssistant_triggered()
{
    assistant->showDocumentation("index.html");
}

/**
 * @brief Show About dialog.
 */
void CMainFrame::on_action_About_ZBridge_triggered()
{
    CAboutDlg about(app, doc, this);
    about.exec();
}
