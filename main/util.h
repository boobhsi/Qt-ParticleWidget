#ifndef UTIL_H
#define UTIL_H

#include <QVector2D>
#include <QVector3D>
#include <QImage>

QVector2D genRandom2D(float radius);
QVector3D genRandom3D(float radius);
QVector3D genRandomDir3D(unsigned int angle);
void RGBtoLuv(const QImage& from, QImage& to);

#endif // UTIL_H
