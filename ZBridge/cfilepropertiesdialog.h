/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CFileProperties.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Play file properties.

  Platforms: Qt.

*/

/**
 * \file
 * Play file properties (declaration).
 */

#ifndef CFILEPROPERTIESDIALOG_H
#define CFILEPROPERTIESDIALOG_H

#include <QDialog>

class CZBridgeApp;
class CZBridgeDoc;

namespace Ui {
class CFilePropertiesDialog;
}

/**
 * @brief The CFilePropertiesDialog class
 */
class CFilePropertiesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CFilePropertiesDialog(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent = 0);
    ~CFilePropertiesDialog();
    
private slots:

private:
    Ui::CFilePropertiesDialog *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
};

#endif // CFILEPROPERTIESDIALOG_H
