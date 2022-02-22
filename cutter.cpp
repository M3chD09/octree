#include "cutter.h"

Cutter::Cutter()
{
}

bool Cutter::isInside(unsigned int time, QVector3D location)
{
    location = location - getLocation(time);
    if (location.z() >= 0 && location.z() < radius) {
        return location.distanceToPoint(QVector3D(0, 0, radius)) <= radius;
    } else if (location.z() >= radius) {
        return location.distanceToLine(QVector3D(0, 0, 0), QVector3D(0, 0, 1)) <= radius;
    }

    return false;
}

QVector3D Cutter::getLocation(unsigned int time)
{
    const float z = 0.7f;

    if (time <= 100) {
        return QVector3D(-0.6f * (float)time / 100.0f, 0.8f - 1.6f * (float)time / 100.0f, z);
    }
    if (time <= 200) {
        return QVector3D(-0.6f + 1.4f * (float)(time - 100) / 100.0f, -0.8f + 1.0f * (float)(time - 100) / 100.0f, z);
    }
    if (time <= 300) {
        return QVector3D(0.8f - 1.6f * (float)(time - 200) / 100.0f, 0.2f, z);
    }
    if (time <= 400) {
        return QVector3D(-0.8f + 1.4f * (float)(time - 300) / 100.0f, 0.2f - 1.0f * (float)(time - 300) / 100.0f, z);
    }
    if (time <= 500) {
        return QVector3D(0.6f - 0.6f * (float)(time - 400) / 100.0f, -0.8f + 1.6f * (float)(time - 400) / 100.0f, z);
    }

    return QVector3D(0, (float)time / 100.0f, z);
}

QRectF Cutter::getRect(unsigned int time)
{
    QRectF rect = QRectF(0, 0, radius * 2.0f, radius * 2.0f);
    rect.moveCenter(getLocation(time).toPointF());
    return rect;
}
