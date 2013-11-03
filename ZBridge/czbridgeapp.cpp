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
 * \file
 * ZBridge application (definition).
 */

#include "czbridgeapp.h"
#include "cmainframe.h"
#include "czbridgedoc.h"

CZBridgeApp *CZBridgeApp::m_pInstance = 0;

int main(int argc, char *argv[])
{
    CZBridgeApp app(argc, argv);

    return app.exec();
}

CZBridgeApp::CZBridgeApp(int &argc, char **argv) :
    QApplication(argc, argv)
{
    m_pInstance = this;

    doc = new CZBridgeDoc(this);

    mainFrame = new CMainFrame(this, doc);
    mainFrame->show();
}

CZBridgeApp* CZBridgeApp::Instance()
{
    return m_pInstance;
}
