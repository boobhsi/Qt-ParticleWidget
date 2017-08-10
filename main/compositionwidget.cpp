#include "compositionwidget.h"

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

    //Set composition background to black
    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

    // Accept fragment if it closer to the camera than the former one
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    test->render();
}

const Camera& CompositionWidget::getActiveCamera()
{
    return activeCamera;
}

void CompositionWidget::push_back(std::shared_ptr<Solid> input)
{
    test = input;
}

void CompositionWidget::setActiveCamera(Camera &ac) {
    activeCamera = ac;
}

