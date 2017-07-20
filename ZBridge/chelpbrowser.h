/****************************************************************************
** This file is part of the documentation of Qt. It was originally
** published as part of Qt Quarterly.
** http://doc.qt.digia.com/qq/qq28-qthelp.html
****************************************************************************/

#ifndef CHELPBROWSER_H
#define CHELPBROWSER_H

#include <QTextBrowser>
#include <QHelpEngine>
#include <QDebug>

class CHelpBrowser : public QTextBrowser
{
public:
    CHelpBrowser(QHelpEngine* helpEngine, QWidget* parent = 0);
    QVariant loadResource (int type, const QUrl& name);
private:
    QHelpEngine* helpEngine;
};

#endif // CHELPBROWSER_H
