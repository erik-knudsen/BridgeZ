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
    Initialize();
}

void CDealOptionDoc::WriteSettings()
{
    QSettings settings("ZBridge settings", "Deals");

    //High card points.
    settings.setValue("hcpLowClubsWest", hcp[0][CLUBS][WEST_SEAT]);
    settings.setValue("hcpHighClubsWest", hcp[1][CLUBS][WEST_SEAT]);
    settings.setValue("hcpLowClubsNorth", hcp[0][CLUBS][NORTH_SEAT]);
    settings.setValue("hcpHighClubsNorth", hcp[1][CLUBS][NORTH_SEAT]);
    settings.setValue("hcpLowClubsEast", hcp[0][CLUBS][EAST_SEAT]);
    settings.setValue("hcpHighClubsEast", hcp[1][CLUBS][EAST_SEAT]);
    settings.setValue("hcpLowClubsSouth", hcp[0][CLUBS][SOUTH_SEAT]);
    settings.setValue("hcpHighClubsSouth", hcp[1][CLUBS][SOUTH_SEAT]);

    settings.setValue("hcpLowDiamondsWest", hcp[0][DIAMONDS][WEST_SEAT]);
    settings.setValue("hcpHighDiamondsWest", hcp[1][DIAMONDS][WEST_SEAT]);
    settings.setValue("hcpLowDiamondsNorth", hcp[0][DIAMONDS][NORTH_SEAT]);
    settings.setValue("hcpHighDiamondsNorth", hcp[1][DIAMONDS][NORTH_SEAT]);
    settings.setValue("hcpLowDiamondsEast", hcp[0][DIAMONDS][EAST_SEAT]);
    settings.setValue("hcpHighDiamondsEast", hcp[1][DIAMONDS][EAST_SEAT]);
    settings.setValue("hcpLowDiamondsSouth", hcp[0][DIAMONDS][SOUTH_SEAT]);
    settings.setValue("hcpHighDiamondsSouth", hcp[1][DIAMONDS][SOUTH_SEAT]);

    settings.setValue("hcpLowHeartsWest", hcp[0][HEARTS][WEST_SEAT]);
    settings.setValue("hcpHighHeartsWest", hcp[1][HEARTS][WEST_SEAT]);
    settings.setValue("hcpLowHeartsNorth", hcp[0][HEARTS][NORTH_SEAT]);
    settings.setValue("hcpHighHeartsNorth", hcp[1][HEARTS][NORTH_SEAT]);
    settings.setValue("hcpLowHeartsEast", hcp[0][HEARTS][EAST_SEAT]);
    settings.setValue("hcpHighHeartsEast", hcp[1][HEARTS][EAST_SEAT]);
    settings.setValue("hcpLowHeartsSouth", hcp[0][HEARTS][SOUTH_SEAT]);
    settings.setValue("hcpHighHeartsSouth", hcp[1][HEARTS][SOUTH_SEAT]);

    settings.setValue("hcpLowSpadesWest", hcp[0][SPADES][WEST_SEAT]);
    settings.setValue("hcpHighSpadesWest", hcp[1][SPADES][WEST_SEAT]);
    settings.setValue("hcpLowSpadesNorth", hcp[0][SPADES][NORTH_SEAT]);
    settings.setValue("hcpHighSpadesNorth", hcp[1][SPADES][NORTH_SEAT]);
    settings.setValue("hcpLowSpadesEast", hcp[0][SPADES][EAST_SEAT]);
    settings.setValue("hcpHighSpadesEast", hcp[1][SPADES][EAST_SEAT]);
    settings.setValue("hcpLowSpadesSouth", hcp[0][SPADES][SOUTH_SEAT]);
    settings.setValue("hcpHighSpadesSouth", hcp[1][SPADES][SOUTH_SEAT]);

    settings.setValue("hcpLowTotalWest", hcp[0][NOTRUMP][WEST_SEAT]);
    settings.setValue("hcpHighTotalWest", hcp[1][NOTRUMP][WEST_SEAT]);
    settings.setValue("hcpLowTotalNorth", hcp[0][NOTRUMP][NORTH_SEAT]);
    settings.setValue("hcpHighTotalNorth", hcp[1][NOTRUMP][NORTH_SEAT]);
    settings.setValue("hcpLowTotalEast", hcp[0][NOTRUMP][EAST_SEAT]);
    settings.setValue("hcpHighTotalEast", hcp[1][NOTRUMP][EAST_SEAT]);
    settings.setValue("hcpLowTotalSouth", hcp[0][NOTRUMP][SOUTH_SEAT]);
    settings.setValue("hcpHighTotalSouth", hcp[1][NOTRUMP][SOUTH_SEAT]);

    //Distribution points.
    settings.setValue("dpLowWest", dp[0][WEST_SEAT]);
    settings.setValue("dpHighWest", dp[1][WEST_SEAT]);
    settings.setValue("dpLowNorth", dp[0][NORTH_SEAT]);
    settings.setValue("dpHighNorth", dp[1][NORTH_SEAT]);
    settings.setValue("dpLowEast", dp[0][EAST_SEAT]);
    settings.setValue("dpHighEast", dp[1][EAST_SEAT]);
    settings.setValue("dpLowSouth", dp[0][SOUTH_SEAT]);
    settings.setValue("dpHighSouth", dp[1][SOUTH_SEAT]);

    //Suit sizes.
    settings.setValue("suitSizeLowClubsWest", suitSize[0][CLUBS][WEST_SEAT]);
    settings.setValue("suitSizeHighClubsWest", suitSize[1][CLUBS][WEST_SEAT]);
    settings.setValue("suitSizeLowClubsNorth", suitSize[0][CLUBS][NORTH_SEAT]);
    settings.setValue("suitSizeHighClubsNorth", suitSize[1][CLUBS][NORTH_SEAT]);
    settings.setValue("suitSizeLowClubsEast", suitSize[0][CLUBS][EAST_SEAT]);
    settings.setValue("suitSizepHighClubsEast", suitSize[1][CLUBS][EAST_SEAT]);
    settings.setValue("suitSizeLowClubsSouth", suitSize[0][CLUBS][SOUTH_SEAT]);
    settings.setValue("suitSizeHighClubsSouth", suitSize[1][CLUBS][SOUTH_SEAT]);

    settings.setValue("suitSizeLowDiamondsWest", suitSize[0][DIAMONDS][WEST_SEAT]);
    settings.setValue("suitSizeHighDiamondsWest", suitSize[1][DIAMONDS][WEST_SEAT]);
    settings.setValue("suitSizeLowDiamondsNorth", suitSize[0][DIAMONDS][NORTH_SEAT]);
    settings.setValue("suitSizeHighDiamondsNorth", suitSize[1][DIAMONDS][NORTH_SEAT]);
    settings.setValue("suitSizeLowDiamondsEast", suitSize[0][DIAMONDS][EAST_SEAT]);
    settings.setValue("suitSizeHighDiamondsEast", suitSize[1][DIAMONDS][EAST_SEAT]);
    settings.setValue("suitSizeLowDiamondsSouth", suitSize[0][DIAMONDS][SOUTH_SEAT]);
    settings.setValue("suitSizeHighDiamondsSouth", suitSize[1][DIAMONDS][SOUTH_SEAT]);

    settings.setValue("suitSizeLowHeartsWest", suitSize[0][HEARTS][WEST_SEAT]);
    settings.setValue("suitSizeHighHeartsWest", suitSize[1][HEARTS][WEST_SEAT]);
    settings.setValue("suitSizeLowHeartsNorth", suitSize[0][HEARTS][NORTH_SEAT]);
    settings.setValue("suitSizeHighHeartsNorth", suitSize[1][HEARTS][NORTH_SEAT]);
    settings.setValue("suitSizeLowHeartsEast", suitSize[0][HEARTS][EAST_SEAT]);
    settings.setValue("suitSizeHighHeartsEast", suitSize[1][HEARTS][EAST_SEAT]);
    settings.setValue("suitSizeLowHeartsSouth", suitSize[0][HEARTS][SOUTH_SEAT]);
    settings.setValue("suitSizeHighHeartsSouth", suitSize[1][HEARTS][SOUTH_SEAT]);

    settings.setValue("suitSizeLowSpadesWest", suitSize[0][SPADES][WEST_SEAT]);
    settings.setValue("suitSizeHighSpadesWest", suitSize[1][SPADES][WEST_SEAT]);
    settings.setValue("suitSizeLowSpadesNorth", suitSize[0][SPADES][NORTH_SEAT]);
    settings.setValue("suitSizeHighSpadesNorth", suitSize[1][SPADES][NORTH_SEAT]);
    settings.setValue("suitSizeLowSpadesEast", suitSize[0][SPADES][EAST_SEAT]);
    settings.setValue("suitSizeHighSpadesEast", suitSize[1][SPADES][EAST_SEAT]);
    settings.setValue("suitSizeLowSpadesSouth", suitSize[0][SPADES][SOUTH_SEAT]);
    settings.setValue("suitSizeHighSpadesSouth", suitSize[1][SPADES][SOUTH_SEAT]);
}

