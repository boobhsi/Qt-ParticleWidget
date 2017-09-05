#include "gaussianblur.h"

GaussianBlur::GaussianBlur(CompositionWidget *parent) :
    Solid(parent)
{
    mBlurTimes = 0;
}

GaussianBlur::~GaussianBlur() {

}

void GaussianBlur::render() {
    for(int i=0;i<mBlurTimes;i++) {
        OPENGLFUNC->glUseProgram(mGaussianBlurProgram.programId());

        OPENGLFUNC->glActiveTexture(GL_TEXTURE0);
        OPENGLFUNC->glBindTexture(GL_TEXTURE_2D, mRenderTextureID);
        OPENGLFUNC->glUniform1i(mRenderTextureUniformID ,0);

        OPENGLFUNC->glUniform1i(mBlurStepUniformID, 1.0 / cParent->size().height());

        OPENGLFUNC->glEnableVertexAttribArray(0);
        OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, mQuadVertexBuffer);
        OPENGLFUNC->glVertexAttribPointer(
                    0,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    0,
                    (void*)0
                    );

        OPENGLFUNC->glDrawArrays(GL_TRIANGLES, 0, 6);

        OPENGLFUNC->glDisableVertexAttribArray(0);
    }
}

void GaussianBlur::play() {

}

void GaussianBlur::stop() {

}

void GaussianBlur::reset(bool replay) {

}

bool GaussianBlur::isPlaying() {
    return true;
}

void GaussianBlur::initializeGLContent()
{
    mBlurStepUniformID = OPENGLFUNC->glGetUniformLocation(mGaussianBlurProgram.programId(), "blurStep");
}

void GaussianBlur::initGLBuffer()
{
    OPENGLFUNC->glGenBuffers(1, &mQuadVertexBuffer);
    OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, mQuadVertexBuffer);
    OPENGLFUNC->glBufferData(GL_ARRAY_BUFFER, sizeof(mQuadVertexData), mQuadVertexData, GL_STATIC_DRAW);
}

void GaussianBlur::initShader()
{
    if(!mGaussianBlurProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "../shaders/QuadVertex.glsl"))
        close();

    if(!mGaussianBlurProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "../shaders/GaussianFragment.glsl"))
        close();

    if(!mGaussianBlurProgram.link())
        close();

    if(!mGaussianBlurProgram.bind())
        close();
}

void GaussianBlur::initTexture()
{
    mRenderTextureUniformID = OPENGLFUNC->glGetUniformLocation(mGaussianBlurProgram.programId(), "renderTexture");

    OPENGLFUNC->glGenTextures(1, &mRenderTextureID);
    OPENGLFUNC->glBindTexture(GL_TEXTURE_2D, mRenderTextureID);
    OPENGLFUNC->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->cParent->width(), this->cParent->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
