/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigIntroPage.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard intro page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard intro page (declaration).
 */

#ifndef CPROGCONFIGINTROPAGE_H
#define CPROGCONFIGINTROPAGE_H

#include <QWizardPage>

namespace Ui {
class CProgConfigIntroPage;
}

/**
 * @brief Wizard intro page.
 */
class CProgConfigIntroPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit CProgConfigIntroPage(QWidget *parent = 0);
    ~CProgConfigIntroPage();
    
private:
    Ui::CProgConfigIntroPage *ui;
};

#endif // CPROGCONFIGINTROPAGE_H
