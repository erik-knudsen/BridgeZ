/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CDealOptionDoc.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Deal option data.

  Platforms: Qt.

*/

/**
 * \file
 * Deal option data (declaration).
 */

#ifndef CDEALOPTIONDOC_H
#define CDEALOPTIONDOC_H

/**
 * @brief Deal options.
 *
 * This class has the deal options and methods to save and restore the options.
 */
class CDealOptionDoc
{
public:
    CDealOptionDoc();

    void WriteSettings();
    void ReadSettings();

    //Points
    int minGamePts;
    int maxGamePts;
    int minMinorGamePts;
    int maxMinorGamePts;
    int minMajorGamePts;
    int maxMajorGamePts;
    int minNoTrumpGamePts;
    int maxNoTrumpGamePts;
    int minSlamPts;
    int maxSlamPts;
    int minSmallSlamPts;
    int maxSmallSlamPts;
    int minGrandSlamPts;
    int maxGrandSlamPts;

    //Minors and Majors
    int minCardsInMajor;
    int minCardsInMinor;
    int minSuitDistMinor;
    int minSuitDistMajor;
    int minTopMajorCard;
    int minTopMinorCard;

    //NT
    bool needTwoBalancedHandsForNT;

    ////Slam.
    int acesNeededForSlam;
    int acesNeededForSmallSlam;
    int acesNeededForGrandSlam;
    int kingsNeededForSlam;
    int kingsNeededForSmallSlam;
    int kingsNeededForGrandSlam;

    //Misc.
    bool giveSouthBestHand;
    bool balancedHands;
    bool enableDealNumbering;
};

#endif // CDEALOPTIONDOC_H
