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
 * \file
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
#include "cwizardoptiondoc.h"

class CZBridgeDoc : public QObject
{
    Q_OBJECT
public:
    static CZBridgeDoc* Instance();
    explicit CZBridgeDoc(QObject *parent = 0);


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
    CWizardOptionDoc &getWizardOptions() { return wizardOptions; }
    void setWizardOptions(CWizardOptionDoc &wizardOptions) { this->wizardOptions = wizardOptions; }
    void synchronizeOptions(bool onlyDefaultBid);
    CBidOptionDoc &getNSBidOptions();
    CBidOptionDoc &getEWBidOptions();

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
    void WriteWizardOptions();
    void ReadWizardOptions();

    void setFileComments(QString& fileComments);
    QString& getFileComments();
    void setShowCommentsUponOpen(bool autoShow);
    bool getShowCommentsUponOpen();

signals:
    
public slots:

private:
    static CZBridgeDoc* instance;

    CSeatOptionDoc seatOptions;
    CBidOptionDoc defaultBidOption;
    QList<CBidOptionDoc> bidOptions;
    QString curBidOption[2];
    CDealOptionDoc dealOptions;
    CDisplayOptionDoc displayOptions;
    CGameOptionDoc gameOptions;
    CWizardOptionDoc wizardOptions;
};

#endif // CZBRIDGEDOC_H
