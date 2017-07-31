#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QWidget>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include "particlewidget.h"

class TestWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
public:
    explicit TestWidget(QWidget* parent = Q_NULLPTR);
    ~TestWidget();

private:
    const GLfloat mVertexData[9] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    QTimer* timer;

    GLuint mVertexArrayID;
    GLuint mVertexBuffer;
    
    QOpenGLShaderProgram mProgram;
    
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    
    void initShaders();
};

#endif // TESTWIDGET_H
