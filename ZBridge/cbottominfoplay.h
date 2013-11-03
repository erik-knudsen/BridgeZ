#ifndef CBOTTOMINFOPLAY_H
#define CBOTTOMINFOPLAY_H

#include <QGraphicsWidget>

class CBottomInfoPlay : public QGraphicsWidget
{
public:
    explicit CBottomInfoPlay(QGraphicsWidget *parent = 0);

    void showNSTricks(int tricks);
    void showEWTricks(int tricks);

private:
    QGraphicsSimpleTextItem *nsLabel;
    QGraphicsSimpleTextItem *ewLabel;
    QGraphicsSimpleTextItem *nsTricks;
    QGraphicsSimpleTextItem *ewTricks;
};

#endif // CBOTTOMINFOPLAY_H
