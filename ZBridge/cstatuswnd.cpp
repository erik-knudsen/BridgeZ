/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CStatusWnd.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Status window.

  Platforms: Qt.
*/

/**
 * \file
 * Status window (definition).
 */

#include "cstatuswnd.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"
#include "ui_cstatuswnd.h"

CStatusWnd::CStatusWnd(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::CStatusWnd)
{
    ui->setupUi(this);
    this->app = app;
    this->doc = doc;

    QStringList labels;

    labels << "#" << "Type" << "Description" << "Winner?" << "Uses" << "Tgt Cd"
                 << "Key Cds" << "Alt Key Cds" << "Alt K/C 2" << "Opp. Key Cds"
                 << "Opp. Alt Key Cds" << "Req. Played Cds";
    ui->playPlan->setHorizontalHeaderLabels(labels);
    ui->playPlan->setColumnWidth(0,20);
    ui->playPlan->setColumnWidth(1,60);
    ui->playPlan->setColumnWidth(2,150);
    ui->playPlan->setColumnWidth(3,60);
    ui->playPlan->setColumnWidth(4,50);
    ui->playPlan->setColumnWidth(5,50);
    ui->playPlan->setColumnWidth(6,60);
    ui->playPlan->setColumnWidth(7,80);
    ui->playPlan->setColumnWidth(8,60);
    ui->playPlan->setColumnWidth(9,80);
    ui->playPlan->setColumnWidth(10,90);
    ui->playPlan->setColumnWidth(11,90);

    labels.clear();
    labels << " " << "Cards Played" << "Cards left" << "Min left" << "Max left" << "Starting Cards";
    ui->cardLocations->setHorizontalHeaderLabels(labels);
    ui->cardLocations->setColumnWidth(0,20);
    ui->cardLocations->setColumnWidth(1,150);
    ui->cardLocations->setColumnWidth(2,150);
    ui->cardLocations->setColumnWidth(3,150);
    ui->cardLocations->setColumnWidth(4,150);
    ui->cardLocations->setColumnWidth(5,150);

    labels.clear();
    labels << " " << "Decl Winners" << "Dummy Winners" << "Decl Entries" << "Dummy Entries";
    ui->holdings->setHorizontalHeaderLabels(labels);
    ui->holdings->setColumnWidth(0,20);
    ui->holdings->setColumnWidth(1,150);
    ui->holdings->setColumnWidth(2,150);
    ui->holdings->setColumnWidth(3,150);
    ui->holdings->setColumnWidth(4,150);

    labels.clear();
    labels << "South" << "West" << "North" << "East";
    ui->cardLocationsObserver->addItems(labels);
    ui->cardLocationsTarget->addItems(labels);
}

CStatusWnd::~CStatusWnd()
{
    delete ui;
}

void CStatusWnd::on_cardLocationsObserver_activated(int index)
{

}

void CStatusWnd::on_cardLocationsTarget_activated(int index)
{

}

void CStatusWnd::on_playAnalysisShowWest_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked)
        ui->analysisWest->hide();
    else
        ui->analysisWest->show();
}

void CStatusWnd::on_playAnalysisShowNorth_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked)
        ui->analysisNorth->hide();
    else
        ui->analysisNorth->show();

}

void CStatusWnd::on_playAnalysisShowEast_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked)
        ui->analysisEast->hide();
    else
        ui->analysisEast->show();

}

void CStatusWnd::on_playAnalysisShowSouth_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked)
        ui->analysisSouth->hide();
    else
        ui->analysisSouth->show();

}

void CStatusWnd::on_cardLocationsObserver_currentIndexChanged(int index)
{

}

void CStatusWnd::on_cardLocationsTarget_currentIndexChanged(int index)
{

}

void CStatusWnd::closeEvent(QCloseEvent * e)
{
    emit UpdateShowAnalysis();
    e->accept();
}
