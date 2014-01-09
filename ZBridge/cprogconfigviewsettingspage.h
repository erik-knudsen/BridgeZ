/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigViewSettingsPage.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard view settings page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard view settings page (declaration).
 */

#ifndef CPROGCONFIGVIEWSETTINGSPAGE_H
#define CPROGCONFIGVIEWSETTINGSPAGE_H

#include <QWizardPage>

namespace Ui {
class CProgConfigViewSettingsPage;
}

/**
 * @brief Wizard view settings page.
 */
class CProgConfigViewSettingsPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit CProgConfigViewSettingsPage(QWidget *parent = 0);
    ~CProgConfigViewSettingsPage();

    Ui::CProgConfigViewSettingsPage *ui;

private:
};

#endif // CPROGCONFIGVIEWSETTINGSPAGE_H
