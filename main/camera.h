#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
class Camera
{
public:
    Camera() {}
    Camera(float inear, float ifar, float ifov);
    void calculateProjectionMatrix(float aspect);
    const QVector3D& getCameraPosition() const;
    const QMatrix4x4& getProjectionMatrix() const;
    void setCameraPosition(const QVector3D& input);

private:
    QVector3D mCameraPosition;
    QMatrix4x4 mProjectionMatrix;
    float zNear, zFar, fov;

};

#endif // CAMERA_H
