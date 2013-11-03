/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CDealOptionDoc.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: Deal option data.

  Platforms: Qt.

*/

/**
 * \file
 * Deal option data (definition).
 */

#include <QSettings>

#include "cdealoptiondoc.h"
#include "Defines.h"

CDealOptionDoc::CDealOptionDoc()
{
    minGamePts = 26;
    maxGamePts = 32;
    minMinorGamePts = 29;
    maxMinorGamePts = 32;
    minMajorGamePts = 26;
    maxMajorGamePts = 32;
    minNoTrumpGamePts = 26;
    maxNoTrumpGamePts = 32;
    minSlamPts = 33;
    maxSlamPts = 38;
    minSmallSlamPts = 33;
    maxSmallSlamPts = 36;
    minGrandSlamPts = 37;
    maxGrandSlamPts = 38;

    //Minors and Majors
    minCardsInMajor = EIGHT_MIN_CARDS_IN_MAJOR;
    minCardsInMinor = EIGHT_MIN_CARDS_IN_MINOR;
    minSuitDistMinor = FOUR_FOUR_MIN_SUIT_DIST_MINOR;
    minSuitDistMajor = FOUR_FOUR_MIN_SUIT_DIST_MAJOR;
    minTopMajorCard = ACE_MIN_TOP_MAJOR_CARD;
    minTopMinorCard = ACE_MIN_TOP_MINOR_CARD;

    //NT
    needTwoBalancedHandsForNT = false;

    //Slam.
    acesNeededForSlam = THREE_ACES_NEEDED_FOR_SLAM;
    acesNeededForSmallSlam = THREE_ACES_NEEDED_FOR_SMALL_SLAM;
    acesNeededForGrandSlam = FOUR_ACES_NEEDED_FOR_SLAM;
    kingsNeededForSlam = THREE_KINGS_NEEDED_FOR_SLAM;
    kingsNeededForSmallSlam = THREE_KINGS_NEEDED_FOR_SMALL_SLAM;
    kingsNeededForGrandSlam = THREE_KINGS_NEEDED_FOR_GRAND_SLAM;

    //Misc.
    giveSouthBestHand = false;
    balancedHands = false;
    enableDealNumbering = false;
}

void CDealOptionDoc::WriteSettings()
{
    QSettings settings("ZBridge settings", "Deals");

    settings.setValue("minGamePts", minGamePts);
    settings.setValue("maxGamePts", maxGamePts);
    settings.setValue("minMinorGamePts", minMinorGamePts);
    settings.setValue("maxMinorGamePts", maxMinorGamePts);
    settings.setValue("minMajorGamePts", minMajorGamePts);
    settings.setValue("maxMajorGamePts", maxMajorGamePts);
    settings.setValue("minNoTrumpGamePts", minNoTrumpGamePts);
    settings.setValue("maxNoTrumpGamePts", maxNoTrumpGamePts);
    settings.setValue("minSlamPts", minSlamPts);
    settings.setValue("maxSlamPts", maxSlamPts);
    settings.setValue("minSmallSlamPts", minSmallSlamPts);
    settings.setValue("maxSmallSlamPts", maxSmallSlamPts);
    settings.setValue("minGrandSlamPts", minGrandSlamPts);
    settings.setValue("maxGrandSlamPts", maxGrandSlamPts);
    settings.setValue("minCardsInMajor", minCardsInMajor);
    settings.setValue("minCardsInMinor", minCardsInMinor);
    settings.setValue("minSuitDistMinor", minSuitDistMinor);
    settings.setValue("minSuitDistMajor", minSuitDistMajor);
    settings.setValue("minTopMajorCard", minTopMajorCard);
    settings.setValue("minTopMinorCard", minTopMinorCard);
    settings.setValue("needTwoBalancedHandsForNT", needTwoBalancedHandsForNT);
    settings.setValue("acesNeededForSlam", acesNeededForSlam);
    settings.setValue("acesNeededForSmallSlam", acesNeededForSmallSlam);
    settings.setValue("acesNeededForGrandSlam", acesNeededForGrandSlam);
    settings.setValue("kingsNeededForSlam", kingsNeededForSlam);
    settings.setValue("kingsNeededForSmallSlam", kingsNeededForSmallSlam);
    settings.setValue("kingsNeededForGrandSlam", kingsNeededForGrandSlam);
    settings.setValue("giveSouthBestHand", giveSouthBestHand);
    settings.setValue("balancedHands", balancedHands);
    settings.setValue("enableDealNumbering", enableDealNumbering);
}

