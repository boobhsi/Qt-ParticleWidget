#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "particlewidget.h"
#include "particlesystemfeatures.h"
#include "testwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSize windowSize(600, 600);
    this->setFixedSize(windowSize);

    ParticleWidget* test = new ParticleWidget(this);
    Camera testCam(3.0f, 200.0f, 45.0f);

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
    parameters.maxParticleNum = 1000;
    parameters.emissionRate = 170;
    parameters.texturePath = "";

    test->setEmitterShape(shape);
    test->setEnviromentPhysic(enviroment);
    test->setEmitParameter(parameters);

    test->setEmitterPosition(QVector3D(0.0f, 0.0f, -100.0f));
    testCam.setCameraPosition(QVector3D(0.0f, 0.0f, 5.0f));
    test->setActiveCamera(testCam);

    test->resize(windowSize);
    test->show();
    test->play();

    /*
    TestWidget* test = new TestWidget(this);
    test->resize(windowSize);
    test->show();
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}
