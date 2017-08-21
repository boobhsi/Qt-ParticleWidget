#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>

struct Particle{
    QVector3D pos, speed;
    unsigned char r, g, b, a;
    float size;
    float life;
    float cameraDistance;

    Particle() {
        r = 0;
        g = 0;
        b = 0;
        a = 0;
        size = 0.0f;
        life = -1.0f;
        cameraDistance = -1.0f;
    }

    bool operator < (const Particle& other) const {
        return this->cameraDistance > other.cameraDistance;
    }

    bool checkLife(float sec) {
        life -= sec;
        if(life >= 0.0f) {
            return true;
        }
        else {
            cameraDistance = -1.0f;
            life = -1.0f;
            return false;
        }
    }
};

#endif // PARTICLE_H
