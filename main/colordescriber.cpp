#include "colordescriber.h"

ColorDescriber::ColorDescriber()
{
    head = new QColor(Qt::white);
    tail = new QColor(Qt::white);
}

ColorDescriber::~ColorDescriber() {
    delete head, tail;
}

QColor ColorDescriber::getColor(float t) {
    if(t > 1.0f) t = 1.0f;
    else if(t < 0.0f) t = 0.0f;
    int rt = (tail->red() - head->red()) * t + head->red();
    int gt = (tail->green() - head->green()) * t + head->green();
    int bt = (tail->blue() - head->blue()) * t + head->blue();
    int at = (tail->alpha() - head->alpha()) * t + head->alpha();
    QColor temp(rt, gt, bt, at);
    return temp;
}

const QColor& ColorDescriber::getSingleColor() {
    return *head;
}
