/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CZBridgeApp.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: ZBridge application.

  Platforms: Qt.

*/

/**
 * \file
 * ZBridge application (declaration).
 */

#ifndef CZBRIDGEAPP_H
#define CZBRIDGEAPP_H

#include <QApplication>

#include "cmainframe.h"
class CZBridgeDoc;

class CZBridgeApp : public QApplication
{
    Q_OBJECT
public:
    static CZBridgeApp* Instance();
    explicit CZBridgeApp(int &argc, char **argv);

private:
    CMainFrame * mainFrame;
    CZBridgeDoc * doc;

    static CZBridgeApp* m_pInstance;

signals:

public slots:

};

#endif // CZBRIDGEAPP_H
