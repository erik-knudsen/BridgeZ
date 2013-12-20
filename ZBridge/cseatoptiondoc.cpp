/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File:
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract:

  Platforms: Qt.

*/

/**
 * \file
 *
 */

#include <QSettings>

#include "cseatoptiondoc.h"
#include "Defines.h"

CSeatOptionDoc::CSeatOptionDoc()
{
    seat = SEAT_SOUTH;
    role = ROLE_STANDALONE;
}

// Assignment operator
CSeatOptionDoc& CSeatOptionDoc::operator=(const CSeatOptionDoc& seatOptionDoc)
{
    // check for self-assignment
    if (this == &seatOptionDoc)
        return *this;

    seat = seatOptionDoc.seat;
    role = seatOptionDoc.role;
    protocol = seatOptionDoc.protocol;
    hostServer = seatOptionDoc.hostServer;
    portServer = seatOptionDoc.portServer;
    hostClient = seatOptionDoc.hostClient;
    portClient = seatOptionDoc.portClient;
    westName = seatOptionDoc.westName;
    northName = seatOptionDoc.northName;
    eastName = seatOptionDoc.eastName;
    southName = seatOptionDoc.southName;
    westActor = seatOptionDoc.westActor;
    northActor = seatOptionDoc.northActor;
    eastActor = seatOptionDoc.eastActor,
    southActor = seatOptionDoc.southActor;

    return *this;
}

void CSeatOptionDoc::WriteSettings()
{
    QSettings settings("ZBridge settings", "Seat");

    settings.setValue("seat", seat);
    settings.setValue("role", role);
    settings.setValue("protocol", protocol);
    settings.setValue("hostServer", hostServer);
    settings.setValue("portServer", portServer);
    settings.setValue("hostClient", hostClient);
    settings.setValue("portClient", portClient);
    settings.setValue("westName", westName);
    settings.setValue("northName", northName);
    settings.setValue("eastName", eastName);
    settings.setValue("southName", southName);
    settings.setValue("westActor", westActor);
    settings.setValue("northActor", northActor);
    settings.setValue("eastActor", eastActor);
    settings.setValue("southActor", southActor);
}

void CSeatOptionDoc::ReadSettings()
{
    QSettings settings("ZBridge settings", "Seat");

    seat = settings.value("seat", SEAT_SOUTH).toInt();
    role = settings.value("role", ROLE_STANDALONE).toInt();
    protocol = settings.value("protocol", NET_PROTOCOL_ADV).toInt();
    hostServer = settings.value("hostServer", "").toString();
    portServer = settings.value("portServer", "").toString();
    hostClient = settings.value("hostClient", "").toString();
    portClient = settings.value("portClient", "").toString();
    westName = settings.value("westName", "W").toString();
    northName = settings.value("northName", "N").toString();
    eastName = settings.value("eastName", "E").toString();
    southName = settings.value("southName", "S").toString();
    westActor = settings.value("westActor", "").toInt();
    northActor = settings.value("northActor", "").toInt();
    eastActor = settings.value("eastActor", "").toInt();
    southActor = settings.value("southActor", "").toInt();
}
