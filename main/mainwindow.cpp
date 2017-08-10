#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "particlewidget.h"
#include "particlesystemfeatures.h"
#include "compositionwidget.h"
#include "cameradragger.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSize windowSize(600, 600);
    this->setFixedSize(windowSize);

    CameraDragger* ctrl = new CameraDragger(this);
    ctrl->setFixedSize(windowSize);

    CompositionWidget* composition = new CompositionWidget(ctrl);
    composition->resize(ctrl->size());
    ctrl->setControlled(composition);

    Camera testCam(3.0f, 200.0f, 45.0f);
    composition->setActiveCamera(testCam);

    std::shared_ptr<ParticleWidget> test(new ParticleWidget(composition));

    EmitterShape shape;
    shape.type = CONE;
    shape.angle = 45;
    shape.radius = 10.0f;
    shape.scale = 1.0f;

    Physic enviroment;
    enviroment.gravityModifier = -1.0f;

    EmitParameter parameters;
    parameters.startLifeTime = 5.0f;
    parameters.startSpeed = 5.0f;
    parameters.startSize = 1.0f;
    parameters.maxParticleNum = 50000;
    parameters.emissionRate = 3000;
    parameters.texturePath = "";

    test->setEmitterShape(shape);
    test->setEnviromentPhysic(enviroment);
    test->setEmitParameter(parameters);

    test->setEmitterPosition(QVector3D(0.0f, 0.0f, -100.0f));

    composition->push_back(std::dynamic_pointer_cast<Solid>(test));

    test->play();

}

MainWindow::~MainWindow()
{
    delete ui;
}
