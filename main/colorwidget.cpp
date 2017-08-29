#include "colorwidget.h"
#include <QDebug>

ColorWidget::ColorWidget(QWidget *parent) : QFrame(parent), mCurrent(Qt::white, Qt::red, Qt::blue)
{
    QImage scbg("../res/transparent.jpeg");
    scbg = scbg.scaledToHeight(this->size().height());
    mBrusher = new QBrush(scbg);
    mPalette = new QPalette(this->palette());
    mPalette->setBrush(QPalette::Window, *mBrusher);
    this->setAutoFillBackground(true);
    this->setPalette(*mPalette);
    mUpper = new QLabel(this);
    mUpper->setFixedSize(this->size());
    drawPalette();
}

ColorWidget::~ColorWidget() {

}

void ColorWidget::drawPalette() {
    QPalette temp(mUpper->palette());
    temp.setColor(QPalette::Window, mCurrent);
    mUpper->setAutoFillBackground(true);
    mUpper->setPalette(temp);
}

void ColorWidget::mousePressEvent(QMouseEvent *event) {
    QColor tempColor = QColorDialog::getColor(mCurrent, this, "Select Color", QColorDialog::ShowAlphaChannel);
    mCurrent = tempColor;

    drawPalette();

    emit colorChange();
}

const QColor &ColorWidget::getColor() {
    
    return mCurrent;
}

GradientWidget::GradientWidget(QWidget *parent) : ColorWidget(parent)
{
    mCurrent = new GradientDescriber(Qt::white, Qt::red);
}

GradientWidget::~GradientWidget()
{
    delete mCurrent;
}

const GradientDescriber& GradientWidget::getGradient() {
    return *mCurrent;
}

const QColor& GradientWidget::getColor() {
    return mCurrent->getColor(0.0f);
}

void GradientWidget::mousePressEvent(QMouseEvent* event) {


    emit gradientChange();
}

void GradientWidget::drawPalette() {

}
