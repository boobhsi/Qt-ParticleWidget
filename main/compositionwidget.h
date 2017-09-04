#ifndef COMPOSITION_H
#define COMPOSITION_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
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

    void push_back(Solid *input);

private:
    QTimer* timer;
    Solid* test;
    Camera activeCamera;
    GLuint mProjectionMatrixID;
    float aspect;

    QOpenGLShaderProgram mQuadProgram;

    GLuint mQuadVertexBuffer;

    const GLfloat mQuadVertexData[18] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
    };

    GLuint mRenderTextureID;
    GLuint mFrameBufferID;

    GLuint mRenderTextureUniformID;

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void initRenderBuffer();
    void initShader();

    void renderToScreen();

};

#endif // COMPOSITION_H
