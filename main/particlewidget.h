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

class ParticleWidget  : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
public:
    explicit ParticleWidget(QWidget* parent = Q_NULLPTR, unsigned pnInput = 1000, float scaleInput = 1);
    ~ParticleWidget();
    void setEmitterPosition(QVector3D input);
    void setCameraPosition(QVector3D input);
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

    const float zNear = 3.0, zFar = 100.0, fov = 45.0;
    float aspect;

    GLuint mVertexArrayID;
    GLuint mParticleVertexBuffer;
    GLuint mParticlePosSizeBuffer;
    GLuint mParticleColorBuffer;

    GLuint mProjectionMatrixID;
    GLuint mTextureID;
    
    QOpenGLShaderProgram mProgram;
    
    QMatrix4x4 mProjectionMatrix;
    QVector3D mCameraPosition;
    QVector3D mEmitterPosition;
    
    QTime mFromStartTime;
    
    unsigned mMaxParticleNum;
    unsigned mCurrentParticleNum;
    bool mIsPlaying;
    float mParticleScale;
    QString mTexturePath;
    
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    
    void initShaders();
    void initTextures();
    
    int findUnusedParticle();
    void sortParticle();
    void updateParticles();
    void calculateProjectionMatrix();
    
};

#endif // PARTICLEWIDGET_H
