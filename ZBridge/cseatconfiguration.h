/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CSeatConfiguration.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Configuration of seats.

  Platforms: Qt.

*/

/**
 * \file
 * Configuration of seats (declaration).
 */

#ifndef CSEATCONFIGURATION_H
#define CSEATCONFIGURATION_H

#include <QDialog>

#include "cseatoptiondoc.h"

class CZBridgeApp;
class CZBridgeDoc;

namespace Ui {
class CSeatConfiguration;
}
/**
 * @brief Seat option dialog.
 *
 * Shows the seat option dialog and allows the user to set/change the options.
 */
class CSeatConfiguration : public QDialog
{
    Q_OBJECT
    
public:
    explicit CSeatConfiguration(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent = 0);
    ~CSeatConfiguration();
    
private slots:

    void on_west_clicked();

    void on_north_clicked();

    void on_east_clicked();

    void on_south_clicked();

    void on_westActor_currentIndexChanged(int index);

    void on_northActor_currentIndexChanged(int index);

    void on_eastActor_currentIndexChanged(int index);

    void on_southActor_currentIndexChanged(int index);

    void on_role_currentIndexChanged(int index);

    void on_protocol_currentIndexChanged(int index);

    void on_buttonBox_accepted();

private:
    void updateSeatAndActor();
    void updateHostAndPort();

    Ui::CSeatConfiguration *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
    CSeatOptionDoc seatOptionDoc;
};

#endif // CSEATCONFIGURATION_H
