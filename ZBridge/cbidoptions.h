/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidOptions.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Select current bid option set (conventions etc.).

  Platforms: Qt.

*/

/**
 * \file
 * Select current bid option set (conventions etc.) (declaration).
 */

#ifndef CBIDOPTIONS_H
#define CBIDOPTIONS_H

#include <QDialog>

class CZBridgeApp;
class CZBridgeDoc;
class CBidOptionDoc;

namespace Ui {
class CBidOptions;
}

/**
 * @brief The class handles bid option sets.
 *
 *The class shows the Bid Options user interface.
 *   - Create a bid option set.
 *   - Delete a bid option set.
 *   - Select bid options for North/South and East/West.
 */
class CBidOptions : public QDialog
{
    Q_OBJECT
    
public:
    explicit CBidOptions(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent);
    ~CBidOptions();

private slots:
    void on_northSouth_clicked();
    void on_eastWest_clicked();
    void on_northSouthConvention_currentIndexChanged(int index);
    void on_eastWestConvention_currentIndexChanged(int index);
    void on_newConvention_clicked();
    void on_editConvention_clicked();
    void on_DeleteConvention_clicked();
    void on_buttonBox_accepted();

private:
    Ui::CBidOptions *ui;
    CZBridgeApp *app;
    CZBridgeDoc *doc;
    int nsIndex, ewIndex;
    QString *curBidOption;
    QList<CBidOptionDoc> *bidOptions;
};

#endif // CBIDOPTIONS_H
