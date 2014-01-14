/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CSeatConfiguration.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Configuration of seats.

  Platforms: Qt.

*/

/**
 * \file
 * Configuration of seats (definition).
 */

#include "cseatconfiguration.h"
#include "ui_cseatconfiguration.h"
#include "czbridgeapp.h"
#include "czbridgedoc.h"

//Protocol name index.
const int PROTOCOL_BASIC = 0;
const int PROTOCOL_ADVANCED = 1;

CSeatConfiguration::CSeatConfiguration(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSeatConfiguration)
{
    ui->setupUi(this);

    this->app = app;
    this->doc = doc;

    //First set up combo boxes (NOTE: current index changed is called with index 0).
    QStringList labels;
    labels << tr("Manual") << tr("Auto");
    ui->westActor->addItems(labels);
    ui->northActor->addItems(labels);
    ui->eastActor->addItems(labels);
    ui->southActor->addItems(labels);

    labels.clear();
    labels << tr("Server") << tr("Client") << tr("Standalone");
    ui->role->addItems(labels);

    labels.clear();
    labels << tr("Basic") << tr("Advanced");
    ui->protocol->addItems(labels);

    //Next set up dialog based on saved values.
    seatOptionDoc = doc->getSeatOptions();

    if (seatOptionDoc.seat == WEST_SEAT)
        ui->west->setChecked(true);
    else if (seatOptionDoc.seat == NORTH_SEAT)
        ui->north->setChecked(true);
    else if (seatOptionDoc.seat == EAST_SEAT)
        ui->east->setChecked(true);
    else
        ui->south->setChecked(true);

    ui->westName->setText(seatOptionDoc.westName);
    ui->northName->setText(seatOptionDoc.northName);
    ui->eastName->setText(seatOptionDoc.eastName);
    ui->southName->setText(seatOptionDoc.southName);

    ui->westActor->setCurrentIndex(seatOptionDoc.westActor);
    ui->northActor->setCurrentIndex(seatOptionDoc.northActor);
    ui->eastActor->setCurrentIndex(seatOptionDoc.eastActor);
    ui->southActor->setCurrentIndex(seatOptionDoc.southActor);

    ui->role->setCurrentIndex(seatOptionDoc.role);
    ui->protocol->setCurrentIndex((seatOptionDoc.protocol == BASIC_PROTOCOL) ? PROTOCOL_BASIC : PROTOCOL_ADVANCED);

    //Enable/disable based on saved values.
    updateHostAndPort();
    updateSeatAndActor();
}

CSeatConfiguration::~CSeatConfiguration()
{
    delete ui;
}

void CSeatConfiguration::on_west_clicked()
{
    seatOptionDoc.seat = WEST_SEAT;
    updateSeatAndActor();
}

void CSeatConfiguration::on_north_clicked()
{
    seatOptionDoc.seat = NORTH_SEAT;
    updateSeatAndActor();
}

void CSeatConfiguration::on_east_clicked()
{
    seatOptionDoc.seat = EAST_SEAT;
    updateSeatAndActor();
}

void CSeatConfiguration::on_south_clicked()
{
    seatOptionDoc.seat = SOUTH_SEAT;
    updateSeatAndActor();
}

void CSeatConfiguration::on_westActor_currentIndexChanged(int index)
{
    seatOptionDoc.westActor = (Actor)index;
    updateSeatAndActor();
}

void CSeatConfiguration::on_northActor_currentIndexChanged(int index)
{
    seatOptionDoc.northActor = (Actor)index;
    updateSeatAndActor();
}

void CSeatConfiguration::on_eastActor_currentIndexChanged(int index)
{
    seatOptionDoc.eastActor = (Actor)index;
    updateSeatAndActor();
}

void CSeatConfiguration::on_southActor_currentIndexChanged(int index)
{
    seatOptionDoc.southActor = (Actor)index;
    updateSeatAndActor();
}

