#ifndef TURBULENCE_H
#define TURBULENCE_H

#include <list>
#include <QVector3D>
#include "util.h"
#include "particle.h"

struct Vortex{
    QVector3D mAngularVelocity;
    QVector3D mPosition;
    float mLife = 0.0f;
    QVector3D mSpeed;

    static float mScale;
    static float mVortexLifeTime;

    Vortex(int effectAngle, int forceAngle, float force, float speed);
    ~Vortex();
    QVector3D getForce(const QVector3D &position);

    bool update(float elapsed);
};

class Turbulence
{
public:
    Turbulence();

    QVector3D calculate(Particle &p);

    void update(float elapsed);
    void setTurbulenceField(int effectAngle, int forceAngle, float scale, float life, float speed, float force);

private:
   std::list<Vortex> mVortex;

   float mPeriod;
   float mElapsed;

   int mEffectAngle;
   int mForceAngle;
   float mScale;
   float mSpeed;
   float mForce;

};

#endif // TURBULENCE_H
