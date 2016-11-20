/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBiddingsystemdialog.cpp
  Developers: eak

  Revision History:
  4-jul-2016 eak: Original

  Abstract: Bid database editor.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the definition of the bid database editor.
 */

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QMessageBox>
#include <cassert>

#include "zbridgeexception.h"
#include "cbiddingsystemdialog.h"
#include "ui_cbiddingsystemdialog.h"
#include "cruledialog.h"

//Bid names (indexed by enum Bids).
const QString BID_NAMES[] =
{
    "P",
    "1C", "1D", "1H", "1S", "1N",
    "2C", "2D", "2H", "2S", "2N",
    "3C", "3D", "3H", "3S", "3N",
    "4C", "4D", "4H", "4S", "4N",
    "5C", "5D", "5H", "5S", "5N",
    "6C", "6D", "6H", "6S", "6N",
    "7C", "7D", "7H", "7S", "7N",
    "X", "XX", "?"
};

/**
 * @brief Construct and initialize bidding system dialog.
 * @param bidDB The bid database.
 * @param bidDesc Informative text description of pages and rules.
 */
CBiddingSystemDialog::CBiddingSystemDialog(CBidDB *bidDB, CBidDesc *bidDesc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBiddingSystemDialog)
{
    ui->setupUi(this);

    this->bidDB = bidDB;
    this->bidDesc = bidDesc;

    bidPageDesc = new CBidPageDescDialog(this);

    //Add pages  to dropdown (the first entry indicates no page selected).
    currentPage = -1;
    ui->pages->addItem("-");            //Also sets current page index to 0.
    QList<qint16> pages = bidDB->getPages();
    for (int i = 0; i < pages.size(); i++)
        ui->pages->addItem(QVariant(pages[i]).toString());
}

CBiddingSystemDialog::~CBiddingSystemDialog()
{
    delete ui;
}

/**
 * @brief Temporary.
 */
