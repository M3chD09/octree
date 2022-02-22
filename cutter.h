#ifndef CUTTER_H
#define CUTTER_H

#include <QRectF>
#include <QVector3D>

class Cutter {
public:
    Cutter();

    float radius = 0.1f;
    unsigned int nowTime = 0;
    unsigned int maxTime = 500;

    bool isInside(unsigned int time, QVector3D location);

    QVector3D getLocation(unsigned int time);
    QRectF getRect(unsigned int time);
};

#endif // CUTTER_H
