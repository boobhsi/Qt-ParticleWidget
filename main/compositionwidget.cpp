#include "compositionwidget.h"
#include <cassert>

#define OPENGLFUNC this->context()->extraFunctions()

CompositionWidget::CompositionWidget(QWidget* parent) :
    QOpenGLWidget(parent),
    QOpenGLExtraFunctions()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start((int)(1000/60)); //60fps
}

CompositionWidget::~CompositionWidget()
{
    //delete test;
    delete timer;
}

void CompositionWidget::initializeGL()
{
    //Initialize QOpenGL(Extra)Functions for this class

    //makeCurrent();
    initializeOpenGLFunctions();;

    initShader();

    mRenderTextureUniformID = OPENGLFUNC->glGetUniformLocation(mQuadProgram.programId(), "renderTexture");

    //Set composition background to black
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

    // Accept fragment if it closer to the camera than the former one
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initRenderBuffer();

    qDebug() << "Composition Initialized!";

    test->initializeGL();
}

void CompositionWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    aspect = float(w) / float(h ? h : 1);

    activeCamera.setAspect(aspect);
}

void CompositionWidget::paintGL()
{
    qDebug() << "painting";
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    OPENGLFUNC->glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    OPENGLFUNC->glViewport(0, 0, width(), height());
    test->render();

    OPENGLFUNC->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    renderToScreen();
}

const Camera& CompositionWidget::getActiveCamera()
{
    return activeCamera;
}

void CompositionWidget::push_back(Solid *input)
{
    test = input;
}

void CompositionWidget::setActiveCamera(Camera &ac) {
    activeCamera = ac;
}

void CompositionWidget::initRenderBuffer() {
    OPENGLFUNC->glGenTextures(1, &mRenderTextureID);
    OPENGLFUNC->glBindTexture(GL_TEXTURE_2D, mRenderTextureID);
    OPENGLFUNC->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(), this->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    OPENGLFUNC->glGenFramebuffers(1, &mFrameBufferID);
    OPENGLFUNC->glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);

    OPENGLFUNC->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderTextureID, 0);

    GLenum temp = GL_COLOR_ATTACHMENT0;
    OPENGLFUNC->glDrawBuffers(1, &temp);

    assert(OPENGLFUNC->glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    OPENGLFUNC->glGenBuffers(1, &mQuadVertexBuffer);
    OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, mQuadVertexBuffer);
    OPENGLFUNC->glBufferData(GL_ARRAY_BUFFER, sizeof(mQuadVertexData), mQuadVertexData, GL_STATIC_DRAW);
}

void CompositionWidget::initShader() {
    if(!mQuadProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "../shaders/quadVertex.glsl"))
        close();

    if(!mQuadProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "../shaders/quadFrag.glsl"))
        close();

    if(!mQuadProgram.link())
        close();

    if(!mQuadProgram.bind())
        close();
}

void CompositionWidget::renderToScreen() {
    OPENGLFUNC->glUseProgram(mQuadProgram.programId());

    OPENGLFUNC->glActiveTexture(GL_TEXTURE0);
    OPENGLFUNC->glBindTexture(GL_TEXTURE_2D, mRenderTextureID);
    OPENGLFUNC->glUniform1i(mRenderTextureUniformID ,0);

    /*
    OPENGLFUNC->glUniform1i(mBlurTimesUniformID, mBlurTimes);
    OPENGLFUNC->glUniform1i(mResolutionUniformID, cParent->size().height());
    */

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

