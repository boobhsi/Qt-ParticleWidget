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
#include "CurveEditor/libspline/aaCurve.h"
#include "editor.h"

class ParticleWidget : public Solid, protected QOpenGLExtraFunctions
{
public:
    explicit ParticleWidget(CompositionWidget* cParent);
    ~ParticleWidget();

    void setEmitterPosition(QVector3D input);
    void setEmitterShape(const EmitterShape& shape);
    void setEnviromentPhysic(const Physic& enviroment);
    void setEmitParameter(const EmitParameter& parameter);
    void setGradient(const GradientDescriber& gradient);
    void setColor(const QColor& color);
    void setBlurRadius(double radius);

    const EmitterShape& getShape();
    const EmitParameter& getParameter();
    const Physic& getPhysic();
    float getBlurRadius();

    aaAaa::aaSpline* getSizeSpline();

    void play() override;
    void stop() override;
    void reset(bool replay) override;
    bool isPlaying() override;
    void render() override;

private:
    typedef std::vector<std::pair<double, double>> ValueList;

    Particle* mParticleContainer;
    GLfloat* mParticlePosSizeData;
    GLfloat* mParticleColorData;

    QImage* mTextureImage;
    QImage* mTextureLImage;

    const GLfloat mParticleVertexData[12] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f
    };

    const GLfloat mQuadVertexData[18] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
    };

    static const QVector3D Gravity;

    GLuint mTextureID;
    GLuint mTextureLID;
    GLuint mRenderTextureID;

    GLuint mVertexArrayID;
    GLuint mParticleVertexBuffer;
    GLuint mParticlePosSizeBuffer;
    GLuint mParticleColorBuffer;

    GLuint mQuadVertexBuffer;

    GLuint mFrameBufferID;
    
    GLuint mProjectionMatrixID;
    GLuint mTextureUniformID;
    GLuint mUpAxisID;
    GLuint mRightAxisID;
    GLuint mRenderTextureUniformID;
    GLuint mBlurRadiusUniformID;

    QOpenGLShaderProgram mProgram;
    QOpenGLShaderProgram mQuadProgram;
    
    QVector3D mEmitterPosition;
    
    QTime mFromStartTime;
    
    EmitterShape mShape;
    EmitParameter mParameter;
    Physic mParticlePhysic;
    aaAaa::aaSpline mSizeSpline;
    GradientDescriber mGradient;
    QColor mColor;
    ValueList mSizeCurve;

    unsigned mCurrentParticleNum;
    bool mIsPlaying;
    bool mNeedEmit;
    float mBlurRadius;
    QString mTexturePath;

    Camera activeCamera;
    
    void initializeGL() override;

    void initShaders();
    void initTextures();
    void initGLBuffer();
    void initRenderBuffer();
    void resetGL();
    void resetParameters();
    void presetGL();

    void sortParticle();
    void updateParticles();
    void genStartPos(Particle& p);
    void genPhysicalForce(float sec, Particle& p);
    void changeColorOverTime(Particle& p);
    void reGenerateSizeCurve();
    
    void renderToFBO();
    void renderToScreen();
};

#endif // PARTICLEWIDGET_H
