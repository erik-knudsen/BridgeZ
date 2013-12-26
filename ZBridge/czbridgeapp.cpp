/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBridgeApp.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: ZBridge application.

  Platforms: Qt.

*/

/**
 * @file
 * ZBridge application.
 */

#include <QAbstractSocket>

#include "czbridgeapp.h"
#include "cmainframe.h"
#include "czbridgedoc.h"


/**
 * Main for starting the ZBridge application.
 * The application is allocated. Relevant meta types are registered and the
 * application is started.
 */
int main(int argc, char *argv[])
{
    CZBridgeApp app(argc, argv);

    qRegisterMetaType<QAbstractSocket::SocketError>();

    return app.exec();
}

/**
 * The constructor initializes the Zbridge application.
 *   - Allocate data for the application (model).
 *   - Allocate the main frame window etc. (view).
 *   - Show the main frame window.
 */
CZBridgeApp::CZBridgeApp(int &argc, char **argv) :
    QApplication(argc, argv)
{
    doc = new CZBridgeDoc(this);

    mainFrame = new CMainFrame(this, doc);
    mainFrame->show();
}
