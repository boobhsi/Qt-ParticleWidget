#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H

#include "solid.h"
#include <QOpenGLShaderProgram>

class GaussianBlur : public Solid
{
public:
    explicit GaussianBlur(CompositionWidget* cParent);
    ~GaussianBlur();

    void render() override;
    void play() override;
    void stop() override;
    void reset(bool replay) override;
    bool isPlaying() override;

    void setBlurTimes(int time);
    int getBlurTimes();

private:
    int mBlurTimes;

    QOpenGLShaderProgram mGaussianBlurProgram;

    GLuint mQuadVertexBuffer;

    const GLfloat mQuadVertexData[18] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
    };

    GLuint mRenderTextureUniformID;
    GLuint mBlurStepUniformID;

    void initializeGLContent() override;
    void initGLBuffer() override;
    void initShader() override;
};

#endif // GAUSSIANBLUR_H
