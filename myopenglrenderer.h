#ifndef MYOPENGLRENDERER_H
#define MYOPENGLRENDERER_H

#include <QOpenGLWidget>

class MyOpenGLRenderer : public QOpenGLWidget
{
public:
    MyOpenGLRenderer();

    void setCameraPosition(QVector3D input);

private:

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};

#endif // MYOPENGLRENDERER_H
