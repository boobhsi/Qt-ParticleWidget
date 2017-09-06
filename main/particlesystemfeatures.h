#ifndef PARTICLESYSTEMFEATURES_H
#define PARTICLESYSTEMFEATURES_H

#include <QVector3D>
#include "particle.h"
#include "util.h"
#include "CurveEditor/libspline/aaCurve.h"
#include "gradientdescriber.h"

enum EmitterType{CONE, TypeCount};

const std::string EmitterTypeString[EmitterType::TypeCount] = {"Cone"};

struct EmitterShape{
    EmitterType type = CONE;
    unsigned angle = 45;
    float radius = 10.0f;
    float scale = 1.0f;
};

struct Physic{
    float gravityModifier = 1.0f;
    float damping = 0.995;
    float terminalSpeed = 10.0f;
};

struct EmitParameter{
    float startLifeTime = 1.0f;
    float startSpeed = 1.0f;
    float startSize = 1.0f;
    unsigned maxParticleNum = 1000;
    unsigned emissionRate = 100;
    QString texturePath = "";
    bool colorCustom = false;
    bool colorOverLife= false;
};

#endif // PARTICLESYSTEMFEATURES_H
