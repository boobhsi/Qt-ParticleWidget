#ifndef PARTICLE_H
#define PARTICLE_H

#include <QVector3D>

struct Particle{
    QVector3D pos, speed;
    unsigned char r, g, b, a;
    float size, angle, weight;
    float life;
    float cameraDistance;
    
    bool operator < (const Particle& other) const {
        return this->cameraDistance > other.cameraDistance;
    }
};

#endif // PARTICLE_H
