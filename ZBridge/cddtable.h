/*Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CDDTable.h
  Developers: eak

  Revision History:
  14-jan-2016 eak: Original

  Abstract: Calculate and show double dummy results and par.

  Platforms: Qt.

*/

/**
 * \file
 * Calculate and show double dummy results and par(declaration).
 */

#ifndef CDDTABLE_H
#define CDDTABLE_H

#include <QDialog>

#include "Defines.h"

namespace Ui {
class CDDTable;
}

class CDDTable : public QDialog
{
    Q_OBJECT

public:
    explicit CDDTable(int cards[][13], Team vulnerable, QWidget *parent = 0);
    ~CDDTable();

private:
    Ui::CDDTable *ui;
};

#endif // CDDTABLE_H
