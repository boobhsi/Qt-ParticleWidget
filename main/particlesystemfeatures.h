#ifndef PARTICLESYSTEMFEATURES_H
#define PARTICLESYSTEMFEATURES_H

#include <QVector3D>
#include "particle.h"
#include "util.h"

enum EmitterType{CONE};

struct EmitterShape{
    EmitterType type = CONE;
    unsigned angle = 45;
    float radius = 10.0f;
    float scale = 1.0f;
};

struct Physic{
    float gravityModifier = 1.0f;

};

struct EmitParameter{
    //float duration;
    //bool looping;
    float startLifeTime = 1.0f;
    float startSpeed = 1.0f;
    float startSize = 1.0f;
    unsigned maxParticleNum = 1000;
    unsigned emissionRate = 100;
    QString texturePath = "";
    //bool colorRandomize;
};

#endif // PARTICLESYSTEMFEATURES_H
