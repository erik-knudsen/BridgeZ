/* Erik Aagaard Knudsen.
  Copyright © 2016 - All Rights Reserved

  Project: ZBridge
  File: CRuleDialog.h
  Developers: eak

  Revision History:
  4-jul-2016 eak: Original

  Abstract: Rule dialog.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements the declaration of the rule dialog (part of the bid database editor).
 */

#ifndef CRULEDIALOG_H
#define CRULEDIALOG_H

#include <QDialog>
#include <QLineEdit>

#include "cauction.h"
#include "crule.h"
#include "cbiddesc.h"

namespace Ui {
class CRuleDialog;
}

/**
 * @brief Edit/create upper and lower limit features for a given rule.
 */
class CRuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRuleDialog(CRule *rule, CBidDesc *bidDesc, QString &bid, QWidget *parent = 0);
    ~CRuleDialog();

private slots:
    /** @name Id, priority, forcing, scoring method, vulnerability, alert id.
     */
    /*@{*/
    void on_ruleId_editingFinished();
    void on_rulePriority_valueChanged(int arg1);
    void on_ruleForcing_activated(int index);
    void on_ruleScoringMethod_activated(int index);
    void on_ruleVulnerability_activated(int index);
    void on_ruleAlertId_editingFinished();
    /*@}*/

    /** @name High card points.
     */
    /*@{*/
    void on_hcpS_editingFinished();
    void on_hcpH_editingFinished();
    void on_hcpD_editingFinished();
    void on_hcpC_editingFinished();
    void on_hcpA_editingFinished();
    /*@}*/

    /** @name Weighed  count of aces and kings.
     */
    /*@{*/
    void on_controlsS_editingFinished();
    void on_controlsH_editingFinished();
    void on_controlsD_editingFinished();
    void on_controlsC_editingFinished();
    void on_controlsA_editingFinished();
    /*@}*/

    /** @name Count of aces, kings and queens.
     */
    /*@{*/
    void on_hcS_editingFinished();
    void on_hcH_editingFinished();
    void on_hcD_editingFinished();
    void on_hcC_editingFinished();
    void on_hcA_editingFinished();
    /*@}*/

    /** @name Count of cards higher than 9.
     */
    /*@{*/
    void on_honorS_editingFinished();
    void on_honorH_editingFinished();
    void on_honorD_editingFinished();
    void on_honorC_editingFinished();
    void on_honorA_editingFinished();
    /*@}*/

    /** @name Count of aces, kings, queens, jacks and tens.
     */
    /*@{*/
    void on_aceS_editingFinished();
    void on_aceH_editingFinished();
    void on_aceD_editingFinished();
    void on_aceC_editingFinished();
    void on_aceA_editingFinished();

    void on_kingS_editingFinished();
    void on_kingH_editingFinished();
    void on_kingD_editingFinished();
    void on_kingC_editingFinished();
    void on_kingA_editingFinished();

    void on_queenS_editingFinished();
    void on_queenH_editingFinished();
    void on_queenD_editingFinished();
    void on_queenC_editingFinished();
    void on_queenA_editingFinished();

    void on_jackS_editingFinished();
    void on_jackH_editingFinished();
    void on_jackD_editingFinished();
    void on_jackC_editingFinished();
    void on_jackA_editingFinished();

    void on_tenS_editingFinished();
    void on_tenH_editingFinished();
    void on_tenD_editingFinished();
    void on_tenC_editingFinished();
    void on_tenA_editingFinished();
    /*@}*/

    /** @name Count of aces in hand + count of kings in suit.
     */
    /*@{*/
    void on_rkcbS_editingFinished();
    void on_rkcbH_editingFinished();
    void on_rkcbD_editingFinished();
    void on_rkcbC_editingFinished();
    /*@}*/

    /** @name Weighed count of intermediate cards.
     */
    /*@{*/
    void on_interS_editingFinished();
    void on_interH_editingFinished();
    void on_interD_editingFinished();
    void on_interC_editingFinished();
    void on_interA_editingFinished();
    /*@}*/

    /** @name Expected number of loosers.
     */
    /*@{*/
    void on_loserS_editingFinished();
    void on_loserH_editingFinished();
    void on_loserD_editingFinished();
    void on_loserC_editingFinished();
    void on_loserA_editingFinished();
    /*@}*/

    /** @name Quick tricks.
     */
    /*@{*/
    void on_quickS_editingFinished();
    void on_quickH_editingFinished();
    void on_quickD_editingFinished();
    void on_quickC_editingFinished();
    void on_quickA_editingFinished();
    /*@}*/

    /** @name Playing tricks.
     */
    /*@{*/
    void on_playS_editingFinished();
    void on_playH_editingFinished();
    void on_playD_editingFinished();
    void on_playC_editingFinished();        
    void on_playA_editingFinished();
    /*@}*/

    /** @name Stopper quality of suit when playing NT.
     */
    /*@{*/
    void on_stopNTS_editingFinished();
    void on_stopNTH_editingFinished();
    void on_stopNTD_editingFinished();
    void on_stopNTC_editingFinished();
    /*@}*/

    /** @name General quality assesment of the suit.
     */
    /*@{*/
    void on_qltyS_editingFinished();
    void on_qltyH_editingFinished();
    void on_qltyD_editingFinished();
    void on_qltyC_editingFinished();
    /*@}*/

    /** @name High Card Points + Distribution Points.
     */
    /*@{*/
    void on_pointsS_editingFinished();
    void on_pointsH_editingFinished();
    void on_pointsD_editingFinished();
    void on_pointsC_editingFinished();    
    void on_pointsA_editingFinished();
    /*@}*/

    /** @name Distribution points.
     */
    /*@{*/
    void on_dpS_editingFinished();
    void on_dpH_editingFinished();
    void on_dpD_editingFinished();
    void on_dpC_editingFinished();
    void on_dpA_editingFinished();
    /*@}*/

    /** @name Length of suit.
     */
    /*@{*/
    void on_LengthS_editingFinished();
    void on_LengthH_editingFinished();
    void on_LengthD_editingFinished();
    void on_LengthC_editingFinished();
    /*@}*/

    /** @name Is the suit the longest in the hand.
     */
    /*@{*/
    void on_longestS_editingFinished();
    void on_longestH_editingFinished();
    void on_longestD_editingFinished();
    void on_longestC_editingFinished();
    /*@}*/

    /** @name Is the suit not shortest in the hand.
     */
    /*@{*/
    void on_longS_editingFinished();
    void on_longH_editingFinished();
    void on_longD_editingFinished();
    void on_longC_editingFinished();
    /*@}*/

    /** @name Difference in length between two suits.
     */
    /*@{*/
    void on_difSH_editingFinished();
    void on_difSD_editingFinished();
    void on_difHD_editingFinished();
    void on_difSC_editingFinished();
    void on_difHC_editingFinished();
    void on_difDC_editingFinished();
    /*@}*/

    /** @name Total length of two suits.
     */
    /*@{*/
    void on_sumSH_editingFinished();
    void on_sumSD_editingFinished();
    void on_sumHD_editingFinished();
    void on_sumSC_editingFinished();
    void on_sumHC_editingFinished();
    void on_sumDC_editingFinished();
    /*@}*/

    /** @name Balanced, longer than three, longer major and longer minor.
     */
    /*@{*/
    void on_balanced_editingFinished();
    void on_longerThan3_editingFinished();
    void on_longerMajor_editingFinished();
    void on_longerMinor_editingFinished();
    /*@}*/

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    void setText(QLineEdit *text, int low, int high, int min, int max);
    bool getValues(QString &text, int min, int max, int *low, int *high);

    Ui::CRuleDialog *ui;

    CRule *rule;
    CBidDesc *bidDesc;
    QString *ruleIdDesc;

    qint16 id;                      /**< Identification of the rule. */
    qint8 priority;                 /**< Priority of the rule.. */
    Forcing status;                 /**< Forcing etc. */
    ScoringMethod scoringMethod;    /**< Scoring method. */
    Vulnerability vulnerability;    /**< Vulnerability. */
    quint8 alertId;                 /**< Alert message. */

    CFeatures lowFeatures;          /**< Lower bound of features. */
    CFeatures highFeatures;         /**< Upper bound of features. */
};

#endif // CRULEDIALOG_H
