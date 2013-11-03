/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgramConfigWizard.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard (declaration).
 */

#ifndef CPROGRAMCONFIGWIZARD_H
#define CPROGRAMCONFIGWIZARD_H

#include <QWizard>

#include "cwizardoptiondoc.h"

class CZBridgeApp;
class CZBridgeDoc;

namespace Ui {
class CProgramConfigWizard;
}

class CProgramConfigWizard : public QWizard
{
    Q_OBJECT
    
public:
    explicit CProgramConfigWizard(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent = 0);
    ~CProgramConfigWizard();

    void accept();

private:
    Ui::CProgramConfigWizard *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
    CWizardOptionDoc wizardOptionDoc;
};

#endif // CPROGRAMCONFIGWIZARD_H
