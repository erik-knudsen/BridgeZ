/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigSUitsDisplayPage.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard suits display page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard suits display page (declaration).
 */

#ifndef CPROGCONFIGSUITSDISPLAYPAGE_H
#define CPROGCONFIGSUITSDISPLAYPAGE_H

#include <QWizardPage>

namespace Ui {
class CProgConfigSuitsDisplayPage;
}

/**
 * @brief Wizard suits display page.
 */
class CProgConfigSuitsDisplayPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit CProgConfigSuitsDisplayPage(QWidget *parent = 0);
    ~CProgConfigSuitsDisplayPage();

    Ui::CProgConfigSuitsDisplayPage *ui;

private:
};

#endif // CPROGCONFIGSUITSDISPLAYPAGE_H