void CBiddingSystemDialog::on_readPage_clicked()
{
    //Constants for position/offset to bid info in lines.
    const int FIRST_POS = 7;        //Comment or bid sequence or bid.
    const int BID_SEQ_POS = 10;     //Bid sequence.
    const int LINK_SEQ_POS = 10;    //Link sequence.
    const int BID_POS = 7;          //Bid.
    const int PRIORITY_POS = 10;    //Priority.
    const int ALERT_POS = 12;       //Alert message number.
    const int TOTAL_POS = 16;       //Total points.
    const int VULNERABILITY_POS = 22;//Vulnerability.
    const int HCP_POS = 24;         //High card points.
    const int DP_PLAY_POS = 31;     //Distribution points or playing tricks.
    const int CONTROL_POS = 36;     //Controls.
    const int SUIT_POS = 40;        //Suits (Length, strength, top honor).
    const int SUIT_SIZE = 8;        //Size of each suit.
    const int SUIT_LENGTH = 0;      //Offset to suit length.
    const int SUIT_STR_TOP = 4;     //Offset to suit strength or top honor condition.
    const int STATUS_POS = 74;      //Status.
    const int SCORING_POS = 76;     //Scoring method.
    const int STOPPER_POS = 78;     //Stopper status.

    bool ok;
    int page = QInputDialog::getInt(this, tr("ZBridge - Create a new Page"),
                                 tr("Page id: "), 0, 0, 10000, 1, &ok);
    if (!ok)
        return;

    //Check that the page does not already exist.
    if (bidDB->pageExist(page))
    {
        QMessageBox::warning(this, tr("ZBridge"), tr("Cannot create alredy existing page"));
        return;
    }

    //Determine Bidding System Data File to read from.
    QString inFileName = QFileDialog::getOpenFileName(this,
        tr("Convert Bidding System Data File"), "", tr("All Files (*.*)"));
    QFile inFile(inFileName);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    //Determine if suit agreement should be calculated.
    bool detAgr = false;
    if (QMessageBox::question(this, tr("ZBridge"), tr("Determine suit agreement?")) == QMessageBox::Yes)
        detAgr = true;
    bool suitAgree[4];
    bool suitOpen[4];
    bool suitAnswer[4];
    bool suitOpenD[4];
    Suit suitLast;
    bool nextBidIsOpen = false;
    bool firstSeq = false;
    for (int i = 0; i < 4; i++)
        suitAgree[i] = suitOpen[i] = suitAnswer[i] = suitOpenD[i]=  false;

    //Symbol file.
    QStringList ruleIds;
    QFileInfo info(inFileName);
    QString symbolFileName(info.path() + "/symbol.smb");
    QFile symbolFile(symbolFileName);
    if (symbolFile.open(QIODevice::ReadOnly))
    {
        QDataStream in(&symbolFile);
        in >> ruleIds;
        symbolFile.close();
    }

    //Add page to bid database.
    bidDB->createPage(page);

    QTextStream in(&inFile);
    int lno = -1;

    try
    {
    QString pageDesc;
    CAuction auction;
    qint8 bid = -1;          //Bid not defined initially.
    bool splinter = false;
    bool stayman = false;
    bool jacoby = false;
    bool negativeDouble = false;
    while (!in.atEnd())
    {
        QString line;
        line = in.readLine(); lno++;

        if ((lno == 0) && (line.compare("splinter", Qt::CaseInsensitive) == 0))
        {
            splinter = true;
            continue;
        }
        else if ((lno == 0) && (line.compare("stayman", Qt::CaseInsensitive) == 0))
        {
            stayman = true;
            continue;
        }
        else if ((lno == 0) && (line.compare("jacoby", Qt::CaseInsensitive) == 0))
        {
            jacoby = true;
            continue;
        }
        else if ((lno == 0) && (line.compare("negative double", Qt::CaseInsensitive) == 0))
        {
            negativeDouble = true;
            continue;
        }

        int tabStop = 8;
        for (int i = 0; i < line.size(); i++)
        {
            if (i >= tabStop)
                tabStop +=8;
            if (line[i] == '\t')
            {
                QString txt(tabStop - i, ' ');
                line = line.replace(i, 1, txt);
            }
        }
        if (line.size() < 80)
        {
            int size = line.size();
            QString txt(80 - size, ' ');
            line.append(txt);
        }

        //Comment?
        if (line[FIRST_POS] == '*')
        {
            pageDesc += line +'\n';
        }

        //Bidding sequence?
        else if (line.indexOf("## ", 0, Qt::CaseInsensitive) == FIRST_POS)
        {
            QString text = line.mid(BID_SEQ_POS);
            text = text.remove(" ");
            if (!textToAuction(&auction, text))
                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                            tr(" illegal auction: ") + text).toStdString());
//            if (bidDB->auctionExist(page, auction))
//                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
//                                            tr(" auction ") + text + tr("already exist")).toStdString());

            bid = -1;          //Bid not defined initially.
            suitLast = ANY;

            //Determine splinter bids.
            //Only determined in splinter auction files. Next bid splinter is
            //handled manually.
            CAuction auctionT = auction;
            int size = auctionT.auction.size();
            if (splinter)
            {
                for (int i = 0; i < (size - 2); i +=2)
                {
                    qint8 bidFirst = auctionT.auction[i];
                    qint8 bidNext = auctionT.auction[i + 2];
                    Suit suitFirst = BID_SUIT(bidFirst);
                    Suit suitNext = BID_SUIT(bidNext);
                    if ((suitFirst != NOTRUMP) && (suitNext != NOTRUMP))
                    {
                        if (suitFirst == suitNext)
                            break;
                        //Splinter (double jump shift)?
                        if (((bidNext - bidFirst) > 10) && ((bidNext - bidFirst) < 15))
                        {
                            auctionT.auction[i + 2] = bidFirst + 10;
                            detAgr = true;
                            break;
                        }
                    }
                }
            }
            //Determine Stayman bids.
            else if (stayman && (size >= 4))
            {
                if (auctionT.auction[0] == BID_1NT)
                {
                    if ((size >= 6) &&
                            (auctionT.auction[2] == BID_2C) &&
                            (auction.auction[4] == BID_2D))
                    {
                        auctionT.auction[2] = BID_NONE;
                        auctionT.auction[4] = BID_NONE;
                    }
                    else if ((size >= 6) &&
                             (auctionT.auction[2] == BID_2C) &&
                             (auction.auction[4] == BID_2S))
                        auctionT.auction[2] = BID_NONE;
                    else if ((size >= 8) &&
                             (auctionT.auction[2] == BID_2C) &&
                             (auction.auction[4] == BID_2H) &&
                             (auction.auction[6] == BID_2NT))
                        auctionT.auction[2] = BID_2S;
                    else if ((size >= 6) &&
                             (auctionT.auction[2] == BID_2C) &&
                             (auction.auction[4] == BID_2H))
                        auctionT.auction[2] = BID_NONE;
                    else if ((size >= 4) &&
                             (auctionT.auction[2] == BID_2C))
                        auctionT.auction[2] = BID_NONE;
                }
                else if ((auctionT.auction[0] == BID_2NT) ||
                         ((size >= 6) && (auctionT.auction[0] == BID_2C) &&
                          (auctionT.auction[2] == BID_2D) &&
                          (auctionT.auction[4] == BID_2NT)))

                {
                    int first = 0;
                    if (auctionT.auction[0] == BID_2C)
                    {
                        first = 4;
                        auctionT.auction[0] = BID_NONE;
                        auctionT.auction[2] = BID_NONE;
                    }
                    if (((size - first) >= 6) &&
                            (auctionT.auction[2 + first] == BID_3C) &&
                            (auction.auction[4 + first] == BID_3D))
                    {
                        auctionT.auction[2 + first] = BID_NONE;
                        auctionT.auction[4 + first] = BID_NONE;
                    }
                    else if (((size - first) >= 6) &&
                            (auctionT.auction[2 + first] == BID_3C) &&
                            (auction.auction[4 + first] == BID_3S))
                        auctionT.auction[2 + first] = BID_NONE;
                    else if (((size - first) >= 8) &&
                            (auctionT.auction[2 + first] == BID_3C) &&
                            (auction.auction[4 + first] == BID_3H) &&
                            (auction.auction[6 + first] == BID_3NT))
                        auctionT.auction[2 + first] = BID_3S;
                    else if (((size - first) >= 6) &&
                            (auctionT.auction[2 + first] == BID_3C) &&
                            (auction.auction[4 + first] == BID_3H))
                        auctionT.auction[2 + first] = BID_NONE;
                    else if (((size - first) >= 4) &&
                            (auctionT.auction[2 + first] == BID_3C))
                        auctionT.auction[2 + first] = BID_NONE;
                }

                detAgr = true;
            }


            //Determine Jacoby bids.
            else if (jacoby && (size >= 2) && (auctionT.auction[0] == BID_1NT))
            {
                qint8 fA = auction.auction[2];
                if ((size >= 8) &&
                        (fA == BID_2D) && (auction.auction[4] == BID_2H) && (auction.auction[6] == BID_2S))
                {
                    auctionT.auction[2] = BID_2H;
                    auctionT.auction[4] = BID_NONE;
                }
                else if ((size >= 8) &&
                        (fA == BID_2H) && (auction.auction[4] == BID_2S) && (auction.auction[6] == BID_3H))
                {
                    auctionT.auction[4] = BID_NONE;
                    auctionT.auction[6] = BID_2S;
                }
                else if ((size >= 6) &&
                        (fA == BID_2D) && (auction.auction[4] == BID_3H))
                    auctionT.auction[2] = BID_2H;
                else if ((size >= 6) &&
                         (fA == BID_2H) && (auction.auction[4] == BID_3S))
                    auctionT.auction[2] = BID_2S;
                else if ((size >= 4) &&
                         ((fA == BID_2D) || (fA == BID_2H) || (fA == BID_2S) || (fA == BID_2NT)))
                {
                    if (size >= 6)
                        auctionT.auction[4] = BID_NONE;

                    if (fA == BID_2D)
                        auctionT.auction[2] = BID_2H;
                    else if (fA == BID_2H)
                        auctionT.auction[2] = BID_2S;
                    else if (fA == BID_2S)
                        auctionT.auction[2] = BID_3C;
                    else
                        auctionT.auction[2] = BID_3D;
                }

                detAgr = true;
            }

            //Determine negative double bids.
            else if (negativeDouble && (size >= 3) && (auctionT.auction[2] == BID_DOUBLE))
            {
                qint8 bidO = auctionT.auction[0];
                qint8 bidD = auctionT.auction[1];
                if (((bidO == BID_1C) && (bidD == BID_1D)) ||
                    ((bidO == BID_1D) && (bidD == BID_2C)))
                    auctionT.auction[2] = BID_2H;   //Should be both majors (Spade handled manually).
                else if (((bidO == BID_1H) && (bidD == BID_1S)) ||
                    ((bidO == BID_1S) && (bidD == BID_2H)))
                    auctionT.auction[2] = BID_3C;   //Should be both minors (Diamond handled manually).
                else if (((bidO == BID_1C) && (bidD == BID_1H)) ||
                    ((bidO == BID_1C) && (bidD == BID_2H)) ||
                    ((bidO == BID_1D) && (bidD == BID_1H)) ||
                    ((bidO == BID_1D) && (bidD == BID_2H)) ||
                    ((bidO == BID_1H) && (bidD == BID_2C)) ||
                    ((bidO == BID_1H) && (bidD == BID_2D)))
                    auctionT.auction[2] = BID_2S;
                else if (((bidO == BID_1C) && (bidD == BID_1S)) ||
                    ((bidO == BID_1C) && (bidD == BID_2S)) ||
                    ((bidO == BID_1D) && (bidD == BID_1S)) ||
                    ((bidO == BID_1D) && (bidD == BID_2S)) ||
                    ((bidO == BID_1S) && (bidD == BID_2C)) ||
                    ((bidO == BID_1S) && (bidD == BID_2D)))
                    auctionT.auction[2] = BID_2H;

                detAgr = true;
            }

            //Determine color agreement.
            if (detAgr)
            {
                for (int i = 0; i < 4; i++)
                    suitAgree[i] = suitOpen[i] = suitAnswer[i] = suitOpenD[i] = false;

                int size = auctionT.auction.size();
                int first = size % 2;
                firstSeq = (first == 0);

                int firstSuit = -1;
                for (int i = first; i <= size; i += 2)
                {
                    //Determine bids from opponent (to identify cue bids).
                    if (i > 0)
                    {
                        Suit suitD = BID_SUIT(auctionT.auction[i - 1]);
                        if ((suitD >= 0) && (suitD < 4))
                            suitOpenD[suitD] = true;
                    }

                    //The last bid is an opponent bid.
                    if (i < size)
                    {
                        Suit suit = BID_SUIT(auctionT.auction[i]);
                        int level = BID_LEVEL(auctionT.auction[i]);
                        if ((suit >= 0) && (suit < 5))
                        {
                            if (firstSuit == -1)
                            {
                                firstSuit = i;
                                nextBidIsOpen = (((size - i) % 4) == 0);
                            }

                            //Skip if cuebid (Michaels cuebid has been dealt with).
                            if ((suit == 4) || !suitOpenD[suit])
                            {

                                //New color on level 4 before a trump fit is found?
                                if ((suit >= 0) && (suit < 4) &&
                                        (level == 4) && (i != first) && firstSeq)
                                {
                                    Suit suitPrev = BID_SUIT(auctionT.auction[i - 2]);
                                    if ((suitPrev >= 0) && (suitPrev < 4) && !suitOpen[suit] && !suitAnswer[suit])
                                    {
                                        //Use previous color as trump fit.
                                        suitOpen[suitPrev] = suitAnswer[suitPrev] = true;
                                        suitAgree[suitPrev] = true;
                                        break;
                                    }
                                }

                                //Open bidder or answer bidder.
                                if (suit != NOTRUMP)
                                {
                                    if (((i - firstSuit) % 4) == 0)
                                        suitOpen[suit] = true;
                                    else
                                        suitAnswer[suit] = true;
                                    if (suitOpen[suit] && suitAnswer[suit])
                                    {
                                        suitAgree[suit] = true;
                                        break;
                                    }
                                }
                                else if (level == 3)
                                    for (int j = 0; j < 4; j++)
                                        suitOpen[j] = suitAnswer[j] = false;
                            }
                            else
                            {
                                //Cue bid is only for the first.
                                suitOpenD[suit] = false;
                                suit = ANY;
                            }
                        }

                        //To be used for the case new color on 4 level in next bid.
                        suitLast = suit;
                    }
                }
            }

            //Add auction to bid database (does nothing if auction already exist).
            bidDB->createAuction(page, auction);
        }

        //Link?
        else if (line.indexOf(" > ", 0, Qt::CaseInsensitive) == FIRST_POS)
        {
            if (!bidDB->auctionExist(page, auction))
                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                            tr(" auction to link from does not exist")).toStdString());

            QString text = line.mid(LINK_SEQ_POS);
            text = text.remove(" ");
            CAuction substituteAuction;
            if (!textToAuction(&substituteAuction, text))
                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                            tr(" illegal substitute auction: ") + text).toStdString());
            if(bidDB->isSubstituteAuction(page, auction) || bidDB->isBids(page, auction))
                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                            tr(" auction to link from is already in use")).toStdString());
            bidDB->createSubstituteAuction(page, auction, substituteAuction);
        }

        //Basic rule.
        else
        {
            //Check for open auction.
            if ((auction.auction.size() == 0) && (!bidDB->auctionExist(page, auction)))
                bidDB->createAuction(page, auction);

            //Bid
            QString text = line.mid(FIRST_POS, 2);
            text = text.trimmed();
            if ((text.size() > 0) && (!textToBid(&bid, text) || !checkBid(bid,auction)))
                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                            tr(" illegal bid: ") + text).toStdString());
            if ((text.size() == 0) && (bid == -1))
                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                            tr(" no bid is defined")).toStdString());

            //Priority (gets value 0 if not defined).
            int priority;
            text = line.mid(PRIORITY_POS, 1);
            priority = text.toInt();
            if (priority > 7)
                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                            tr(" illegal priority: ") + QString::number(priority)).toStdString());

            //Alert message number (gets value 0 if not defined).
            int alertNo;
            text = line.mid(ALERT_POS, 3);
            alertNo = text.toInt();
            if ((alertNo < 0) || (alertNo > 255))
                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                            tr(" illegal alert message number: ") + QString::number(alertNo)).toStdString());

            CFeatures lowFeatures;
            CFeatures highFeatures;
            lowFeatures.setMinFeatures();
            highFeatures.setMaxFeatures();

            //Total point.
            if (line[TOTAL_POS + 2] == '-')
            {
                text = line.mid(TOTAL_POS, 2).trimmed();
                int lowTotal = (text.size() > 0) ? text.toInt() : 0;
                text = line.mid(TOTAL_POS + 3 ,2).trimmed();
                int highTotal = (text.size() > 0) ? text.toInt() : highFeatures.getMaxPoints();

                if ((lowTotal > highTotal) || (highTotal >  highFeatures.getMaxPoints()))
                    throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                                tr(" illegal total point interval")).toStdString());

                Suit suit = suitAgreed(suitAgree, suitOpen, suitAnswer, nextBidIsOpen, firstSeq,
                                       suitOpenD, suitLast, bid);
                lowFeatures.setPoints(suit, lowTotal);
                highFeatures.setPoints(suit, highTotal);
            }

            //Vulnerability condition.
            Vulnerability vulnerability;
            text = line.mid(VULNERABILITY_POS, 2).trimmed();
            if (text.size() == 0)
                vulnerability = VUL_II;
            else if (text == "N")
                vulnerability = VUL_NI;
            else if (text == "V")
                vulnerability = VUL_YI;
            else if (text == "NN")
                vulnerability = VUL_NN;
            else if (text == "NV")
                vulnerability = VUL_NY;
            else if (text == "VN")
                vulnerability = VUL_YN;
            else if (text == "VV")
                vulnerability = VUL_YY;
            else
                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                            tr(" illegal vulnerability")).toStdString());

            //HCP.
            if ((line[HCP_POS + 2] == '+') || (line[HCP_POS + 2] == '-'))
            {
                text = line.mid(HCP_POS, 2).trimmed();
                int lowHcp = (text.size() > 0) ? text.toInt() : 0;
                text = line.mid(HCP_POS + 3, 2).trimmed();
                int highHcp = (text.size() > 0) ? text.toInt() : highFeatures.getMaxHcp(ANY);

                if ((lowHcp > highHcp) || (highHcp >  highFeatures.getMaxHcp(ANY)))
                    throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                               tr(" illegal HCP interval")).toStdString());

                lowFeatures.setHcp(ANY, lowHcp);
                highFeatures.setHcp(ANY, highHcp);
            }

            //Distribution points?
            if ((line[DP_PLAY_POS] == ' ') && (line[DP_PLAY_POS + 2] == '-'))
            {
                text = line.mid(DP_PLAY_POS + 1, 1).trimmed();
                int lowDp = (text.size() > 0) ? text.toInt() : 0;
                text = line.mid(DP_PLAY_POS + 3, 1).trimmed();
                int highDp = (text.size() > 0) ? text.toInt() : 9;

                if ((lowDp > highDp) || (highDp >  9))
                    throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                                tr(" illegal DP interval")).toStdString());

                Suit suit = suitAgreed(suitAgree, suitOpen, suitAnswer, nextBidIsOpen, firstSeq,
                                       suitOpenD, suitLast, bid);
                lowFeatures.setDp(suit, lowDp);
                highFeatures.setDp(suit, highDp);
            }

            //Playing tricks?
            else if ((line[DP_PLAY_POS] == 'P') && ((line[DP_PLAY_POS + 2] == '+') || (line[DP_PLAY_POS + 2] == '-')))
            {
                text = line.mid(DP_PLAY_POS + 1, 1).trimmed();
                int lowPlayingTricks = (text.size() > 0) ? text.toInt(&ok, 16) : 0;
                text = line.mid(DP_PLAY_POS + 3, 1).trimmed();
                int highPlayingTricks = (text.size() > 0) ? text.toInt(&ok, 16) : highFeatures.getMaxPlayingTricks();

                if ((lowPlayingTricks > highPlayingTricks) || (highPlayingTricks >  highFeatures.getMaxPlayingTricks()))
                    throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                                tr(" illegal playing trick interval interval")).toStdString());

                Suit suit = suitAgreed(suitAgree, suitOpen, suitAnswer, nextBidIsOpen, firstSeq,
                                       suitOpenD, suitLast, bid);
                lowFeatures.setPlayingTricks(suit, lowPlayingTricks);
                highFeatures.setPlayingTricks(suit, highPlayingTricks);
            }

            //Controls.
            if (line[CONTROL_POS + 1] == '-')
            {
                text = line.mid(CONTROL_POS, 1).trimmed();
                int lowControls = (text.size() > 0) ? text.toInt(&ok, 16) : 0;
                text = line.mid(CONTROL_POS + 2, 1).trimmed();
                int highControls = (text.size() > 0) ? text.toInt(&ok, 16) : highFeatures.getMaxControls(ANY);

                if ((lowControls > highControls) || (highControls >  highFeatures.getMaxControls(ANY)))
                    throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                                tr(" illegal controls interval")).toStdString());

                lowFeatures.setControls(ANY, lowControls);
                highFeatures.setControls(ANY, highControls);
            }

            //Suits
            for (int suit = 0; suit < 4; suit++)
            {
                int suitPos = SUIT_POS + suit * SUIT_SIZE;

                //Length.
                if (line[suitPos + SUIT_LENGTH + 1] == '-')
                {
                    QChar character = line[suitPos + SUIT_LENGTH];
                    if (character.isDigit())
                        lowFeatures.setSuitLen((Suit)suit, character.digitValue());
                    else if ((character == 'c') && (suit != CLUBS))
                        lowFeatures.setDif2((Suit)suit, CLUBS, 1);
                    else if ((character == 'd') && (suit != DIAMONDS))
                        lowFeatures.setDif2((Suit)suit, DIAMONDS, 1);
                    else if ((character == 'h') && (suit != HEARTS))
                        lowFeatures.setDif2((Suit)suit, HEARTS, 1);
                    else if ((character == 's') && (suit != SPADES))
                        lowFeatures.setDif2((Suit)suit, SPADES, 1);
                    else if ((character == 'C') && (suit != CLUBS))
                        lowFeatures.setDif2((Suit)suit, CLUBS, 0);
                    else if ((character == 'D') && (suit != DIAMONDS))
                        lowFeatures.setDif2((Suit)suit, DIAMONDS, 0);
                    else if ((character == 'H') && (suit != HEARTS))
                        lowFeatures.setDif2((Suit)suit, HEARTS, 0);
                    else if ((character == 'S') && (suit != SPADES))
                        lowFeatures.setDif2((Suit)suit, SPADES, 0);
                    else if ((character == 'L') && (suit != CLUBS))
                        lowFeatures.setDif2((Suit)suit, CLUBS, -1);
                    else if ((character == 'I') && (suit != DIAMONDS))
                        lowFeatures.setDif2((Suit)suit, DIAMONDS, -1);
                    else if ((character == 'E') && (suit != HEARTS))
                        lowFeatures.setDif2((Suit)suit, HEARTS, -1);
                    else if ((character == 'P') && (suit != SPADES))
                        lowFeatures.setDif2((Suit)suit, SPADES, -1);
                    else if (character != ' ')
                        throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                                    tr(" illegal suit length combinations")).toStdString());

                    character = line[suitPos + SUIT_LENGTH + 2];
                    if (character.isDigit())
                        highFeatures.setSuitLen((Suit)suit, character.digitValue());
                    else if ((character == 'c') && (suit != CLUBS))
                        highFeatures.setDif2((Suit)suit, CLUBS,- 1);
                    else if ((character == 'd') && (suit != DIAMONDS))
                        highFeatures.setDif2((Suit)suit, DIAMONDS,- 1);
                    else if ((character == 'h') && (suit != HEARTS))
                        highFeatures.setDif2((Suit)suit, HEARTS, -1);
                    else if ((character == 's') && (suit != SPADES))
                        highFeatures.setDif2((Suit)suit, SPADES, -1);
                    else if ((character == 'C') && (suit != CLUBS))
                        highFeatures.setDif2((Suit)suit, CLUBS, 0);
                    else if ((character == 'D') && (suit != DIAMONDS))
                        highFeatures.setDif2((Suit)suit, DIAMONDS, 0);
                    else if ((character == 'H') && (suit != HEARTS))
                        highFeatures.setDif2((Suit)suit, HEARTS, 0);
                    else if ((character == 'S') && (suit != SPADES))
                        highFeatures.setDif2((Suit)suit, SPADES, 0);
                    else if ((character == 'L') && (suit != CLUBS))
                        highFeatures.setDif2((Suit)suit, CLUBS, 1);
                    else if ((character == 'I') && (suit != DIAMONDS))
                        highFeatures.setDif2((Suit)suit, DIAMONDS, 1);
                    else if ((character == 'E') && (suit != HEARTS))
                        highFeatures.setDif2((Suit)suit, HEARTS, 1);
                    else if ((character == 'P') && (suit != SPADES))
                        highFeatures.setDif2((Suit)suit, SPADES, 1);
                    else if (character != ' ')
                        throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                                    tr(" illegal suit length combinations")).toStdString());
                }

                //Top honor condition.
                QString txt = line.mid(suitPos + SUIT_STR_TOP, 3);
                if (txt.contains('A') || txt.contains('K') || txt.contains('Q') || txt.contains('-'))
                {
                    if (txt.contains('A'))
                        lowFeatures.setCountCard((Suit)suit, ACE, 1);
                    if (txt.contains('K'))
                        lowFeatures.setCountCard((Suit)suit, KING, 1);
                    if (txt.contains('Q'))
                        lowFeatures.setCountCard((Suit)suit, QUEEN, 1);

                    if (((txt[0] == '-') && (lowFeatures.getCountCard((Suit)suit, ACE) == 1)) ||
                         ((txt[1] == '-') && (lowFeatures.getCountCard((Suit)suit, KING) == 1)) ||
                          ((txt[2] == '-') && (lowFeatures.getCountCard((Suit)suit, QUEEN) == 1)))
                        throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                                    tr(" illegal club top honor combinations")).toStdString());
                    if (txt[0] == '-')
                        highFeatures.setCountCard((Suit)suit, ACE, 0);
                    if (txt[1] == '-')
                        highFeatures.setCountCard((Suit)suit, KING, 0);
                    if (txt[2] == '-')
                        highFeatures.setCountCard((Suit)suit, QUEEN, 0);
                }

                //Strength condition.
                else if ((txt[0] != ' ') && (txt[0] != '0'))
                {
                    bool upper = (txt[1] == '>');
                    if (txt[0] == '1')
                    {
                        if (upper)
                            highFeatures.setQlty((Suit)suit, 1);
                        else
                            lowFeatures.setQlty((Suit)suit, 1);
                    }
                    else if (txt[0] == '2')
                    {
                        if (upper)
                            highFeatures.setQlty((Suit)suit, 2);
                        else
                            lowFeatures.setQlty((Suit)suit, 2);
                    }
                    else if (txt[0] == '3')
                    {
                        if (upper)
                            highFeatures.setQlty((Suit)suit, 3);
                        else
                            lowFeatures.setQlty((Suit)suit, 3);
                    }
                    else if (txt[0] == '4')
                    {
                        if (upper)
                            highFeatures.setQlty((Suit)suit, 4);
                        else
                            lowFeatures.setQlty((Suit)suit, 4);
                    }
                    else if (txt[0] == '5')
                    {
                        if (upper)
                            highFeatures.setQlty((Suit)suit, 5);
                        else
                            lowFeatures.setQlty((Suit)suit, 5);
                    }
                    else if (txt[0] == '6')
                    {
                        if (upper)
                            highFeatures.setQlty((Suit)suit, 6);
                        else
                            lowFeatures.setQlty((Suit)suit, 6);
                    }
                    else if (txt[0] == '7')
                    {
                        lowFeatures.setCountCard((Suit)suit, ACE, 1);
                    }
                    else
                        throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                                    tr(" illegal club suit strength")).toStdString());
                }
            }

            //Status.
            Forcing status;
            if (line[STATUS_POS] == ' ')
                status = NON_FORCING;
            else if (line[STATUS_POS] == 'F')
                status = FORCING;
            else if (line[STATUS_POS] == 'G')
                status = GAME_FORCING;
            else if (line[STATUS_POS] == 'P')
                status = MUST_PASS;
            else
                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                            tr(" illegal status")).toStdString());

            //Scoring method.
            ScoringMethod scoringMethod;
            if (line[SCORING_POS] == ' ')
                    scoringMethod = NOSCORE;
            else if (line[SCORING_POS] == 'I')
                scoringMethod = IMP;
            else if (line[SCORING_POS] == 'M')
                scoringMethod = MP;
            else
                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                            tr(" illegal scoring method")).toStdString());

            //Stoppers for NT.
            if (line[STOPPER_POS] == '1')
            {
                lowFeatures.setStopNT(SPADES, 3);
                lowFeatures.setStopNT(HEARTS, 3);
                lowFeatures.setStopNT(DIAMONDS, 3);
                lowFeatures.setStopNT(CLUBS, 3);
            }
            else if ((line[STOPPER_POS] != ' ') && (line[STOPPER_POS] != '0'))
                throw PlayException(QString(inFileName + tr("  at line: ") + QString::number(lno) +
                                            tr(" illegal scoring method")).toStdString());

            //Create bid and update rule.
            CRule * rule = bidDB->createBid(page, auction, bid);
            QString ruleId = line.left(7);
            ruleId = ruleId.trimmed();
            int id = 0;
            if (ruleId.size() != 0)
            {
                id = ruleIds.indexOf(ruleId) + 1;
                if (id == 0)
                {
                    ruleIds.append(ruleId);
                    id = ruleIds.size();
                }
            }
            rule->setId(id);                 //Must be edited afterwards.
            rule->setPriority(priority);
            rule->setAlertId(alertNo);
            rule->setVulnerability(vulnerability);
            rule->setStatus(status);
            rule->setScoringMethod(scoringMethod);
            rule->setFeatures(lowFeatures, highFeatures);
        }
    }

    //Set page description.
    bidDesc->setPageDesc(page, pageDesc);

    //Add page to page drop down and select it as current selected page.
    ui->pages->addItem(QVariant(page).toString());
    ui->pages->setCurrentIndex(ui->pages->count() - 1);
    }
    catch (PlayException &e)
    {
        QMessageBox::critical(this, tr("ZBridge"), e.what());
        bidDB->deletePage(page);
    }

    //Symbol file.
    if (symbolFile.open(QIODevice::WriteOnly))
    {
        QDataStream out(&symbolFile);
        out << ruleIds;
        symbolFile.close();
    }

    inFile.close();
}

