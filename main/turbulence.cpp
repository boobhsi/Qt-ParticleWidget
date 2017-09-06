#include "turbulence.h"

Vortex::Vortex(int effectAngle, int forceAngle, float force, float speed) {
    mAngularVelocity = QVector3D(genRandomDir3D(forceAngle)) * force;
    mPosition = QVector3D(0.0f, 0.0f, -100.0f);
    mSpeed = QVector3D(genRandomDir3D(effectAngle)) * speed;
}

Vortex::~Vortex() {

}

QVector3D Vortex::getForce(const QVector3D& position) {
    QVector3D displace = position - mPosition;

    QVector3D force = QVector3D::crossProduct(mAngularVelocity, displace);

    float factor = 1 / (1 + displace.lengthSquared() / mScale);

    float lifeFactor = mLife / mVortexLifeTime;
    factor *= (1-lifeFactor) * lifeFactor * 4;

    force *= factor;

    return force;
}

bool Vortex::update(float elapsed) {
    mLife += elapsed;

    if(mLife > mVortexLifeTime) return false;

    mPosition += mSpeed * elapsed;

    return true;
}

float Vortex::mVortexLifeTime = 0.0f;
float Vortex::mScale = 0.0f;

Turbulence::Turbulence()
{
    mPeriod = 1.0f;
    mElapsed = 0.0f;

    mEffectAngle = 0;
    mForceAngle = 0;
    mScale = 0.0f;
    mSpeed = 0.0f;
    mForce = 0.0f;
}

QVector3D Turbulence::calculate(Particle &p) {
    QVector3D temp(0.0f, 0.0f, 0.0f);

    for(std::list<Vortex>::iterator i = mVortex.begin(); i != mVortex.end(); i++) {
        temp += (*i).getForce(p.pos);
    }

    return temp;
}

void Turbulence::update(float elapsed) {
    mElapsed += elapsed;
    if(mElapsed > mPeriod) {
        Vortex newGen(mEffectAngle, mForceAngle, mForce, mSpeed);
        mVortex.push_back(newGen);
        mElapsed = 0.0f;
    }

    int toDie = 0;

    for(std::list<Vortex>::iterator i = mVortex.begin(); i != mVortex.end(); i++) {
          if(!(*i).update(elapsed)) toDie += 1;
    }

    for(int i=0;i<toDie;i++) mVortex.pop_front();
}

void Turbulence::setTurbulenceField(int effectAngle, int forceAngle, float scale, float life, float speed, float force){
    mEffectAngle = effectAngle;
    mForceAngle = forceAngle;
    Vortex::mScale = scale;
    mSpeed = speed;
    mForce = force;
    Vortex::mVortexLifeTime = life;
}
