#include "util.h"
#include <QtMath>
#include <QDebug>

QVector2D genRandom2D(float radius){
    if(radius == 0.0f) return QVector2D(0.0f, 0.0f);
    qreal phi = (qreal)(qrand()%20000)/10000.0f * M_PI;
    qreal u = (qreal)(qrand()%10000)/10000.0f;

    float r = radius * sqrt(u);
    QVector2D temp(
                r * cos(phi),
                r * sin(phi)
                );
    return temp;
}

QVector3D genRandom3D(float radius) {

    qreal phi = (qreal)(qrand()%20000)/10000.0f * M_PI;
    qreal costheta = (qreal)(qrand()%20000 - 10000)/10000.0f;
    qreal u = (qreal)(qrand()%10000)/10000.0f;

    qreal theta = acos(costheta);

    float r = radius * cbrt(u);
    QVector3D temp(
                r * sin(theta) * cos(phi),
                r * sin(theta) * sin(phi),
                r * cos(theta)
                );
    return temp;
}

QVector3D genRandomDir3D(unsigned int angle) {
    //Q_ASSERT(angle <= 90 && angle >= 0.0f);

    qreal phi = (qreal)(qrand()%20000)/10000.0f * M_PI;
    qreal costheta;
    if(angle == 0) costheta = 1;
    else costheta = (qreal)(10000 - qrand() % (int)floor(10000 * angle / 90.0f))/10000.0f;
    qreal theta = acos(costheta);
    QVector3D temp(
                sin(theta) * cos(phi),
                cos(theta),
                sin(theta) * sin(phi)
                );
    return temp * log(cos(theta) + 1);
}

void RGBtoLuv(const QImage& from, QImage& to) {
    qDebug() << from.byteCount();
    const double rY = 0.299, gY = 0.587, bY = 0.114;
    const uchar* fPointer = from.bits();
    uchar* tPointer = to.bits();
    for(int i=0;i<from.height();i++) {
        for(int j=0;j<from.width();j++) {
            int index = i * from.bytesPerLine() + j * 4;
            int Y = (int)(fPointer[index + 2] & 0xff) * rY +
                    (int)(fPointer[index + 1] & 0xff) * gY +
                    (int)(fPointer[index] & 0xff) * bY;
            tPointer[index] = Y;
            tPointer[index + 1] = Y;
            tPointer[index + 2] = Y;
            tPointer[index + 3] = fPointer[index + 3];

            qDebug() << "(" << (int)(fPointer[index]) << ", " << (int)(fPointer[index + 1]) << ", " << (int)(fPointer[index + 2]) << ", " << (int)(fPointer[index + 3]) << ") to (" << Y << ")";
        }
    }
}
