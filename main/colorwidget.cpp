#include "colorwidget.h"
#include <QLayout>

ColorWidget::ColorWidget(QWidget *parent) : QFrame(parent), mCurrent(Qt::red)
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

void ColorWidget::setColor(const QColor &iColor) {
    mCurrent = iColor;
    drawPalette();
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
    float t = (float)event->pos().x() / (float)width();
    QColor c = QColorDialog::getColor(mCurrent->getColor(t), this, "Select Color", QColorDialog::ShowAlphaChannel);
    addGradientLabel(t, c);
    mCurrent->insertColor(c, t);

    drawPalette();

    emit gradientChange();
}

void GradientWidget::drawPalette() {
    QLinearGradient gradient(0, 0, this->width(), 0);

    const std::vector<GradientDescriber::ColorNode>& temp = mCurrent->getList();

    for(int i=0;i<temp.size();i++) {
        gradient.setColorAt(temp[i].first, temp[i].second);
    }

    QBrush brush(gradient);
    QPalette tempPalette(this->palette());
    tempPalette.setBrush(QPalette::Window, brush);
    mUpper->setFixedSize(this->size());
    mUpper->setAutoFillBackground(true);
    mUpper->setPalette(tempPalette);
}

void GradientWidget::setGradient(const GradientDescriber ig) {
    *mCurrent = ig;
    for(std::map<float, ColorLabel*>::iterator i = mColorInterfaces.begin(); i != mColorInterfaces.end(); i++) {
        (*i).second->hide();

        disconnect((*i).second, SIGNAL(colorElementChange(QColor,float)), this, SIGNAL(onColorChange(QColor,float)));
        disconnect((*i).second, SIGNAL(colorElementDelete(float)), this, SIGNAL(onColorDelete(float)));

        delete (*i).second;
    }
    mColorInterfaces.clear();
    initGradientPalette();

    drawPalette();
}


void GradientWidget::initGradientPalette() {
    const std::vector<GradientDescriber::ColorNode>& temp = mCurrent->getList();
    for(std::vector<GradientDescriber::ColorNode>::const_iterator i = temp.begin(); i != temp.end(); i++) {
        addGradientLabel((*i).first, (*i).second);
    }
}

void GradientWidget::addGradientLabel(float t, const QColor &color) {
    ColorLabel* newLabel = new ColorLabel(color, t, this);
    mColorInterfaces.insert(Cl(t, newLabel));
    newLabel->move((int)(width() * t) - newLabel->width() / 2, height() / 2 - newLabel->height() / 2);
    newLabel->show();

    connect(newLabel, SIGNAL(colorElementChange(QColor,float)), this, SLOT(onColorChange(QColor,float)));
    connect(newLabel, SIGNAL(colorElementDelete(float)), this, SLOT(onColorDelete(float)));
}

void GradientWidget::onColorChange(const QColor& c, float t) {
    mCurrent->changeColor(c, t);

    drawPalette();

    emit gradientChange();
}

void GradientWidget::onColorDelete(float t) {
    if(!mCurrent->deleteColor(t)) return;
    mColorInterfaces[t]->hide();

    disconnect(mColorInterfaces[t], SIGNAL(colorElementChange(QColor,float)), this, SLOT(onColorChange(QColor,float)));
    disconnect(mColorInterfaces[t], SIGNAL(colorElementDelete(float)), this, SLOT(onColorDelete(float)));

    mColorInterfaces.erase(t);

    drawPalette();

    emit gradientChange();
}
