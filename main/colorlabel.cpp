#include "colorlabel.h"
#include <QColorDialog>

ColorLabel::ColorLabel(const QColor& iColor, float iTime, QWidget *parent) : QFrame(parent), mColor(iColor)
{
    this->setFixedSize(10, parent->height() + 10);
    mTime = iTime;

    this->setStyleSheet("border: 1px solid black");

    QPalette temp(this->palette());
    temp.setColor(QPalette::Window, mColor);
    this->setAutoFillBackground(true);
    this->setPalette(temp);
}

ColorLabel::~ColorLabel() {

}

void ColorLabel::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        mColor = QColorDialog::getColor(mColor, this, "Select Color", QColorDialog::ShowAlphaChannel);

        QPalette temp(this->palette());
        temp.setColor(QPalette::Window, mColor);
        this->setAutoFillBackground(true);
        this->setPalette(temp);

        emit colorElementChange(mColor, mTime);
    }
    else if(event->button() == Qt::RightButton) {
        emit colorElementDelete(this->mTime);
    }
}
