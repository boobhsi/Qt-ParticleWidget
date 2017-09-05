#include "editor.h"
#include "ui_editor.h"
#include "particlesystemfeatures.h"

Editor::Editor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Editor),
    ge(0)
{
    ui->setupUi(this);

    initUI();
}

Editor::~Editor()
{
    if(ge) {
        disconnect(ge, SIGNAL(graphChange(void)), 0, 0);
        delete ge;
    }
    delete ui;
}

void Editor::onShowSizeClicked(void)
{
    if(!ge)
    {
        ge = new graphiceditor(this->parentWidget());
        ge->setSpline(nowControll->getSizeSpline());
        connect(ge, SIGNAL(graphChange()), this, SIGNAL(needRestart()));
    }
    ge->show();
}

void Editor::setControlled(ParticleWidget* toControll, GaussianBlur* toGaussian) {
    nowControll = toControll;
    mGaussianBlurControll = toGaussian;

    setUItoInitControll();
    connectChangeSignals();

    connect(this, SIGNAL(needRestart()), nowControll, SLOT(restart()), Qt::QueuedConnection);
}


void Editor::initUI() {
    ui->colEnbaled->setCheckState(Qt::Unchecked);
    ui->graType->setCurrentIndex(0);
    ui->graType->hide();
    ui->head->hide();
    ui->tail->hide();

    connect(ui->solButton, SIGNAL(clicked()), this, SLOT(onShowSizeClicked()));

    for(int i=0;i<EmitterType::TypeCount;i++) {
        ui->typeCombobox->addItem(EmitterTypeString[i].c_str());
    }
    ui->angleSlider->setRange(0, 90);
    connect(ui->angleSlider, SIGNAL(valueChanged(int)), ui->angleObs, SLOT(setNum(int)));
    connect(ui->colEnbaled, SIGNAL(stateChanged(int)), this, SLOT(drawPalettePanel(int)));
    connect(ui->graType, SIGNAL(currentIndexChanged(int)), this, SLOT(brushPaletteLabel(int)));

    ui->radius->setMinimum(0.0);
    ui->scale->setMinimum(0.0);

    ui->max->setRange(1, 1000000);
    ui->life->setRange(0.1f, 100000.0f);
    ui->speed->setRange(0.1f, 100000.0f);
    ui->rate->setRange(1, 100000);
    ui->size->setRange(0.1f, 100.0f);

    ui->gravity->setRange(-100.0f, 100.0f);

    ui->blurTimes->setRange(0, 10);
}

void Editor::drawPalettePanel(int checked) {
    switch(checked) {
    case 0:
        ui->graType->hide();
        ui->head->hide();
        ui->tail->hide();
        break;
    case 1:
        break;
    case 2:
        ui->graType->show();
        brushPaletteLabel(ui->graType->currentIndex());
    default:
        break;
    }
}

void Editor::brushPaletteLabel(int type) {
    switch(type) {
    case 0:
        ui->head->show();
        ui->tail->hide();
        break;
    case 1:
        ui->head->hide();
        ui->tail->show();
        break;
    default:
        ui->head->hide();
        ui->tail->hide();
        break;
    }
}

void Editor::connectChangeSignals() {
    connect(ui->typeCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(onEmitterShapeChange()));
    connect(ui->angleSlider, SIGNAL(valueChanged(int)), this, SLOT(onEmitterShapeChange()));
    connect(ui->radius, SIGNAL(valueChanged(double)), this, SLOT(onEmitterShapeChange()));
    connect(ui->scale, SIGNAL(valueChanged(double)), this, SLOT(onEmitterShapeChange()));

    connect(ui->max, SIGNAL(valueChanged(int)), this, SLOT(onParametersChange()));
    connect(ui->life, SIGNAL(valueChanged(double)), this, SLOT(onParametersChange()));
    connect(ui->rate, SIGNAL(valueChanged(int)), this, SLOT(onParametersChange()));
    connect(ui->size, SIGNAL(valueChanged(double)), this, SLOT(onParametersChange()));
    connect(ui->speed, SIGNAL(valueChanged(double)), this, SLOT(onParametersChange()));

    connect(ui->gravity, SIGNAL(valueChanged(double)), this, SLOT(onEnvironmentChange()));

    connect(ui->colEnbaled, SIGNAL(stateChanged(int)), this, SLOT(onParametersChange()));
    connect(ui->graType, SIGNAL(currentIndexChanged(int)), this, SLOT(onParametersChange()));
    connect(ui->head, SIGNAL(colorChange()), this, SLOT(onColorChange()));
    connect(ui->tail, SIGNAL(gradientChange()), this, SLOT(onGradientChange()));

    connect(ui->blurTimes, SIGNAL(valueChanged(int)), this, SLOT(onBlurChange(int)));
}

void Editor::setUItoInitControll() {
    const EmitterShape& shape = nowControll->getShape();
    ui->typeCombobox->setCurrentIndex(shape.type);
    ui->angleSlider->setValue(shape.angle);
    ui->scale->setValue(shape.scale);
    ui->radius->setValue(shape.radius);

    const EmitParameter& para = nowControll->getParameter();
    ui->rate->setValue(para.emissionRate);
    ui->max->setValue(para.maxParticleNum);
    ui->speed->setValue(para.startSpeed);
    ui->size->setValue(para.startSize);
    ui->life->setValue(para.startLifeTime);

    const Physic& phy = nowControll->getPhysic();
    ui->gravity->setValue(phy.gravityModifier);

    ui->blurTimes->setValue(mGaussianBlurControll->getBlurTimes());

    ui->head->setColor(nowControll->getColor());
    ui->tail->setGradient(nowControll->getGradient());
}

void Editor::onEmitterShapeChange() {
    EmitterShape temp;
    temp.type = (EmitterType)ui->typeCombobox->currentIndex();
    temp.angle = ui->angleSlider->value();
    temp.radius = ui->radius->value();
    temp.scale = ui->scale->value();
    nowControll->setEmitterShape(temp);
}

void Editor::onParametersChange() {
    EmitParameter temp;
    temp.colorCustom = ui->colEnbaled->isChecked();
    if(ui->colEnbaled->isChecked()) {
        temp.colorOverLife = ui->graType->currentIndex() == 0 ? false : true;
    }
    temp.emissionRate = ui->rate->value();
    temp.maxParticleNum = ui->max->value();
    temp.startSpeed = ui->speed->value();
    temp.startSize = ui->size->value();
    temp.startLifeTime = ui->life->value();
    nowControll->setEmitParameter(temp);
}

void Editor::onEnvironmentChange() {
    Physic temp;
    temp.gravityModifier = ui->gravity->value();
    nowControll->setEnviromentPhysic(temp);
}

void Editor::onColorChange() {
    nowControll->setColor(ui->head->getColor());
}

void Editor::onBlurChange(int a) {
    mGaussianBlurControll->setBlurTimes(a);
}

void Editor::onGradientChange() {
    nowControll->setGradient(ui->tail->getGradient());
}
