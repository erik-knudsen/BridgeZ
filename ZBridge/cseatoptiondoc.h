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

class QString;

class CSeatOptionDoc
{
public:
    CSeatOptionDoc();

    CSeatOptionDoc& operator=(const CSeatOptionDoc& seatOptionDoc);

    void WriteSettings();
    void ReadSettings();

    int seat;
    int role;
    int protocol;
    QString hostServer;
    QString portServer;
    QString hostClient;
    QString portClient;
    QString westName;
    QString northName;
    QString eastName;
    QString southName;
    int westActor;
    int northActor;
    int eastActor;
    int southActor;
};

#endif // CSEATOPTIONDOC_H