/**
 * @brief Accept corrections to the bid database.
 */
void CBiddingSystemDialog::on_buttonBox_accepted()
{    
    //Update description.
    if (currentPage >= 0)
    {
        QString txt = bidPageDesc->getPageDesc();
        bidDesc->setPageDesc(currentPage, txt);
    }
/*
    //Determine Alert File to read from.
    QString alertFileName = QFileDialog::getOpenFileName(this,
        tr("Alert File"), "", tr("All Files (*.*)"));
    QFile alertFile(alertFileName);
    alertFile.open(QIODevice::ReadOnly | QIODevice::Text);

    quint8 alertId = 0;
    QString alertTxt;
    while (!alertFile.atEnd())
    {
        QString line = alertFile.readLine();
        QString text = line.mid(7, 3);
        if (text.toInt() != alertId)
        {
            bidDesc->setAlertIdDesc(alertId, alertTxt);
            alertTxt = line.mid(13).trimmed() + '\n';
            alertId = text.toInt();
        }
        else
            alertTxt += QString(line.mid(13).trimmed()) + '\n';
    }
*/
    accept();
}

/**
 * @brief Do not accept corrections to the bid database.
 */
void CBiddingSystemDialog::on_buttonBox_rejected()
{
    reject();
}

/**
 * @brief Another page was selected in the page drop down.
 * @param index Index of the new page.
 */
