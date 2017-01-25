/* Erik Aagaard Knudsen.
  Copyright © 2017 - All Rights Reserved

  Project: ZBridge
  File: CBidAndPlayEngines.h
  Developers: eak

  Revision History:
  25-jan-2017 eak: Original

  Abstract: Bid and play engines.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the bid and the play engines.
 */

#ifndef CBIDANDPLAYENGINES_H
#define CBIDANDPLAYENGINES_H

#include "cfeatures.h"

class CBidDB;
class CBidDesc;
class CBidOptionDoc;
class CBidEngine;
class CPlayEngine;

class CBidAndPlayEngines
{
public:
    CBidAndPlayEngines(CBidDB *bidDB, CBidDesc *bidDesc,
                       CBidOptionDoc &nsBidOptionDoc, CBidOptionDoc &ewBidOptionDoc);
    virtual ~CBidAndPlayEngines();

private:
    CBidEngine *bidEngine;
    CPlayEngine *playEngine;

    CFeatures features[2][4];
};

#endif // CBIDANDPLAYENGINES_H
