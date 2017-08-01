#ifndef PARTICLEWIDGET_H
#define PARTICLEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTime>
#include <QTimer>
#include "particle.h"
#include "particlesystemfeatures.h"
#include "util.h"
#include "camera.h"

class ParticleWidget  : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
public:
    explicit ParticleWidget(QWidget* parent = Q_NULLPTR);
    ~ParticleWidget();

    void setEmitterPosition(QVector3D input);
    void setCameraPosition(QVector3D input);

    void setEmitterShape(EmitterShape& shape);
    void setEnviromentPhysic(Physic& enviroment);
    void setEmitParameter(EmitParameter& parameter);
    void setActiveCamera(Camera& ac);

    void play();
    void stop();
    bool isPlaying();

private:
    Particle* mParticleContainer;
    GLfloat* mParticlePosSizeData;
    GLubyte* mParticleColorData;

    QOpenGLTexture *mTexture;

    QTimer* timer;

    const GLfloat mParticleVertexData[12] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f
    };

    float aspect;

    GLuint mVertexArrayID;
    GLuint mParticleVertexBuffer;
    GLuint mParticlePosSizeBuffer;
    GLuint mParticleColorBuffer;

    GLuint mProjectionMatrixID;
    GLuint mTextureID;
    
    QOpenGLShaderProgram mProgram;
    
    QMatrix4x4 mProjectionMatrix;
    QVector3D mEmitterPosition;
    
    QTime mFromStartTime;
    
    EmitterShape mShape;
    EmitParameter mParameter;
    Physic mParticlePhysic;

    unsigned mCurrentParticleNum;
    bool mIsPlaying;
    QString mTexturePath;

    Camera activeCamera;
    
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    
    void initShaders();
    void initTextures();

    void sortParticle();
    void updateParticles();
    void calculateProjectionMatrix();
    void genStartPos(Particle& p);
    void genPhysicalForce(float sec, Particle& p);

};

#endif // PARTICLEWIDGET_H