void CDealOptionDoc::ReadSettings()
{
    QSettings settings("ZBridge settings", "Deals");

    //High card points.
    hcp[0][CLUBS][WEST_SEAT] = settings.value("hcpLowClubsWest", 0).toInt();
    hcp[1][CLUBS][WEST_SEAT] = settings.value("hcpHighClubsWest", MAX_SUIT_HCP).toInt();
    hcp[0][CLUBS][NORTH_SEAT] = settings.value("hcpLowClubsNorth", 0).toInt();
    hcp[1][CLUBS][NORTH_SEAT] = settings.value("hcpHighClubsNorth", MAX_SUIT_HCP).toInt();
    hcp[0][CLUBS][EAST_SEAT] = settings.value("hcpLowClubsEast", 0).toInt();
    hcp[1][CLUBS][EAST_SEAT] = settings.value("hcpHighClubsEast", MAX_SUIT_HCP).toInt();
    hcp[0][CLUBS][SOUTH_SEAT] = settings.value("hcpLowClubsSouth", 0).toInt();
    hcp[1][CLUBS][SOUTH_SEAT] = settings.value("hcpHighClubsSouth", MAX_SUIT_HCP).toInt();

    hcp[0][DIAMONDS][WEST_SEAT] = settings.value("hcpLowDiamondsWest", 0).toInt();
    hcp[1][DIAMONDS][WEST_SEAT] = settings.value("hcpHighDiamondsWest", MAX_SUIT_HCP).toInt();
    hcp[0][DIAMONDS][NORTH_SEAT] = settings.value("hcpLowDiamondsNorth", 0).toInt();
    hcp[1][DIAMONDS][NORTH_SEAT] = settings.value("hcpHighDiamondsNorth", MAX_SUIT_HCP).toInt();
    hcp[0][DIAMONDS][EAST_SEAT] = settings.value("hcpLowDiamondsEast", 0).toInt();
    hcp[1][DIAMONDS][EAST_SEAT] = settings.value("hcpHighDiamondsEast", MAX_SUIT_HCP).toInt();
    hcp[0][DIAMONDS][SOUTH_SEAT] = settings.value("hcpLowDiamondsSouth", 0).toInt();
    hcp[1][DIAMONDS][SOUTH_SEAT] = settings.value("hcpHighDiamondsSouth", MAX_SUIT_HCP).toInt();

    hcp[0][HEARTS][WEST_SEAT] = settings.value("hcpLowHeartsWest", 0).toInt();
    hcp[1][HEARTS][WEST_SEAT] = settings.value("hcpHighHeartsWest", MAX_SUIT_HCP).toInt();
    hcp[0][HEARTS][NORTH_SEAT] = settings.value("hcpLowHeartsNorth", 0).toInt();
    hcp[1][HEARTS][NORTH_SEAT] = settings.value("hcpHighHeartsNorth", MAX_SUIT_HCP).toInt();
    hcp[0][HEARTS][EAST_SEAT] = settings.value("hcpLowHeartsEast", 0).toInt();
    hcp[1][HEARTS][EAST_SEAT] = settings.value("hcpHighHeartsEast", MAX_SUIT_HCP).toInt();
    hcp[0][HEARTS][SOUTH_SEAT] = settings.value("hcpLowHeartsSouth", 0).toInt();
    hcp[1][HEARTS][SOUTH_SEAT] = settings.value("hcpHighHeartsSouth", MAX_SUIT_HCP).toInt();

    hcp[0][SPADES][WEST_SEAT] = settings.value("hcpLowSpadesWest", 0).toInt();
    hcp[1][SPADES][WEST_SEAT] = settings.value("hcpHighSpadesWest", MAX_SUIT_HCP).toInt();
    hcp[0][SPADES][NORTH_SEAT] = settings.value("hcpLowSpadesNorth", 0).toInt();
    hcp[1][SPADES][NORTH_SEAT] = settings.value("hcpHighSpadesNorth", MAX_SUIT_HCP).toInt();
    hcp[0][SPADES][EAST_SEAT] = settings.value("hcpLowSpadesEast", 0).toInt();
    hcp[1][SPADES][EAST_SEAT] = settings.value("hcpHighSpadesEast", MAX_SUIT_HCP).toInt();
    hcp[0][SPADES][SOUTH_SEAT] = settings.value("hcpLowSpadesSouth", 0).toInt();
    hcp[1][SPADES][SOUTH_SEAT] = settings.value("hcpHighSpadesSouth", MAX_SUIT_HCP).toInt();

    hcp[0][NOTRUMP][WEST_SEAT] = settings.value("hcpLowTotalWest", 0).toInt();
    hcp[1][NOTRUMP][WEST_SEAT] = settings.value("hcpHighTotalWest", MAX_TOT_HCP).toInt();
    hcp[0][NOTRUMP][NORTH_SEAT] = settings.value("hcpLowTotalNorth", 0).toInt();
    hcp[1][NOTRUMP][NORTH_SEAT] = settings.value("hcpHighTotalNorth", MAX_TOT_HCP).toInt();
    hcp[0][NOTRUMP][EAST_SEAT] = settings.value("hcpLowTotalEast", 0).toInt();
    hcp[1][NOTRUMP][EAST_SEAT] = settings.value("hcpHighTotalEast", MAX_TOT_HCP).toInt();
    hcp[0][NOTRUMP][SOUTH_SEAT] = settings.value("hcpLowTotalSouth", 0).toInt();
    hcp[1][NOTRUMP][SOUTH_SEAT] = settings.value("hcpHighTotalSouth", MAX_TOT_HCP).toInt();

    //Distribution points.
    dp[0][WEST_SEAT] = settings.value("dpLowWest", 0).toInt();
    dp[1][WEST_SEAT] = settings.value("dpHighWest", MAX_TOT_DP).toInt();
    dp[0][NORTH_SEAT] = settings.value("dpLowNorth", 0).toInt();
    dp[1][NORTH_SEAT] = settings.value("dpHighNorth", MAX_TOT_DP).toInt();
    dp[0][EAST_SEAT] = settings.value("dpLowEast", 0).toInt();
    dp[1][EAST_SEAT] = settings.value("dpHighEast", MAX_TOT_DP).toInt();
    dp[0][SOUTH_SEAT] = settings.value("dpLowSouth", 0).toInt();
    dp[1][SOUTH_SEAT] = settings.value("dpHighSouth", MAX_TOT_DP).toInt();

    //Suit sizes.
    suitSize[0][CLUBS][WEST_SEAT] = settings.value("suitSizeLowClubsWest", 0).toInt();
    suitSize[1][CLUBS][WEST_SEAT] = settings.value("suitSizeHighClubsWest", MAX_SUIT_SIZE).toInt();
    suitSize[0][CLUBS][NORTH_SEAT] = settings.value("suitSizeLowClubsNorth", 0).toInt();
    suitSize[1][CLUBS][NORTH_SEAT] = settings.value("suitSizeHighClubsNorth", MAX_SUIT_SIZE).toInt();
    suitSize[0][CLUBS][EAST_SEAT] = settings.value("suitSizeLowClubsEast", 0).toInt();
    suitSize[1][CLUBS][EAST_SEAT] = settings.value("suitSizepHighClubsEast", MAX_SUIT_SIZE).toInt();
    suitSize[0][CLUBS][SOUTH_SEAT] = settings.value("suitSizeLowClubsSouth", 0).toInt();
    suitSize[1][CLUBS][SOUTH_SEAT] = settings.value("suitSizeHighClubsSouth", MAX_SUIT_SIZE).toInt();

    suitSize[0][DIAMONDS][WEST_SEAT] = settings.value("suitSizeLowDiamondsWest", 0).toInt();
    suitSize[1][DIAMONDS][WEST_SEAT] = settings.value("suitSizeHighDiamondsWest", MAX_SUIT_SIZE).toInt();
    suitSize[0][DIAMONDS][NORTH_SEAT] = settings.value("suitSizeLowDiamondsNorth", 0).toInt();
    suitSize[1][DIAMONDS][NORTH_SEAT] = settings.value("suitSizeHighDiamondsNorth", MAX_SUIT_SIZE).toInt();
    suitSize[0][DIAMONDS][EAST_SEAT] = settings.value("suitSizeLowDiamondsEast", 0).toInt();
    suitSize[1][DIAMONDS][EAST_SEAT] = settings.value("suitSizeHighDiamondsEast", MAX_SUIT_SIZE).toInt();
    suitSize[0][DIAMONDS][SOUTH_SEAT] = settings.value("suitSizeLowDiamondsSouth", 0).toInt();
    suitSize[1][DIAMONDS][SOUTH_SEAT] = settings.value("suitSizeHighDiamondsSouth", MAX_SUIT_SIZE).toInt();

    suitSize[0][HEARTS][WEST_SEAT] = settings.value("suitSizeLowHeartsWest", 0).toInt();
    suitSize[1][HEARTS][WEST_SEAT] = settings.value("suitSizeHighHeartsWest", MAX_SUIT_SIZE).toInt();
    suitSize[0][HEARTS][NORTH_SEAT] = settings.value("suitSizeLowHeartsNorth", 0).toInt();
    suitSize[1][HEARTS][NORTH_SEAT] = settings.value("suitSizeHighHeartsNorth", MAX_SUIT_SIZE).toInt();
    suitSize[0][HEARTS][EAST_SEAT] = settings.value("suitSizeLowHeartsEast", 0).toInt();
    suitSize[1][HEARTS][EAST_SEAT] = settings.value("suitSizeHighHeartsEast", MAX_SUIT_SIZE).toInt();
    suitSize[0][HEARTS][SOUTH_SEAT] = settings.value("suitSizeLowHeartsSouth", 0).toInt();
    suitSize[1][HEARTS][SOUTH_SEAT] = settings.value("suitSizeHighHeartsSouth", MAX_SUIT_SIZE).toInt();

    suitSize[0][SPADES][WEST_SEAT] = settings.value("suitSizeLowSpadesWest", 0).toInt();
    suitSize[1][SPADES][WEST_SEAT] = settings.value("suitSizeHighSpadesWest", MAX_SUIT_SIZE).toInt();
    suitSize[0][SPADES][NORTH_SEAT] = settings.value("suitSizeLowSpadesNorth", 0).toInt();
    suitSize[1][SPADES][NORTH_SEAT] = settings.value("suitSizeHighSpadesNorth", MAX_SUIT_SIZE).toInt();
    suitSize[0][SPADES][EAST_SEAT] = settings.value("suitSizeLowSpadesEast", 0).toInt();
    suitSize[1][SPADES][EAST_SEAT] = settings.value("suitSizeHighSpadesEast", MAX_SUIT_SIZE).toInt();
    suitSize[0][SPADES][SOUTH_SEAT] = settings.value("suitSizeLowSpadesSouth", 0).toInt();
    suitSize[1][SPADES][SOUTH_SEAT] = settings.value("suitSizeHighSpadesSouth", MAX_SUIT_SIZE).toInt();
}

void CDealOptionDoc::Initialize()
{
    //Min low linmit (zero for all).
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 4; j++)
            hcp[0][i][j] = 0;

    for (int i = 0; i < 4; i++)
        dp[0][i] = 0;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            suitSize[0][i][j] = 0;

    //Max high limit.
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            hcp[1][i][j] = MAX_SUIT_HCP;

    for (int j = 0; j < 4; j++)
        hcp[1][4][j] = MAX_TOT_HCP;

    for (int i = 0; i < 4; i++)
        dp[1][i] = MAX_TOT_DP;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            suitSize[1][i][j] = MAX_SUIT_SIZE;
}