void CBiddingSystemDialog::on_pages_currentIndexChanged(int index)
{
    //Clear auction, substitute link or next bid / rules.
    ui->auctions->clear();              //Clear auction drop down.
    ui->link->clear();                  //Clear substitute link.
    ui->linkGroup->setEnabled(false);   //No substitute link is defined.
    ui->rules->clear();                 //Clear next bid / rules dropdown.
    ui->rulesGroup->setEnabled(false);  //No next bid / rules are defined.

    //Update description.
    if (currentPage >= 0)
    {
        QString txt = bidPageDesc->getPageDesc();
        bidDesc->setPageDesc(currentPage, txt);
    }

    //Is first entry of page drop down selected?
    if (index <= 0)
    {
        //Current page is no page (not possible to delete page and work on auctions).
        ui->deletePage->setEnabled(false);      //Disable delete page.
        ui->auctionsGroup->setEnabled(false);   //Disable auction group.

        //Clear and hide page description.
        QString txt = "";
        bidPageDesc->setPageDesc(txt);
        bidPageDesc->hide();
        currentPage = -1;
    }
    else
    {
        //Possible to delete page and work on auctions.
        ui->deletePage->setEnabled(true);       //Enable delete page.
        ui->auctionsGroup->setEnabled(true);    //Enable auctions group.

        qint16 page = ui->pages->currentText().toInt();

        QString txt = bidDesc->getPageDesc(page);
        bidPageDesc->setPageDesc(txt);
        bidPageDesc->show();
        currentPage = page;

        //First entry in auction drop down is none selected.
        ui->auctions->addItem("-");         //Also sets current auction index to 0.

        //Add auctions to auction drop down
        QList<CAuction> auctions = bidDB->getAuctions(page);
        for (int i = 0; i < auctions.size(); i++)
        {
            QString text;
            auctionToText(auctions[i], &text);
            ui->auctions->addItem(text);
        }
    }
}

