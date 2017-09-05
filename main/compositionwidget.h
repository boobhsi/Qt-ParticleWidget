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

    bool isGLReady();

    GLuint getRenderTexture();

protected:
    virtual void paintEvent();

private:
    QTimer* timer;
    std::vector<Solid*> test;
    Camera activeCamera;
    GLuint mProjectionMatrixID;
    float aspect;

    bool glReady;

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
    void initQuadShader();
    void initQuadTexture();
    void initQuadGLBuffer();

    void renderToScreen();

};

#endif // COMPOSITION_H
