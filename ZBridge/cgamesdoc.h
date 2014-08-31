/* Erik Aagaard Knudsen.
  Copyright © 2014 - All Rights Reserved

  Project: ZBridge
  File: CGamesDoc.cpp
  Developers: eak

  Revision History:
  31-aug-2014 eak: Original

  Abstract: A set of bridge games (including results) played by the same players.

  Platforms: Qt.
  */

/**
 * \file
 * The file implements handling of data for a set of bridge games (including results).
 */

#ifndef CGAMESDOC_H
#define CGAMESDOC_H

#include <QObject>

#include "cbidhistory.h"
#include "cplayhistory.h"

class CGamesDoc : public QObject
{
    Q_OBJECT
public:
    explicit CGamesDoc(QObject *parent = 0);

    struct CAuctionAndPlay
    {
        CBidHistory bidHistory;
        CPlayHistory playHistory;
        QString westName;
        QString northName;
        QString eastName;
        QString southName;
    };

    struct CGame
    {
        QString gameId;
        int wCards[13];
        int nCards[13];
        int eCards[13];
        int sCards[13];
        QList<CAuctionAndPlay> auctionAndPlay;
    };

    QList<CGame> games;

signals:

public slots:

};

#endif // CGAMESDOC_H