/**
 * @brief Create a new page.
 */
void CBiddingSystemDialog::on_createPage_clicked()
{
    bool ok;
    int page = QInputDialog::getInt(this, tr("ZBridge - Create a new Page"),
                                 tr("Page id: "), 0, 0, 10000, 1, &ok);
    if (ok)
    {
        //Check that the page does not already exist.
        if (bidDB->pageExist(page))
        {
            QMessageBox::warning(this, tr("ZBridge"), tr("Cannot create alredy existing page"));
            return;
        }
        //Add page to bid database.
        bidDB->createPage(page);

        //Add page to page drop down and select it as current selected page.
        ui->pages->addItem(QVariant(page).toString());
        ui->pages->setCurrentIndex(ui->pages->count() - 1);
    }
}

/**
 * @brief Delete the current page (and connected auctions etc.).
 */
void CBiddingSystemDialog::on_deletePage_clicked()
{
    if (QMessageBox::question(this, tr("ZBridge"), tr("Do you want to delete the current page")) == QMessageBox::Yes)
    {
        qint16 page = ui->pages->currentText().toInt();

        //Clear page from bid database.
        bidDB->deletePage(page);
        QString txt = "";
        bidDesc->setPageDesc(page, txt);
        currentPage = -1;

        //Remove page from page drop down and set current page as first entry (i.e. none)
        ui->pages->removeItem(ui->pages->currentIndex());
        ui->pages->setCurrentIndex(0);
    }
}

