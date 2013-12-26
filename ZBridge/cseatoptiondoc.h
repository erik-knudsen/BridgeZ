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

#ifndef CSEATOPTIONDOC_H
#define CSEATOPTIONDOC_H

#include "Defines.h"

class QString;

class CSeatOptionDoc
{
public:
    CSeatOptionDoc();

    CSeatOptionDoc& operator=(const CSeatOptionDoc& seatOptionDoc);

    void WriteSettings();
    void ReadSettings();

    Seat seat;
    Role role;
    Protocol protocol;
    QString hostServer;
    QString portServer;
    QString hostClient;
    QString portClient;
    QString westName;
    QString northName;
    QString eastName;
    QString southName;
    Actor westActor;
    Actor northActor;
    Actor eastActor;
    Actor southActor;
};

#endif // CSEATOPTIONDOC_H
