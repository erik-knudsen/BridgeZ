/*Erik Aagaard Knudsen.
  Copyright © 2013 - All Rights Reserved

  Project: ZBridge
  File: CZBridgeDoc.cpp
  Developers: eak

  Revision History:
  26-feb-2013 eak: Original

  Abstract: ZBridge model (data etc.).

  Platforms: Qt.

*/

/**
 * @file
 * ZBridge model (data etc.).
 */

#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QSettings>

#include "czbridgedoc.h"
#include "ZBridgeException.h"

CZBridgeDoc *CZBridgeDoc::instance = 0;


/**
 * Constructor for the model container.
 * Reads data from permanent storage.
 */
CZBridgeDoc::CZBridgeDoc(QObject *parent) :
    QObject(parent)
{
    ReadSeatOptions();
    ReadSettings();
    LoadBidOptions();
    ReadDealOptions();
    ReadDisplayOptions();
    ReadGameOptions();
    ReadBidDBFileName();

    LoadBidDB();
}

/**
 * This static method returns a pointer to the (singleton) data container.
 */
CZBridgeDoc* CZBridgeDoc::Instance()
{
    return instance;
}

/**
 * @brief Get current North/South bid options.
 *
 * @return Current North/South bid options.
 */
CBidOptionDoc &CZBridgeDoc::getNSBidOptions()
{
    if (bidOptions.empty())
        return defaultBidOption;

    int i;

    for (i = 0; i < bidOptions.size(); i++)
        if (QString::compare(bidOptions[i].name, curBidOption[NSBIDOPTIONSINDEX]) == 0)
            break;

    if (i == bidOptions.size())
        i = 0;

    return bidOptions[i];
}

/**
 * @brief Get Current East/West bid options.
 *
 * @return Current East/West bid options.
 */
CBidOptionDoc &CZBridgeDoc::getEWBidOptions()
{
    if (bidOptions.empty())
        return defaultBidOption;

    int i;

    for (i = 0; i < bidOptions.size(); i++)
        if (QString::compare(bidOptions[i].name, curBidOption[EWBIDOPTIONSINDEX]) == 0)
            break;

    if (i == bidOptions.size())
        i = 0;

    return bidOptions[i];
}

/**
 * @brief Write seat options to permanent storage.
 */
void CZBridgeDoc::WriteSeatOptions()
{
    seatOptions.WriteSettings();
}

/**
 * @brief Read seat options from permanent storage.
 */
void CZBridgeDoc::ReadSeatOptions()
{
    seatOptions.ReadSettings();
}

/**
 * @brief Write current convention settings to permanent storage.
 */
void CZBridgeDoc::WriteSettings()
{
    QSettings settings("ZBridge settings", "Conventions");

    settings.setValue("nsConvention", curBidOption[NSBIDOPTIONSINDEX]);
    settings.setValue("ewConvention", curBidOption[EWBIDOPTIONSINDEX]);
}

/**
 * @brief Read current convention settings from permanent storage.
 */
void CZBridgeDoc::ReadSettings()
{
    QSettings settings("ZBridge settings", "Conventions");

    curBidOption[NSBIDOPTIONSINDEX] = settings.value("nsConvention","").toString();
    curBidOption[EWBIDOPTIONSINDEX] = settings.value("ewConvention","").toString();
}

/**
 * @brief Write bid options to permanent storage.
 */
void CZBridgeDoc::SaveBidOptions()
{
    QFile file(BIDOPTIONFILENAME);
    file.open(QIODevice::WriteOnly);
    if (bidOptions.size() == 0)
        file.remove();
    else
    {
        QDataStream out(&file);
        out << bidOptions;
        if (out.status() != QDataStream::Ok)
            QMessageBox::warning(0, tr("ZBridge"), tr("Bid options could not be saved."));
        file.close();
    }
}

/**
 * @brief Read bid options from permanent storage.
 */
void CZBridgeDoc::LoadBidOptions()
{
    QFile file;
    file.setFileName(BIDOPTIONFILENAME);
    if (!file.exists())
        file.setFileName(QString(":/") + BIDOPTIONFILENAME);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> bidOptions;
    if (in.status() != QDataStream::Ok)
        QMessageBox::warning(0, tr("ZBridge"), tr("Bid options could not be loaded.\n"
                                                  "Defaults will be used."));
    file.close();
}

/**
 * @brief Write deal options to permanent storage
 */
void CZBridgeDoc::WriteDealOptions()
{
    dealOptions.WriteSettings();
}

