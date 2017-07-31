#include "testwidget.h"

TestWidget::TestWidget(QWidget* parent) :
    QOpenGLWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(17); //60fps
}

TestWidget::~TestWidget()
{
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteVertexArrays(1, &mVertexArrayID);
    glDeleteProgram(mProgram.programId());
    delete timer;
}

void TestWidget::initializeGL()
{
    //Initialize QOpenGL(Extra)Functions for current context
    initializeOpenGLFunctions();
    
    //Set background to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    qDebug() << "Initialize shaders";
    initShaders();
    
    //Enable depth test
    //glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    //glDepthFunc(GL_LESS);

    glGenVertexArrays(1, &mVertexArrayID);
    glBindVertexArray(mVertexArrayID);
    
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexData), mVertexData, GL_STATIC_DRAW);

    qDebug() << "Initialized!";
}

void TestWidget::resizeGL(int w, int h)
{
    qDebug() << "resize!";
}

void TestWidget::paintGL()
{
    qDebug() << "paint";

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mProgram.programId());

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexData), mVertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    return;
}

void TestWidget::initShaders()
{

    // Compile vertex shader
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/TestVertex.glsl"))
        close();

    // Compile fragment shader
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/TestFragment.glsl"))
        close();

    // Link shader pipeline
    if (!mProgram.link())
        close();

    // Bind shader pipeline for use
    if (!mProgram.bind())
        close();

    qDebug() << "Shaders initialized!";
}
