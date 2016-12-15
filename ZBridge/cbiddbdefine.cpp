/*Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBidDBDefine.cpp
  Developers: eak

  Revision History:
  1-dec-2016 eak: Original

  Abstract: Defines for bidding database.

  Platforms: Qt.

*/

#include "cbiddbdefine.h"

CBidDBDefine::CBidDBDefine(CZBridgeDoc *doc)
{
    nsBidOptions = doc->getNSBidOptions();
    ewBidOptions = doc->getEWBidOptions();

    calcIds(nsBidOptions, nsPages, nsRules);
    calcIds(ewBidOptions, ewPages, ewRules);
}

void CBidDBDefine::calcIds(CBidOptionDoc &bidOptions, QSet<qint16> &pages, QSet<qint16> &rules)
{

}