/**
 * @brief Read deal options from permanent storage
 */
void CZBridgeDoc::ReadDealOptions()
{
    dealOptions.ReadSettings();
}

/**
 * @brief Write display options to permanent storage
 */
void CZBridgeDoc::WriteDisplayOptions()
{
    displayOptions.WriteSettings();
}

/**
 * @brief Read display options from permanent storage
 */
void CZBridgeDoc::ReadDisplayOptions()
{
    displayOptions.ReadSettings();
}

/**
 * @brief Write game options to permanent storage
 */
void CZBridgeDoc::WriteGameOptions()
{
    gameOptions.WriteSettings();
}

/**
 * @brief Read game options from permanent storage
 */
void CZBridgeDoc::ReadGameOptions()
{
    gameOptions.ReadSettings();
}

/**
 * @brief Read bid database file name from permanent storage.
 */
void CZBridgeDoc::WriteBidDBFileName()
{
    QSettings settings("ZBridge settings", "div");

    settings.setValue("bidDBFileName", bidDBFileName);
}

/**
 * @brief Write bid database file name to permanent storage.
 */
void CZBridgeDoc::ReadBidDBFileName()
{
    QSettings settings("ZBridge settings", "div");

    bidDBFileName = settings.value("bidDBFileName",":/biddb.bsf").toString();
}

/**
 * @brief Write bid database to permanent storage.
 */
void CZBridgeDoc::SaveBidDB()
{
    try
    {
        QFile file(bidDBFileName);
        if (file.open(QIODevice::WriteOnly))
        {
            QDataStream out(&file);
            out << bidDB;
            file.close();
            if (out.status() != QDataStream::Ok)
                throw PlayException(tr("Bid DB: Write of bid DB failed").toStdString());
        }
        QString descFilename = bidDBFileName.left(bidDBFileName.indexOf(".bsf", 0, Qt::CaseInsensitive)) + ".dsc";
        QFile descFile(descFilename);
        if (descFile.open(QIODevice::WriteOnly))
        {
            QDataStream out(&descFile);
            out << bidDesc;
            descFile.close();
            if (out.status() != QDataStream::Ok)
                throw PlayException(tr("Bid DB: Write of bid description failed").toStdString());
        }
    }
    catch (PlayException &e)
    {
        QMessageBox::critical(0, tr("ZBridge"), e.what());
    }
}

/**
 * @brief Read bid database from permanent storage.
 */
void CZBridgeDoc::LoadBidDB()
{
    if (bidDBFileName.size() == 0)
        return;

    try
    {
        bidDB.clearBidDB();
        bidDesc.clearBidDesc();

        QFile file(bidDBFileName);
        if (file.open(QIODevice::ReadOnly))
        {
            QDataStream in(&file);
            in >> bidDB;
            file.close();
            if (in.status() != QDataStream::Ok)
                throw PlayException(tr("Bid DB: Corrupt or version not supported or not a bid database").toStdString());
        }

        //Open file with text description of pages etc. in the bid database.
        QString descFilename = bidDBFileName.left(bidDBFileName.indexOf(".bsf", 0, Qt::CaseInsensitive)) + ".dsc";
        QFile descFile(descFilename);
        if (descFile.open(QIODevice::ReadOnly))
        {
            QDataStream in(&descFile);
            in >> bidDesc;
            descFile.close();
            if (in.status() != QDataStream::Ok)
                throw PlayException(tr("Bid DB: Corrupt or version not supported or not a bid description").toStdString());
        }
    }
    catch (PlayException &e)
    {
        bidDB.clearBidDB();
        bidDesc.clearBidDesc();

        QMessageBox::critical(0, tr("ZBridge"), e.what());
    }
}

/**
 * @brief CZBridgeDoc::setFileComments
 * @param fileComments
 */
void CZBridgeDoc::setFileComments(QString &fileComments)
{
}

/**
 * @brief CZBridgeDoc::getFileComments
 * @return
 */
QString &CZBridgeDoc::getFileComments()
{
    QString *txt = new QString("text");
    return *txt;
}

/**
 * @brief CZBridgeDoc::setShowCommentsUponOpen
 * @param autoShow
 */
void CZBridgeDoc::setShowCommentsUponOpen(bool autoShow)
{
}

/**
 * @brief CZBridgeDoc::getShowCommentsUponOpen
 * @return
 */
bool CZBridgeDoc::getShowCommentsUponOpen()
{
    return false;
}
