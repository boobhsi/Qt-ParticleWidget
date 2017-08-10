#ifndef PARTICLEWIDGET_H
#define PARTICLEWIDGET_H

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QTime>
#include <QTimer>
#include "particle.h"
#include "particlesystemfeatures.h"
#include "util.h"
#include "camera.h"
#include "solid.h"

class ParticleWidget : public Solid, protected QOpenGLExtraFunctions
{
public:
    explicit ParticleWidget(CompositionWidget* parent);
    ~ParticleWidget();

    void setEmitterPosition(QVector3D input);
    void setEmitterShape(EmitterShape& shape);
    void setEnviromentPhysic(Physic& enviroment);
    void setEmitParameter(EmitParameter& parameter);

    void play();
    void stop();
    bool isPlaying();

    void render() override;

private:
    Particle* mParticleContainer;
    GLfloat* mParticlePosSizeData;
    GLubyte* mParticleColorData;

    QOpenGLTexture *mTexture;

    const GLfloat mParticleVertexData[12] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f
    };

    static const QVector3D Gravity;

    GLuint mVertexArrayID;
    GLuint mParticleVertexBuffer;
    GLuint mParticlePosSizeBuffer;
    GLuint mParticleColorBuffer;

    GLuint mProjectionMatrixID;
    GLuint mTextureID;
    
    QOpenGLShaderProgram mProgram;
    
    QVector3D mEmitterPosition;
    
    QTime mFromStartTime;
    
    EmitterShape mShape;
    EmitParameter mParameter;
    Physic mParticlePhysic;

    unsigned mCurrentParticleNum;
    bool mIsPlaying;
    QString mTexturePath;

    Camera activeCamera;
    
    void initShaders();
    void initTextures();
    void initializeGL() override;

    void sortParticle();
    void updateParticles();
    void genStartPos(Particle& p);
    void genPhysicalForce(float sec, Particle& p);
};

#endif // PARTICLEWIDGET_H
