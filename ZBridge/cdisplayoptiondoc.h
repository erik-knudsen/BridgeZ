/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File:CDisplayOptionDoc.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Display option data.

  Platforms: Qt.

*/

/**
 * \file
 * Display option data (declaration).
 */

#ifndef CDISPLAYOPTIONDOC_H
#define CDISPLAYOPTIONDOC_H

class QString;
class QColor;

/**
 * @brief Diaplay options.
 *
 * This class has the display options and methods to save and restore the options.
 */
class CDisplayOptionDoc
{
public:
    CDisplayOptionDoc();

    CDisplayOptionDoc& operator=(const CDisplayOptionDoc& displayOptionDoc);

    int cardBack;
    int backgroundRed, backgroundGreen, backgroundBlue;

    void WriteSettings();
    void ReadSettings();
};

#endif // CDISPLAYOPTIONDOC_H
