#ifndef COLORDESCRIBER_H
#define COLORDESCRIBER_H

#include <QColor>

struct ColorDescriber
{
    QColor* head, tail;

    ColorDescriber();
    ~ColorDescriber();

    const QColor& getSingleColor();
    QColor getColor(float t);
};

#endif // COLORDESCRIBER_H