void CDealOptionDoc::ReadSettings()
{
    QSettings settings("ZBridge settings", "Deals");

    minGamePts = settings.value("minGamePts", 26).toInt();
    maxGamePts = settings.value("maxGamePts", 32).toInt();
    minMinorGamePts = settings.value("minMinorGamePts", 29).toInt();
    maxMinorGamePts = settings.value("maxMinorGamePts", 32).toInt();
    minMajorGamePts = settings.value("minMajorGamePts", 26).toInt();
    maxMajorGamePts = settings.value("maxMajorGamePts", 32).toInt();
    minNoTrumpGamePts = settings.value("minNoTrumpGamePts", 26).toInt();
    maxNoTrumpGamePts = settings.value("maxNoTrumpGamePts", 32).toInt();
    minSlamPts = settings.value("minSlamPts", 33).toInt();
    maxSlamPts = settings.value("maxSlamPts", 36).toInt();
    minSmallSlamPts = settings.value("minSmallSlamPts", 33).toInt();
    maxSmallSlamPts = settings.value("maxSmallSlamPts", 36).toInt();
    minGrandSlamPts = settings.value("minGrandSlamPts", 37).toInt();
    maxGrandSlamPts = settings.value("maxGrandSlamPts", 38).toInt();
    minCardsInMajor = settings.value("minCardsInMajor", EIGHT_MIN_CARDS_IN_MAJOR).toInt();
    minCardsInMinor = settings.value("minCardsInMinor", EIGHT_MIN_CARDS_IN_MINOR).toInt();
    minSuitDistMinor = settings.value("minSuitDistMinor", FOUR_FOUR_MIN_SUIT_DIST_MINOR).toInt();
    minSuitDistMajor = settings.value("minSuitDistMajor", FOUR_FOUR_MIN_SUIT_DIST_MAJOR).toInt();
    minTopMajorCard = settings.value("minTopMajorCard", ACE_MIN_TOP_MAJOR_CARD).toInt();
    minTopMinorCard = settings.value("minTopMinorCard", ACE_MIN_TOP_MINOR_CARD).toInt();
    needTwoBalancedHandsForNT = settings.value("needTwoBalancedHandsForNT", false).toBool();
    acesNeededForSlam = settings.value("acesNeededForSlam", THREE_ACES_NEEDED_FOR_SLAM).toInt();
    acesNeededForSmallSlam = settings.value("acesNeededForSmallSlam", THREE_ACES_NEEDED_FOR_SMALL_SLAM).toInt();
    acesNeededForGrandSlam = settings.value("acesNeededForGrandSlam", FOUR_ACES_NEEDED_FOR_SLAM).toInt();
    kingsNeededForSlam = settings.value("kingsNeededForSlam", THREE_KINGS_NEEDED_FOR_SLAM).toInt();
    kingsNeededForSmallSlam = settings.value("kingsNeededForSmallSlam", THREE_KINGS_NEEDED_FOR_SMALL_SLAM).toInt();
    kingsNeededForGrandSlam = settings.value("kingsNeededForGrandSlam", THREE_KINGS_NEEDED_FOR_GRAND_SLAM).toInt();
    giveSouthBestHand = settings.value("giveSouthBestHand", false).toBool();
    balancedHands = settings.value("balancedHands", false).toBool();
    enableDealNumbering = settings.value("enableDealNumbering", false).toBool();
}
