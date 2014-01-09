/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigGameMechanicsPage.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard game mechanics page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard game mechanics page (declaration).
 */

#ifndef CPROGCONFIGGAMEMECHANICSPAGE_H
#define CPROGCONFIGGAMEMECHANICSPAGE_H

#include <QWizardPage>

namespace Ui {
class CProgConfigGameMechanicsPage;
}

/**
 * @brief Wizard game mechanics page.
 */
class CProgConfigGameMechanicsPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit CProgConfigGameMechanicsPage(QWidget *parent = 0);
    ~CProgConfigGameMechanicsPage();

    Ui::CProgConfigGameMechanicsPage *ui;

private:
};

#endif // CPROGCONFIGGAMEMECHANICSPAGE_H
