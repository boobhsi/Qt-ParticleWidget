#include "util.h"
#include <QtMath>

QVector2D genRandom2D(float radius){
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

QVector3D genRandomDir3D(float angle) {
    qreal phi = (qreal)(qrand()%20000)/10000.0f * M_PI;
    float limit = 10000 * angle / 90.0f;
    qreal costheta;
    if(limit < 1.0f) {
        costheta = 1.0f;
    }
    else{
        costheta = (qreal)(10000 - qrand() % (int)floor(10000 * angle / 90.0f))/10000.0f;
    }
    qreal theta = acos(costheta);

    QVector3D temp(
                sin(theta) * cos(phi),
                sin(theta) * sin(phi),
                cos(theta)
                );
    return temp;
}
