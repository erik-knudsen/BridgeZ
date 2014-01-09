/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CWizardOptionDoc.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Wizard data.

  Platforms: Qt.
*/

/**
 * \file
 * Wizard data (declaration).
 */

#ifndef CWIZARDOPTIONDOC_H
#define CWIZARDOPTIONDOC_H

/**
 * @brief Wizard options.
 *
 * This class has the wizrd options and methods to save and restore the options.
 */
class CWizardOptionDoc
{
public:
    CWizardOptionDoc();

    void WriteSettings();
    void ReadSettings();

    //Help Level.
    int helpLevel;
    bool enableTracing;

    //View Settings.
    bool autoShowBidHistory;
    bool autoShowPlayHistory;
    bool useSuitSymbols;

    //Suits Display.
    int suitSequence;
    bool showDummyTrumpsOnLeft;

    //Game Options.
    bool useDuplicateScoring;
    bool scoreHonorsBonuses;

    //Pauses.
    bool insertBiddingPause;
    int biddingPauseLength;
    bool insertPlayPause;
    int playPauseLenght;

    //Bidding Style.
    int biddingStyle;
};

#endif // CWIZARDOPTIONDOC_H
