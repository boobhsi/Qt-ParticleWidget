#ifndef COMPOSITION_H
#define COMPOSITION_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "camera.h"
#include <QTimer>
#include <memory>
#include "solid.h"
#include "cameradragger.h"

class Solid;

class CompositionWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    friend class CameraDragger;
public:
    explicit CompositionWidget(QWidget* parent = Q_NULLPTR);
    ~CompositionWidget();

    void setActiveCamera(Camera& ac);
    const Camera& getActiveCamera();

    void push_back(std::shared_ptr<Solid> input);

private:
    QTimer* timer;
    std::shared_ptr<Solid> test;
    Camera activeCamera;
    GLuint mProjectionMatrixID;
    float aspect;

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

};

#endif // COMPOSITION_H
