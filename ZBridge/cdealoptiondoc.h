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
 * This class has the deal options and methods to save and restore the options. There is
 * also a method for checking the deal options to assure that there exist deals, which fulfills
 * the limitiations imposed by the deal options. And there is a method for getting a deal, which
 * fulfills the limitations.
 */
class CDealOptionDoc
{
public:
    CDealOptionDoc();

    void WriteSettings();
    void ReadSettings();
    void Initialize();
    int checkDealOptions(int noDeals);
    void getDeal(int cards[][13]);

    //High Card Points. Indexed by [low/high, suit, seat].
    int hcp[2][5][4];

    //Distribution points. Indexed by [low/high, seat].
    int dp[2][4];

    //Suit size. Indexed by [low/high, suit, seat].
    int suitSize[2][4][4];

private:
    int deal(int cards[][13], int noDeals);
};

#endif // CDEALOPTIONDOC_H
