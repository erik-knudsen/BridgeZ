/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CZBridgeDoc.h
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: ZBridge model (data etc.).

  Platforms: Qt.

*/

/**
 * @file
 * ZBridge model (data etc.) (declaration).
 */

#ifndef CZBRIDGEDOC_H
#define CZBRIDGEDOC_H

#include <QObject>
#include <QTcpSocket>

#include "Defines.h"
#include "cseatoptiondoc.h"
#include "cbidoptiondoc.h"
#include "cdealoptiondoc.h"
#include "cdisplayoptiondoc.h"
#include "cgameoptiondoc.h"
#include "cbiddb.h"
#include "cbiddesc.h"


/**
 * @brief Container for configuration parameters.
 *
 * The application is partly controlled by configuration parameters. These parameters
 * can be changed by the user. This is done via the main frame menu.
 */
class CZBridgeDoc : public QObject
{
    Q_OBJECT
public:
    //The container is a singleton and can be referenced anywhere.
    static CZBridgeDoc* Instance();

    explicit CZBridgeDoc(QObject *parent = 0);


    //Get and set options.
    CSeatOptionDoc &getSeatOptions() { return seatOptions; }
    void setSeatOptions(CSeatOptionDoc &seatOptions) { this->seatOptions = seatOptions; }
    CBidOptionDoc &getDefaultBidOption() { return defaultBidOption; }
    QList<CBidOptionDoc> *getBidOptions() { return &bidOptions;}
    QString* getCurBidOption() { return curBidOption; }
    CDealOptionDoc &getDealOptions() { return dealOptions; }
    void setDealOptions(CDealOptionDoc &dealOptions) { this->dealOptions = dealOptions; }
    CDisplayOptionDoc &getDisplayOptions() { return displayOptions; }
    void setDisplayOptions(CDisplayOptionDoc &displayOptions) { this->displayOptions = displayOptions; }
    CGameOptionDoc &getGameOptions() { return gameOptions; }
    void setGameOptions(CGameOptionDoc &gameOptions) { this->gameOptions = gameOptions; }
    CBidOptionDoc &getNSBidOptions();
    CBidOptionDoc &getEWBidOptions();
    QString getBidDBFileName() { return bidDBFileName; }
    void setBidDBFileName(QString &bidDBFileName) { this->bidDBFileName = bidDBFileName; }

    CBidDB *getBidDB() { return &bidDB; }
    CBidDesc *getBidDesc() {return &bidDesc; }

    //Read and write options (write and read to system storage, save and load to file storage).
    void WriteSeatOptions();
    void ReadSeatOptions();
    void SaveBidOptions();
    void LoadBidOptions();
    void WriteSettings();
    void ReadSettings();
    void WriteDealOptions();
    void ReadDealOptions();
    void WriteDisplayOptions();
    void ReadDisplayOptions();
    void WriteGameOptions();
    void ReadGameOptions();
    void WriteBidDBFileName();
    void ReadBidDBFileName();

    //Bid database.
    void SaveBidDB();
    void LoadBidDB();

    //Other.
    void setFileComments(QString& fileComments);
    QString& getFileComments();
    void setShowCommentsUponOpen(bool autoShow);
    bool getShowCommentsUponOpen();

signals:
    
public slots:

private:
    static CZBridgeDoc* instance;   //Reference to singleton instance.

    //Option variables.
    CSeatOptionDoc seatOptions;
    CBidOptionDoc defaultBidOption;
    QList<CBidOptionDoc> bidOptions;
    QString curBidOption[2];
    CDealOptionDoc dealOptions;
    CDisplayOptionDoc displayOptions;
    CGameOptionDoc gameOptions;

    //Bid database.
    QString bidDBFileName;
    CBidDB bidDB;
    CBidDesc bidDesc;
};

#endif // CZBRIDGEDOC_H
