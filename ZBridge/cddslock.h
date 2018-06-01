#ifndef CDDSLOCK_H
#define CDDSLOCK_H

#include <QMutex>

class CddsLock
{
public:
    CddsLock();

    static QMutex mutex;
};

#endif // CDDSLOCK_H
