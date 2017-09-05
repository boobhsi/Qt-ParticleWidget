#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "particlesystemfeatures.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSize windowSize(940, 640);
    QSize compositionSize(640, 640);
    //QSize editorSize(300, 720);
    this->setFixedSize(windowSize);

    testBackground = new QLabel(this);
    testBackground->setFixedSize(300, 300);
    QPixmap dial("../res/dial_test");
    dial = dial.scaled(300, 300);
    testBackground->setPixmap(dial);

    ctrl = new CameraDragger(this);
    ctrl->setFixedSize(compositionSize);

    composition = new CompositionWidget(ctrl);
    composition->resize(ctrl->size());
    ctrl->setControlled(composition);

    Camera testCam(3.0f, 200.0f, 45.0f);
    testCam.setCameraPosition(QVector3D(0.0f, 0.0f, 5.0f));
    composition->setActiveCamera(testCam);

    ParticleWidget* test = new ParticleWidget(composition);

    EmitterShape shape;
    shape.type = CONE;
    shape.angle = 60;
    shape.radius = 2.0f;
    shape.scale = 1.0f;

    Physic enviroment;
    enviroment.gravityModifier = -0.8f;

    EmitParameter parameters;
    parameters.startLifeTime = 10.0f;
    parameters.startSpeed = 5.0f;
    parameters.startSize = 1.0f;
    parameters.maxParticleNum = 100000;
    parameters.emissionRate = 10000;
    parameters.texturePath = "../texture/fire.jpg";

    test->setEmitterShape(shape);
    test->setEnviromentPhysic(enviroment);
    test->setEmitParameter(parameters);
    test->setEmitterPosition(QVector3D(0.0f, 0.0f, -100.0f));

    composition->push_back(test);

    GaussianBlur* testGaussian = new GaussianBlur(composition);

    composition->push_back(testGaussian);

    comEditor = new Editor(this);
    comEditor->move(640, 10);
    comEditor->setControlled(test, testGaussian);

    test->play();
    //composition->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}
