#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "particlewidget.h"
#include "testwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSize windowSize(600, 600);
    this->setFixedSize(windowSize);

    ParticleWidget* test = new ParticleWidget(this, 20000, 1);
    test->setEmitterPosition(QVector3D(0.0f, 0.0f, -20.0f));
    test->setCameraPosition(QVector3D(0.0f, 0.0f, 5.0f));
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
