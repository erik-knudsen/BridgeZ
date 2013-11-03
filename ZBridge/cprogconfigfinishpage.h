/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigFinishPage.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard finish page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard finish page (declaration).
 */

#ifndef CPROGCONFIGFINISHPAGE_H
#define CPROGCONFIGFINISHPAGE_H

#include <QWizardPage>

namespace Ui {
class CProgConfigFinishPage;
}

class CProgConfigFinishPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit CProgConfigFinishPage(QWidget *parent = 0);
    ~CProgConfigFinishPage();
    
private:
    Ui::CProgConfigFinishPage *ui;
};

#endif // CPROGCONFIGFINISHPAGE_H
