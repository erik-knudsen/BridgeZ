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
 * @file
 * ZBridge application.
 */

#ifndef CZBRIDGEAPP_H
#define CZBRIDGEAPP_H

#include <QApplication>

class CMainFrame;
class CZBridgeDoc;
class CGamesDoc;


/**
 * @brief ZBridge application.
 *
 * This is the ZBridge application class.
 * It owns the main frame window (mainframe) and data (doc) for the application.
 *
 */
class CZBridgeApp : public QApplication
{
    Q_OBJECT
public:
    explicit CZBridgeApp(int &argc, char **argv);

private:
    CMainFrame * mainFrame;
    CZBridgeDoc * doc;
    CGamesDoc * games;

signals:

public slots:

};

#endif // CZBRIDGEAPP_H
