#include "camera.h"

Camera::Camera(float inear, float ifar, float ifov) : mCameraPosition(0.0, 0.0, 1.0)
{
    zNear = inear;
    zFar = ifar;
    fov = ifov;
}

QMatrix4x4 Camera::calculateProjectionMatrix(float aspect) {
    // Reset projection
    QMatrix4x4 projectionMatrix;
    projectionMatrix.setToIdentity();

    // Set perspective projection
    projectionMatrix.perspective(fov, aspect, zNear, zFar);
    qDebug() << projectionMatrix;

    //mProjectionMatrix.ortho(-10, 10, -10, 10, zNear, zFar);

    QMatrix4x4 matrix;
    matrix.translate(-mCameraPosition);
    qDebug() << matrix;

    projectionMatrix = projectionMatrix * matrix;
    qDebug() << projectionMatrix;

    return projectionMatrix;
}

const QVector3D& Camera::getCameraPosition() {
    return mCameraPosition;
}

void Camera::setCameraPosition(const QVector3D &input) {
    mCameraPosition = input;
}

