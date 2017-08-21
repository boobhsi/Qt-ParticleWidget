#include "camera.h"

Camera::Camera(float inear, float ifar, float ifov) : mCameraPosition(0.0, 0.0, 1.0)
{
    zNear = inear;
    zFar = ifar;
    fov = ifov;
    mCameraPerspective.setToIdentity();
    mCameraRotation.setToIdentity();
}

void Camera::setAspect(float aspect) {
    // Reset projection
    mCameraPerspective.setToIdentity();

    // Set perspective projection
    mCameraPerspective.perspective(fov, aspect, zNear, zFar);

    calculateProjectionMatrix();
}

void Camera::calculateProjectionMatrix() {

    QMatrix4x4 matrixP, matrixR;
    matrixP.translate(-mCameraPosition);
    qDebug() << matrixP;

    matrixR = mCameraRotation.transposed();
    qDebug() << matrixR;

    mProjectionMatrix = mCameraPerspective * (matrixR * matrixP);
    qDebug() << mProjectionMatrix;
}

const QVector3D& Camera::getCameraPosition() const {
    return mCameraPosition;
}

const QMatrix4x4& Camera::getProjectionMatrix() const {
    return mProjectionMatrix;
}

const QMatrix4x4& Camera::getCameraRotation() const {
    return mCameraRotation;
}

void Camera::setCameraPosition(const QVector3D& input) {
    mCameraPosition = input;
    calculateProjectionMatrix();
}

void Camera::setCameraRotation(const QVector3D& input) {
    mCameraRotation.setToIdentity();
    rotateCamera(input);
    calculateProjectionMatrix();
}

void Camera::rotateCamera(const QVector3D& input) {
    QVector3D xr(1.0f, 0.0f, 0.0f);
    QVector3D yr(0.0f, 1.0f, 0.0f);
    QVector3D zr(0.0f, 0.0f, 1.0f);
    mCameraRotation.rotate(input.x(), xr);
    mCameraRotation.rotate(input.y(), yr);
    mCameraRotation.rotate(input.z(), zr);
    calculateProjectionMatrix();
}

void Camera::translateCamera(const QVector3D& input) {
    mCameraPosition += input;
    qDebug() << mCameraPosition;
    calculateProjectionMatrix();
}

void Camera::forwardCamera(const float& dis) {
    translateCamera(dis * *(QVector3D*)(getCameraRotation().data() + 8));
}