/**
 * @brief Another auction was selected in the auctions drop down.
 * @param index Index of the new auction.
 */
void CBiddingSystemDialog::on_auctions_currentIndexChanged(int index)
{
    //Clear link and rules drop down.
    ui->link->clear();
    ui->rules->clear();
    ui->linkGroup->setEnabled(false);
    ui->rulesGroup->setEnabled(false);

    //Is first entry of auction drop down selected?
    if (index <= 0)
    {
        //Current auction is no auction.
        ui->deleteAuction->setEnabled(false);   //Not possible to delete auction.
    }
    else
    {
        ui->deleteAuction->setEnabled(true);    //Auction can be deleted.

        //Get page and auction.
        qint16 page = ui->pages->currentText().toInt();
        QString text = ui->auctions->currentText();
        CAuction auction;
        textToAuction(&auction, text);

        //Link or Rule not defined yet?
        if (!bidDB->isBids(page, auction) && !bidDB->isSubstituteAuction(page, auction))
        {
            ui->linkGroup->setEnabled(true);    //Substitute link can be created.
            ui->createLink->setEnabled(true);   //Substitute link can be created.
            ui->deleteLink->setEnabled(false);  //No substitute link to delete.
            ui->rulesGroup->setEnabled(true);   //Next bid / rule can be created.
            ui->createRule->setEnabled(true);   //Next bid / rule can be created.
            ui->deleteRule->setEnabled(false);  //No next bid / rule to delete.
        }

        //Link group?
        else if (bidDB->isSubstituteAuction(page, auction))
        {
            ui->linkGroup->setEnabled(true);    //Substitute link exist.
            ui->deleteLink->setEnabled(true);   //Substitute link can be deleted.
            ui->createLink->setEnabled(false);  //Substitute link cannot be created.
            CAuction substituteAuction = bidDB->getSubstituteAuction(page, auction);
            QString text;
            auctionToText(substituteAuction, &text);
            ui->link->setText(text);
        }

        //Rule group.
        else if (bidDB->isBids(page, auction))
        {
            ui->rulesGroup->setEnabled(true);   //Next bids exist.
            ui->createRule->setEnabled(true);   //Next bid can be created.

            //First entry in rules drop down is none selected.
            ui->rules->addItem("-");

            //Add next bid / rules to rules drop down.
            QList<qint8> bids;
            QList<CRule*> pRules;
            bidDB->getBids(page, auction, &bids, &pRules);
            for (int i = 0; i < bids.size(); i++)
            {
                qint8 bid = bids[i];
                if (!(IS_BID(bid) || IS_DOUBLE_BID(bid) || IS_REDOUBLE_BID(bid) || (bid == BID_PASS)))
                    bid = BID_PLAYER;
                ui->rules->addItem(BID_NAMES[bid], VPtr<CRule>::asQVariant(pRules[i]));
            }
            //Select current rule as the first entry in rules drop down (i.e. none).
            ui->rules->setCurrentIndex(0);

            //Activate current rule.
            on_rules_activated(ui->rules->currentIndex());
        }
    }
}

/**
 * @brief Create a new auction.
 */
void CBiddingSystemDialog::on_createAuction_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("ZBridge - Create an auction (bidding sequence)"),
                                 tr("Auction: "),  QLineEdit::Normal, "", &ok);
    if (ok)
    {
        //Get the auction (bidding sequence).
        CAuction auction;
        if (!textToAuction(&auction, text))
        {
            QMessageBox::warning(this, tr("ZBridge"), tr("Syntax error in auction"));
            return;
        }

        qint16 page = ui->pages->currentText().toInt();
        if (bidDB->auctionExist(page, auction))
        {
            QMessageBox::warning(this, tr("ZBridge"), tr("Auction does already exist"));
            return;
        }

        //Add auction to bid database.
        bidDB->createAuction(page, auction);

        //Add auction to auction drop down and select it as the current auction.
        ui->auctions->addItem(text);
        ui->auctions->setCurrentIndex(ui->auctions->count() - 1);
    }
}

/**
 * @brief Delete the current auction (and connected bids etc.).
 */
void CBiddingSystemDialog::on_deleteAuction_clicked()
{
    if (QMessageBox::question(this, tr("ZBridge"), tr("Do you want to delete the current auction")) == QMessageBox::Yes)
    {
        qint16 page = ui->pages->currentText().toInt();
        CAuction auction;
        QString text = ui->auctions->currentText();
        textToAuction(&auction, text);

        //Delete auction from bid database.
        bidDB->deleteAuction(page, auction);

        on_pages_currentIndexChanged(ui->pages->currentIndex());
    }
}

