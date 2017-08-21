#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
class Camera
{
public:
    Camera() {}
    Camera(float inear, float ifar, float ifov);
    void calculateProjectionMatrix();
    const QVector3D& getCameraPosition() const;
    const QMatrix4x4& getProjectionMatrix() const;
    const QMatrix4x4& getCameraRotation() const;
    void setCameraPosition(const QVector3D& input);
    void setCameraRotation(const QVector3D& input);
    void rotateCamera(const QVector3D& input);
    void translateCamera(const QVector3D& input);
    void forwardCamera(const float& dis);
    void setAspect(float aspect);

private:
    QVector3D mCameraPosition;
    QMatrix4x4 mCameraRotation;
    QMatrix4x4 mCameraPerspective;
    QMatrix4x4 mProjectionMatrix;
    float zNear, zFar, fov;

};

#endif // CAMERA_H
