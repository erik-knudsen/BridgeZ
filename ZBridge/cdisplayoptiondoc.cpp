/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CDisplayOptionDoc.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Display options (data).

  Platforms: Qt.

*/

/**
 * \file
 * Display options (data) (definition).
 */

#include <QSettings>

#include "cdisplayoptiondoc.h"
#include "Defines.h"

CDisplayOptionDoc::CDisplayOptionDoc()
{
    cardBack = 0;
    backgroundRed = backgroundBlue = backgroundGreen = 0;
}

// Assignment operator
CDisplayOptionDoc& CDisplayOptionDoc::operator=(const CDisplayOptionDoc& displayOptionDoc)
{
    // check for self-assignment
    if (this == &displayOptionDoc)
        return *this;

    cardBack = displayOptionDoc.cardBack;
    backgroundRed = displayOptionDoc.backgroundRed;
    backgroundGreen = displayOptionDoc.backgroundGreen;
    backgroundBlue = displayOptionDoc.backgroundBlue;

    return *this;
}

void CDisplayOptionDoc::WriteSettings()
{
    QSettings settings("ZBridge settings", "Display");

    settings.setValue("cardBack", cardBack);
    settings.setValue("backgroundRed", backgroundRed);
    settings.setValue("backgroundGreen", backgroundGreen);
    settings.setValue("backgroundBlue", backgroundBlue);
}

void CDisplayOptionDoc::ReadSettings()
{
    QSettings settings("ZBridge settings", "Display");

    cardBack = settings.value("cardBack", 0).toInt();
    backgroundRed = settings.value("backgroundRed", 0).toInt();
    backgroundGreen = settings.value("backgroundGreen", 0).toInt();
    backgroundBlue = settings.value("backgroundBlue", 0).toInt();
}