/**
 * @brief Create a new substitute link (auction).
 */
void CBiddingSystemDialog::on_createLink_clicked()
{
    bool ok;
    QString substituteText = QInputDialog::getText(this, tr("ZBridge - Create a substitute auction (bidding sequence)"),
                                 tr("Substitute auction: "),  QLineEdit::Normal, "", &ok);
    if (ok)
    {
        //Get the substitute auction.
        CAuction substituteAuction;
        if (!textToAuction(&substituteAuction, substituteText))
        {
            QMessageBox::warning(this, tr("ZBridge"), tr("Syntax error in substitute auction (bidding sequence)"));
            return;
        }

        //Get auction.
        QString text = ui->auctions->currentText();
        CAuction auction;
        if (!textToAuction(&auction, text))
            assert (false);

        //Get page.
        qint16 page = ui->pages->currentText().toInt();

        //Create substitute auction in bid database.
        bidDB->createSubstituteAuction(page, auction, substituteAuction);

        //Show the substitute auction.
        ui->link->setText(substituteText);
                
        //Enable/disable properly.
        ui->rulesGroup->setEnabled(false);      //Not possible to work on rules (substitute link exist).
        ui->createLink->setEnabled(false);      //Not possible to create substitute link (does already exist).
        ui->deleteLink->setEnabled(true);       //Substitute link can be deleted.
    }
}

/**
 * @brief Delete the current substitute link (auction).
 */
void CBiddingSystemDialog::on_deleteLink_clicked()
{    
    if (QMessageBox::question(this, tr("ZBridge"), tr("Do you want to delete the substitute link?")) == QMessageBox::Yes)
    {
        //Get page.
        qint16 page = ui->pages->currentText().toInt();

        //Get auction.
        CAuction auction;
        QString text = ui->auctions->currentText();
        textToAuction(&auction, text);

        //Delete substitute link from bid database.
        bidDB->deleteSubstituteAuction(page, auction);

        //Clear from link group.
        ui->link->clear();

        //Enable/disable properly.
        ui->rulesGroup->setEnabled(true);       //Possible to work on next bid / rules.
        ui->createRule->setEnabled(true);       //Possible to create next bid / rule.
        ui->deleteRule->setEnabled(false);      //Not possible to delete next bid / rule (does not exist).
        ui->createLink->setEnabled(true);       //Possible to create a substitute link.
        ui->deleteLink->setEnabled(false);      //Not possible to delete substitute link (does not exist).
    }
}

/**
 * @brief Another bid was selected in the rules drop down.
 * @param index Index of the new bid.
 */
void CBiddingSystemDialog::on_rules_activated(int index)
{
    //Is first entry of rules drop down selected?
    if (index <= 0)
    {
        ui->deleteRule->setEnabled(false);      //Cannot delete next bid / rule (none is selected).
    }
    else
    {
        ui->deleteRule->setEnabled(true);       //Can delete next bid / rule.

        //Page.
        qint16 page = ui->pages->currentText().toInt();

        //Auction.
        QString text = ui->auctions->currentText();
        CAuction auction;
        textToAuction(&auction, text);

        //Bid.
        QString bidText = ui->rules->currentText();
        qint8 bid;
        textToBid(&bid, bidText);

        //pRule.
        QVariant variant = ui->rules->itemData(index);
        CRule *pRuleId = VPtr<CRule>::asPtr(variant);

        //Get rule.
        CRule *pRule = bidDB->getpRule(page, auction, bid, pRuleId);
        CRule rule = *pRule;

        //Rule dialog.
        CRuleDialog ruleDialog(&rule, bidDesc, bidText);

        //Accept corrections to rule?
        if (ruleDialog.exec() == QDialog::Accepted)
            *pRule = rule;
    }
}

/**
 * @brief Create a new next bid.
 */
void CBiddingSystemDialog::on_createRule_clicked()
{
    bool ok;
    QString bidText = QInputDialog::getText(this, tr("ZBridge - Create next bid"),
                                 tr("Next bid: "),  QLineEdit::Normal, "", &ok);
    if (ok)
    {
        //Get the bid.
        qint8 bid;
        if (!textToBid(&bid, bidText))
        {
            QMessageBox::warning(this, tr("ZBridge"), tr("Syntax error in next bid"));
            return;
        }

        //Get auction.
        QString text = ui->auctions->currentText();
        CAuction auction;
        textToAuction(&auction, text);

        //Check the bid.
        if (!checkBid(bid, auction))
        {
            QMessageBox::warning(this, tr("ZBridge"), tr("Illegal next bid"));
            return;
        }

        //Create the rule and add to bid database.
        qint16 page = ui->pages->currentText().toInt();
        if (!bidDB->isBids(page, auction))
            ui->rules->addItem("-");
        CRule *pRule = bidDB->createBid(page, auction, bid);

        //Add to next bid/rule dropdown and set as current selected..
        ui->rules->addItem(bidText, VPtr<CRule>::asQVariant(pRule));
        ui->rules->setCurrentIndex(ui->rules->count() - 1);

        //Enable/disable properly.
        ui->linkGroup->setEnabled(false);       //Cannot work on link group.

        //Activate current rule.
        on_rules_activated(ui->rules->currentIndex());
    }
}

/**
 * @brief Delete the current next bid.
 */
void CBiddingSystemDialog::on_deleteRule_clicked()
{
    if (QMessageBox::question(this, tr("ZBridge"), tr("Do you want to delete the current next bid?")) == QMessageBox::Yes)
    {
        //Page.
        qint16 page = ui->pages->currentText().toInt();

        //Auction.
        QString text = ui->auctions->currentText();
        CAuction auction;
        textToAuction(&auction, text);

        //Bid.
        QString bidText = ui->rules->currentText();
        qint8 bid;
        textToBid(&bid, bidText);

        //pRule.
        int index = ui->rules->currentIndex();
        QVariant variant = ui->rules->itemData(index);
        CRule *pRule = VPtr<CRule>::asPtr(variant);

        //Delete next bid/rule from bid database.
        bidDB->deleteBid(page, auction, bid, pRule);

        //Remove from nextbid/rule dropdown.
        if (!bidDB->isBids(page, auction))
        {
            //Clear rule drop down (only none selected entry left).
            ui->rules->clear();

            //Enable/disable properly (prepare  for choice of substitute link or next bid / rule).
            ui->deleteRule->setEnabled(false);      //Cannot delete next bid / rule (none exist).
            ui->linkGroup->setEnabled(true);        //Can work on link group.
            ui->createLink->setEnabled(true);       //Can create subnstitute link.
            ui->deleteLink->setEnabled(false);      //Cannot delete substitute link (does not exist).
        }
        else
        {
            //Remove next bid / rule from rules drop down And set current selected to none.
            ui->rules->removeItem(ui->rules->currentIndex());
            ui->rules->setCurrentIndex(0);

            //Activate current rule.
            on_rules_activated(ui->rules->currentIndex());
        }
    }
}

