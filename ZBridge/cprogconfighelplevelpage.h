/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigHelpLevelPage.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard help level page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard help level page (declaration).
 */

#ifndef CPROGCONFIGHELPLEVELPAGE_H
#define CPROGCONFIGHELPLEVELPAGE_H

#include <QWizardPage>

namespace Ui {
class CProgConfigHelpLevelPage;
}

/**
 * @brief Wizard help level page.
 */
class CProgConfigHelpLevelPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit CProgConfigHelpLevelPage(QWidget *parent = 0);
    ~CProgConfigHelpLevelPage();

    Ui::CProgConfigHelpLevelPage *ui;

private slots:

private:
};

#endif // CPROGCONFIGHELPLEVELPAGE_H
