/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CBiddingsystemdialog.h
  Developers: eak

  Revision History:
  4-jul-2016 eak: Original

  Abstract: Bid database editor.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the bid database editor.
 */

#ifndef CBIDDINGSYSTEMDIALOG_H
#define CBIDDINGSYSTEMDIALOG_H

#include <QVariant>
#include <QDialog>

#include "cbiddb.h"
#include "cbiddesc.h"
#include "cbidpagedescdialog.h"

namespace Ui {
class CBiddingSystemDialog;
}

/**
 * @brief Bid database editor.
 *
 * The dialog implements an editor for creating and editing the bid database. It consists of 4
 * groups with similar functionalities. The first group works on pages, the first level of the
 * bid database. It allows to create, delete and select an entry in the first level (page) of the Bid
 * database. The second group works on auctions, the second level of the bid database. It allows
 * to create, delete and select an entry in the second level (page, auction) of the bid database. The
 * third group works on substitute auctions in the third level of the bid database. It allows to create,
 * delete and edit a substitute bid entry in the third level (page, auction, substitute auction) of
 * the bid database. The fourth group works on next bid / rules in the third level of the bid database.
 * It allows to create, delete and edit a next bid / rule in the third level (page, auction, next bid /rule)
 * of the bid database.
 */
class CBiddingSystemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBiddingSystemDialog(CBidDB *bidDB, CBidDesc *bidDesc, QWidget *parent = 0);
    ~CBiddingSystemDialog();

private slots:    
    void on_readPage_clicked();

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    /** @name Page handling.
     */
    /*@{*/
    void on_pages_currentIndexChanged(int index);
    void on_createPage_clicked();
    void on_deletePage_clicked();
    /*@}*/

    /** @name Auction handling.
     */
    /*@{*/
    void on_auctions_currentIndexChanged(int index);
    void on_createAuction_clicked();
    void on_deleteAuction_clicked();
    /*@}*/

    /** @name Substitute link handling.
     */
    /*@{*/
    void on_createLink_clicked();
    void on_deleteLink_clicked();
    /*@}*/

    /** @name Rule handling.
     */
    /*@{*/
    void on_rules_activated(int index);
    void on_createRule_clicked();
    void on_deleteRule_clicked();
    /*@}*/

private:
    void auctionToText(CAuction &auction, QString *text);   /**< Convert auction to text. */
    bool textToAuction(CAuction *auction, QString &text);   /**< Convert text to auction. */
    void bidToText(qint8 bid, QString *text);               /**< Convert bid to text. */
    bool textToBid(qint8 *bid, QString &text);              /**< Convert text to bid. */
    bool checkBid(qint8 bid, CAuction &auction);            /**< Check if bid is ok. */

    Suit suitAgreed(bool suitAgree[], bool suitOpen[], bool suitAnswer[], bool nextBidIsOpen,
                    bool firstSeq, bool suitOpenD[], Suit suitLast, qint8 bid);

    Ui::CBiddingSystemDialog *ui;
    CBidPageDescDialog *bidPageDesc;

    CBidDB *bidDB;
    CBidDesc *bidDesc;
    int currentPage;
};

//Cast QVariant to pointer and cast pointer to QVariant.
template <class T> class VPtr
{
public:
    static T* asPtr(QVariant v)
    {
    return  (T *) v.value<void *>();
    }

    static QVariant asQVariant(T* ptr)
    {
    return qVariantFromValue((void *) ptr);
    }
};
#endif // CBIDDINGSYSTEMDIALOG_H
