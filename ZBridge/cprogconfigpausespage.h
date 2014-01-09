/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigPausesPage.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard pauses page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard pauses page (declaration).
 */

#ifndef CPROGCONFIGPAUSESPAGE_H
#define CPROGCONFIGPAUSESPAGE_H

#include <QWizardPage>

namespace Ui {
class CProgConfigPausesPage;
}

/**
 * @brief Wizard pauses page.
 */
class CProgConfigPausesPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit CProgConfigPausesPage(QWidget *parent = 0);
    ~CProgConfigPausesPage();

    Ui::CProgConfigPausesPage *ui;

private slots:
    void on_biddingPause_sliderMoved(int position);
    void on_playPause_sliderMoved(int position);

private:
};

#endif // CPROGCONFIGPAUSESPAGE_H
