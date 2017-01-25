/* Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CBidEngine.h
  Developers: eak

  Revision History:
  13-jun-2013 eak: Original

  Abstract: Bid engine.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the bid engine.
 */

#ifndef CBIDENGINE_H
#define CBIDENGINE_H

class CBidDB;
class CBidDesc;
class CBidOptionDoc;
class CBidDBDefine;

/**
 * @brief The bid engine class handles automatic bidding.
 */
class CBidEngine
{
public:
    CBidEngine(CBidDB *bidDB, CBidDesc *bidDesc,
               CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc);
    virtual ~CBidEngine();

private:
    CBidDB *bidDB;
    CBidDesc *bidDesc;
    CBidDBDefine *bidDBDefine;
};

#endif // CBIDENGINE_H
