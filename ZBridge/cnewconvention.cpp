/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CNewConvention.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Select name for new convention.

  Platforms: Qt.

*/

/**
 * \file
 * Select name for new convention (definition).
 */

#include <QString>

#include "cnewconvention.h"
#include "ui_cnewconvention.h"

CNewConvention::CNewConvention(QString &refConventionName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CNewConvention)
{
    ui->setupUi(this);

    ui->refConvention->setText(refConventionName);
}

CNewConvention::~CNewConvention()
{
    delete ui;
}

/**
 * @brief Get the name of the new bid option set.
 */
QString CNewConvention::getNewConventionName()
{
    return ui->newConvention->text();
}
