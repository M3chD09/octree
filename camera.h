#ifndef CAMERA_H
#define CAMERA_H

#include <QKeyEvent>
#include <QMouseEvent>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>
#include <QWheelEvent>

class Camera {
public:
    Camera();

    float fov = 45.0f;
    QVector3D pos = QVector3D(2.5f, 2.5f, 2.5f);
    QVector3D front = QVector3D(-1.0f, -1.0f, -1.0f);
    QVector3D up = QVector3D(0.0f, 1.0f, 0.0f);
    QQuaternion rotation;

    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

private:
    bool isFirstMousePress = true;
    QVector2D mousePressPosition;
    QVector3D rotationAxis;
};

#endif // CAMERA_H
