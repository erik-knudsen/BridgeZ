/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CProgConfigBidding Page.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard bidding page.

  Platforms: Qt.

*/

/**
 * \file
 * Wizard bidding page (definition).
 */

#ifndef CPROGCONFIGBIDDINGPAGE_H
#define CPROGCONFIGBIDDINGPAGE_H

#include <QWizardPage>

namespace Ui {
class CProgConfigBiddingPage;
}

/**
 * @brief Wizard bidding page.
 */
class CProgConfigBiddingPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit CProgConfigBiddingPage(QWidget *parent = 0);
    ~CProgConfigBiddingPage();

    Ui::CProgConfigBiddingPage *ui;

private:
};

#endif // CPROGCONFIGBIDDINGPAGE_H
