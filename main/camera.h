#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
class Camera
{
public:
    Camera() {}
    Camera(float inear, float ifar, float ifov);
    QMatrix4x4 calculateProjectionMatrix(float aspect);
    const QVector3D& getCameraPosition();
    void setCameraPosition(const QVector3D& input);

private:
    QVector3D mCameraPosition;
    float zNear, zFar, fov;

};

#endif // CAMERA_H
