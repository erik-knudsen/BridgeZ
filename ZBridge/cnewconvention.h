/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CNewConvention.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Select name for new convention.

  Platforms: Qt.

*/

/**
 * \file
 * Select name for new convention (declaration).
 */

#ifndef CNEWCONVENTION_H
#define CNEWCONVENTION_H

#include <QDialog>

class QString;

namespace Ui {
class CNewConvention;
}

/**
 * @brief Dialog for user to select the name of a new bid option set.
 */
class CNewConvention : public QDialog
{
    Q_OBJECT
    
public:
    explicit CNewConvention(QString &refConventionName,  QWidget *parent = 0);
    ~CNewConvention();

    QString getNewConventionName();
    
private slots:

private:
    Ui::CNewConvention *ui;
};

#endif // CNEWCONVENTION_H
