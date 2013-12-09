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

CSeatConfiguration::CSeatConfiguration(CZBridgeApp *app, CZBridgeDoc *doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSeatConfiguration)
{
    ui->setupUi(this);

    this->app = app;
    this->doc = doc;

    QStringList labels;
    labels << ACTOR_NAMES[ACTOR_MANUAL] << ACTOR_NAMES[ACTOR_AUTO];
    ui->westActor->addItems(labels);
    ui->northActor->addItems(labels);
    ui->eastActor->addItems(labels);
    ui->southActor->addItems(labels);

    seatOptionDoc = doc->getSeatOptions();

    if (seatOptionDoc.seat == SEAT_WEST)
        ui->west->setChecked(true);
    else if (seatOptionDoc.seat == SEAT_NORTH)
        ui->north->setChecked(true);
    else if (seatOptionDoc.seat == SEAT_EAST)
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

    updateSeatAndActor();

    labels.clear();
    labels << ROLE_NAMES[ROLE_SERVER] << ROLE_NAMES[ROLE_CLIENT];
    ui->role->addItems(labels);
    ui->role->setCurrentIndex(seatOptionDoc.role);

    labels.clear();
    labels << PROTOCOL_NAMES[PROTOCOL_BASIC] << PROTOCOL_NAMES[PROTOCOL_ADVANCED];
    ui->protocol->addItems(labels);
    ui->protocol->setCurrentIndex(seatOptionDoc.protocol);

    ui->host->setText(seatOptionDoc.host);
    ui->port->setText(seatOptionDoc.port);
}

CSeatConfiguration::~CSeatConfiguration()
{
    delete ui;
}

void CSeatConfiguration::on_west_clicked()
{
    seatOptionDoc.seat = SEAT_WEST;
    updateSeatAndActor();
}

void CSeatConfiguration::on_north_clicked()
{
    seatOptionDoc.seat = SEAT_NORTH;
    updateSeatAndActor();
}

void CSeatConfiguration::on_east_clicked()
{
    seatOptionDoc.seat = SEAT_EAST;
    updateSeatAndActor();
}

void CSeatConfiguration::on_south_clicked()
{
    seatOptionDoc.seat = SEAT_SOUTH;
    updateSeatAndActor();
}

void CSeatConfiguration::on_westActor_currentIndexChanged(int index)
{
    seatOptionDoc.westActor = index;
    updateSeatAndActor();
}

void CSeatConfiguration::on_northActor_currentIndexChanged(int index)
{
    seatOptionDoc.northActor = index;
    updateSeatAndActor();
}

void CSeatConfiguration::on_eastActor_currentIndexChanged(int index)
{
    seatOptionDoc.eastActor = index;
    updateSeatAndActor();
}

void CSeatConfiguration::on_southActor_currentIndexChanged(int index)
{
    seatOptionDoc.southActor = index;
    updateSeatAndActor();
}

void CSeatConfiguration::on_role_currentIndexChanged(int index)
{
    seatOptionDoc.role = index;
    updateSeatAndActor();
}

void CSeatConfiguration::on_protocol_currentIndexChanged(int index)
{
    seatOptionDoc.protocol = index;
}

void CSeatConfiguration::on_buttonBox_accepted()
{
    seatOptionDoc.westName = ui->westName->text();
    seatOptionDoc.northName = ui->northName->text();
    seatOptionDoc.eastName = ui->eastName->text();
    seatOptionDoc.southName = ui->southName->text();
    seatOptionDoc.host = ui->host->text();
    seatOptionDoc.port = ui->port->text();
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

    if (seatOptionDoc.role == ROLE_SERVER)
    {
        if (seatOptionDoc.westActor == ACTOR_AUTO)
            ui->westName->setVisible(false);
        if (seatOptionDoc.northActor == ACTOR_AUTO)
            ui->northName->setVisible(false);
        if (seatOptionDoc.eastActor == ACTOR_AUTO)
            ui->eastName->setVisible(false);
        if (seatOptionDoc.southActor == ACTOR_AUTO)
            ui->southName->setVisible(false);
    }
    else if (seatOptionDoc.role == ROLE_CLIENT)
    {
        if (seatOptionDoc.seat != WEST_SEAT)
        {
            ui->westName->setVisible(false);
            ui->westActor->setVisible(false);
        }
        else if (seatOptionDoc.westActor == ACTOR_AUTO)
            ui->westName->setVisible(false);
        if (seatOptionDoc.seat != NORTH_SEAT)
        {
            ui->northName->setVisible(false);
            ui->northActor->setVisible(false);
        }
        else if (seatOptionDoc.northActor == ACTOR_AUTO)
            ui->northName->setVisible(false);
        if (seatOptionDoc.seat != EAST_SEAT)
        {
            ui->eastName->setVisible(false);
            ui->eastActor->setVisible(false);
        }
        else if (seatOptionDoc.eastActor == ACTOR_AUTO)
            ui->eastName->setVisible(false);
        if (seatOptionDoc.seat != SOUTH_SEAT)
        {
            ui->southName->setVisible(false);
            ui->southActor->setVisible(false);
        }
        else if (seatOptionDoc.southActor == ACTOR_AUTO)
            ui->southName->setVisible(false);
    }
}