void CSeatConfiguration::on_role_currentIndexChanged(int index)
{
    seatOptionDoc.role = (Role)index;
    updateHostAndPort();
    updateSeatAndActor();
}

void CSeatConfiguration::on_protocol_currentIndexChanged(int index)
{
    seatOptionDoc.protocol = (index == PROTOCOL_BASIC) ? BASIC_PROTOCOL : ADVANCED_PROTOCOL;
}

void CSeatConfiguration::on_buttonBox_accepted()
{
    seatOptionDoc.westName = ui->westName->text();
    seatOptionDoc.northName = ui->northName->text();
    seatOptionDoc.eastName = ui->eastName->text();
    seatOptionDoc.southName = ui->southName->text();
    if (seatOptionDoc.role == SERVER_ROLE)
    {
        seatOptionDoc.hostServer = ui->host->text();
        seatOptionDoc.portServer = ui->port->text();
    }
    else if (seatOptionDoc.role == CLIENT_ROLE)
    {
        seatOptionDoc.hostClient = ui->host->text();
        seatOptionDoc.portClient = ui->port->text();
    }
    doc->setSeatOptions(seatOptionDoc);
}

void CSeatConfiguration::updateSeatAndActor()
{
    ui->westName->setVisible(true);
    ui->westActor->setVisible(true);
    ui->northName->setVisible(true);
    ui->northActor->setVisible(true);
    ui->eastName->setVisible(true);
    ui->eastActor->setVisible(true);
    ui->southName->setVisible(true);
    ui->southActor->setVisible(true);

    if ((seatOptionDoc.role == SERVER_ROLE) || (seatOptionDoc.role == STANDALONE_ROLE))
    {
        if (seatOptionDoc.westActor == AUTO_ACTOR)
            ui->westName->setVisible(false);
        if (seatOptionDoc.northActor == AUTO_ACTOR)
            ui->northName->setVisible(false);
        if (seatOptionDoc.eastActor == AUTO_ACTOR)
            ui->eastName->setVisible(false);
        if (seatOptionDoc.southActor == AUTO_ACTOR)
            ui->southName->setVisible(false);
    }
    else if (seatOptionDoc.role == CLIENT_ROLE)
    {
        if (seatOptionDoc.seat != WEST_SEAT)
        {
            ui->westName->setVisible(false);
            ui->westActor->setVisible(false);
        }
        else if (seatOptionDoc.westActor == AUTO_ACTOR)
            ui->westName->setVisible(false);
        if (seatOptionDoc.seat != NORTH_SEAT)
        {
            ui->northName->setVisible(false);
            ui->northActor->setVisible(false);
        }
        else if (seatOptionDoc.northActor == AUTO_ACTOR)
            ui->northName->setVisible(false);
        if (seatOptionDoc.seat != EAST_SEAT)
        {
            ui->eastName->setVisible(false);
            ui->eastActor->setVisible(false);
        }
        else if (seatOptionDoc.eastActor == AUTO_ACTOR)
            ui->eastName->setVisible(false);
        if (seatOptionDoc.seat != SOUTH_SEAT)
        {
            ui->southName->setVisible(false);
            ui->southActor->setVisible(false);
        }
        else if (seatOptionDoc.southActor == AUTO_ACTOR)
            ui->southName->setVisible(false);
    }
}

void CSeatConfiguration::updateHostAndPort()
{
    ui->host->setVisible(true);
    ui->host->setEnabled(true);
    ui->port->setVisible(true);
    ui->port->setEnabled(true);

    if (seatOptionDoc.role == STANDALONE_ROLE)
    {
        ui->host->setText("");
        ui->port->setText("");
        ui->host->setEnabled(false);
        ui->port->setEnabled(false);
    }
    else if (seatOptionDoc.role == SERVER_ROLE)
    {
        ui->host->setText(seatOptionDoc.hostServer);
        ui->port->setText(seatOptionDoc.portServer);
    }
    else
    {
        ui->host->setText(seatOptionDoc.hostClient);
        ui->port->setText(seatOptionDoc.portClient);
    }
}
