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

    if (seatOptionDoc.role == SERVER_ROLE)
        ui->server->setChecked(true);
    else
        ui->client->setChecked(true);

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
}

void CSeatConfiguration::on_north_clicked()
{
    seatOptionDoc.seat = SEAT_NORTH;
}

void CSeatConfiguration::on_east_clicked()
{
    seatOptionDoc.seat = SEAT_EAST;
}

void CSeatConfiguration::on_south_clicked()
{
    seatOptionDoc.seat = SEAT_SOUTH;
}

void CSeatConfiguration::on_westActor_currentIndexChanged(int index)
{
    seatOptionDoc.westActor = index;
}

void CSeatConfiguration::on_northActor_currentIndexChanged(int index)
{
    seatOptionDoc.northActor = index;
}

void CSeatConfiguration::on_eastActor_currentIndexChanged(int index)
{
    seatOptionDoc.eastActor = index;
}

void CSeatConfiguration::on_southActor_currentIndexChanged(int index)
{
    seatOptionDoc.southActor = index;
}

void CSeatConfiguration::on_server_clicked()
{
    seatOptionDoc.role = SERVER_ROLE;
}

void CSeatConfiguration::on_client_clicked()
{
    seatOptionDoc.role = CLIENT_ROLE;
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
