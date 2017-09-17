/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File:
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Seat option data.

  Platforms: Qt.
*/

/**
 * \file
 * Seat option data
 */

#include <QSettings>

#include "cseatoptiondoc.h"
#include "Defines.h"

CSeatOptionDoc::CSeatOptionDoc()
{
    seat = SOUTH_SEAT;
    role = STANDALONE_ROLE;
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
    int tmp;

    QSettings settings("ZBridge settings", "Seat");

    tmp = settings.value("seat", SOUTH_SEAT).toInt();
    seat = ((tmp >= WEST_SEAT) && (tmp <= SOUTH_SEAT)) ? (Seat)tmp : SOUTH_SEAT;
    tmp = settings.value("role", STANDALONE_ROLE).toInt();
    role = ((tmp >= SERVER_ROLE) && (tmp <= STANDALONE_ROLE)) ? (Role)tmp : STANDALONE_ROLE;
    tmp = settings.value("protocol", PROTOCOL_V0).toInt();
    protocol = PROTOCOL_V0;         //Only this protocol at present.
    hostServer = settings.value("hostServer", "").toString();
    portServer = settings.value("portServer", "").toString();
    hostClient = settings.value("hostClient", "").toString();
    portClient = settings.value("portClient", "").toString();
    westName = settings.value("westName", "W").toString();
    northName = settings.value("northName", "N").toString();
    eastName = settings.value("eastName", "E").toString();
    southName = settings.value("southName", "S").toString();
    tmp = settings.value("westActor", AUTO_ACTOR).toInt();
    westActor = ((tmp >= MANUAL_ACTOR) && (tmp <= REMOTE_ACTOR)) ? (Actor)tmp : AUTO_ACTOR;
    tmp = settings.value("northActor", AUTO_ACTOR).toInt();
    northActor = ((tmp >= MANUAL_ACTOR) && (tmp <= REMOTE_ACTOR)) ? (Actor)tmp : AUTO_ACTOR;
    tmp = settings.value("eastActor", "").toInt();
    eastActor = ((tmp >= MANUAL_ACTOR) && (tmp <= REMOTE_ACTOR)) ? (Actor)tmp : AUTO_ACTOR;
    tmp = settings.value("southActor", "").toInt();
    southActor = ((tmp >= MANUAL_ACTOR) && (tmp <= REMOTE_ACTOR)) ? (Actor)tmp : AUTO_ACTOR;
}