//Convert auction to text (to display).
void CBiddingSystemDialog::auctionToText(CAuction &auction, QString *text)
{
    *text = "";

    for (int i = 0; i < auction.auction.size(); i++)
    {
        qint8 bid = auction.auction[i];
        if (!(IS_BID(bid) || (bid == BID_PASS) || IS_DOUBLE_BID(bid) || IS_REDOUBLE_BID(bid)))
            bid = 38;

        if (i != 0)
            *text += "-";
        *text += BID_NAMES[bid];
    }
}

//Convert text to auction (and check that the auction is a possible bid sequence).
bool CBiddingSystemDialog::textToAuction(CAuction *auction, QString &text)
{
    //Convert the text to an auction and check the auction sequence.
    QStringList bidNames;
    QStringList bidTxts;

    auction->auction.clear();
    text = text.trimmed();
    if (text.size() == 0)
        return true;

    for (int i = 0; i <= BID_REDOUBLE; i++)
        bidNames << BID_NAMES[i];

    bidTxts = text.split('-');
    for (int i = 0; i < bidTxts.size(); i++)
    {
        int bid = bidNames.indexOf(bidTxts[i]);
        if ((bid == -1) || (bid > BID_REDOUBLE))
        {
            auction->auction.clear();
            return false;
        }
        auction->auction.append(bid);
    }

    //Check bidding sequence.
    Bids lastProperBid = BID_NONE;
    Bids bidEnableDouble = BID_NONE;
    bool firstPass = true;
    bool noPass = 0;
    for (int i = 0; i < auction->auction.size(); i++)
    {
        if (auction->auction.at(i) == BID_PASS)
        {
            noPass++;
            if ((firstPass && (noPass > 3)) || (!firstPass && (noPass > 2)))
            {
                auction->auction.clear();
                return false;
            }
        }
        else if (IS_BID(auction->auction.at(i)))
        {
            Bids bid = (Bids)auction->auction.at(i);
            if (bid <= lastProperBid)
            {
                auction->auction.clear();
                return false;
            }
            noPass = 0;
            firstPass = false;
            lastProperBid = (Bids)auction->auction.at(i);
            bidEnableDouble = BID_DOUBLE;
        }
        else if ((IS_DOUBLE_BID(auction->auction.at(i))) && (bidEnableDouble == BID_DOUBLE))
        {
            noPass = 0;
            bidEnableDouble = BID_REDOUBLE;
        }
        else if ((IS_REDOUBLE_BID(auction->auction.at(i))) && (bidEnableDouble == BID_REDOUBLE))
        {
            noPass = 0;
            bidEnableDouble = BID_NONE;
        }
        else
        {
            auction->auction.clear();
            return false;
        }
    }
    return true;
}

//Convert a bid to text (to display).
void CBiddingSystemDialog::bidToText(qint8 bid, QString *text)
{
    if (!(IS_BID(bid) || (bid == BID_PASS) || IS_DOUBLE_BID(bid) || IS_REDOUBLE_BID(bid)))
        bid = 38;

    *text = BID_NAMES[bid];
}

//Convert text to bid (and check that it is a bid).
bool CBiddingSystemDialog::textToBid(qint8 *bid, QString &text)
{
    qint8 b;
    QList<QString> bidNames;
    for (int i = 0; i <=BID_REDOUBLE; i++)
        bidNames.append(BID_NAMES[i]);

    if ((b = bidNames.indexOf(text)) == -1)
    return (false);

    *bid = b;
    return true;
}

//Check that a bid is possible following the auction.
bool CBiddingSystemDialog::checkBid(qint8 bid, CAuction &auction)
{
    //Check that the bid is a proper bid i.e can follow auction.
    Bids lastProperBid, bidEnableDouble;

    //Next bid.
    lastProperBid = bidEnableDouble = BID_NONE;
    for (int i = 0; i < auction.auction.size(); i++)
    {
        if (IS_BID(auction.auction[i]))
        {
            lastProperBid = (Bids)auction.auction[i];
            bidEnableDouble = BID_DOUBLE;
        }
        else if (IS_DOUBLE_BID(auction.auction[i]))
            bidEnableDouble = BID_REDOUBLE;
        else if (IS_REDOUBLE_BID(auction.auction[i]))
            bidEnableDouble = BID_NONE;
    }

    return (((bidEnableDouble == BID_DOUBLE) && (bid == BID_DOUBLE)) ||
            ((bidEnableDouble == BID_REDOUBLE) && (bid == BID_REDOUBLE)) ||
            ((bid > lastProperBid) && (bid <= BID_7NT)) ||
            (bid == BID_PASS));
}

//Temporary.
Suit CBiddingSystemDialog::suitAgreed(bool suitAgree[], bool suitOpen[], bool suitAnswer[],
                                      bool nextBidIsOpen, bool firstSeq, bool suitOpenD[],
                                      Suit suitLast, qint8 bid)
{
    Suit suit = BID_SUIT(bid);
    int level = BID_LEVEL(bid);

    //Try for nt?
    if (suit == NOTRUMP)
        return ANY;

    //After a minor suit trump fit has been found, a new suit bid on the 3 level normally shows
    //stoppers in the suit and is an attempt to try for nt.
    if (suitAgree[CLUBS])
    {
        if ((suit == CLUBS) || (suit == ANY) || (level > 3))
            return CLUBS;
        else
            return ANY;
    }

    if (suitAgree[DIAMONDS])
    {
        if ((suit == DIAMONDS) || (suit == ANY) || (level > 3))
            return DIAMONDS;
        else
            return ANY;
    }

    //After a major suit trump fit has been found, try for the suit.
    if (suitAgree[HEARTS])
        return HEARTS;

    if (suitAgree[SPADES])
        return SPADES;

    //No trump fit before the next bid.
    //Cuebid?
    if (suitOpenD[suit])
        return ANY;

    if ((suitLast < 0) || (suitLast > 3))
        suitLast = ANY;
    if (nextBidIsOpen)
    {
        //Check for trump fit with answer.
        if ((suit == CLUBS) && suitAnswer[CLUBS])
            return CLUBS;
        else if ((suit == DIAMONDS) && suitAnswer[DIAMONDS])
            return DIAMONDS;
        else if ((suit == HEARTS) && suitAnswer[HEARTS])
            return HEARTS;
        else if ((suit == SPADES) && suitAnswer[SPADES])
            return SPADES;
        else if (firstSeq && (level == 4) && !suitOpen[suit])
            return suitLast;
        else
            return ANY;
    }
    else
    {
        //Check for trump fit with open.
        if ((suit == CLUBS) && suitOpen[CLUBS])
            return CLUBS;
        else if ((suit == DIAMONDS) && suitOpen[DIAMONDS])
            return DIAMONDS;
        else if ((suit == HEARTS) && suitOpen[HEARTS])
            return HEARTS;
        else if ((suit == SPADES) && suitOpen[SPADES])
            return SPADES;
        else if (firstSeq && (level == 4) && !suitAnswer[suit])
            return suitLast;
        else
            return ANY;
    }
}
