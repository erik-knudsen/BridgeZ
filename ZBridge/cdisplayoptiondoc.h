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

class CDisplayOptionDoc
{
public:
    CDisplayOptionDoc();

    CDisplayOptionDoc& operator=(const CDisplayOptionDoc& displayOptionDoc);

    int suitSequence;
    bool showDummyTrumpsOnLeft;

    bool tripleBuffer;
    bool animateCards;
    bool granularity;

    int cardBack;

    bool autoShowBidHistory;
    bool autoHideBidHistory;
    bool autoShowPlayHistory;
    bool autoHidePlayHistory;
    bool showLayoutWhenEditing;

    QString autoHintDialogFont;
    QString bidAndPlayHistoryDialogFont;
    QString analysisDialogFont;
    bool useSuitSymbols;

    bool lowResolutionOption;
    bool showStartupBanner;
    bool showBackgroundBitmap;
    int bitmapMode;
    bool scaleLargeBitmaps;
    QString filePath;
    int backgroundRed, backgroundGreen, backgroundBlue;

    void WriteSettings();
    void ReadSettings();
};

#endif // CDISPLAYOPTIONDOC_H
