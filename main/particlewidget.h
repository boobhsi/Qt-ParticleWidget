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

    const EmitterShape& getShape();
    const EmitParameter& getParameter();
    const Physic& getPhysic();

    const QColor& getColor();
    const GradientDescriber& getGradient();

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

    static const QVector3D Gravity;

    GLuint mTextureID;
    GLuint mTextureLID;

    GLuint mVertexArrayID;
    GLuint mParticleVertexBuffer;
    GLuint mParticlePosSizeBuffer;
    GLuint mParticleColorBuffer;
    
    GLuint mProjectionMatrixID;
    GLuint mTextureUniformID;
    GLuint mUpAxisID;
    GLuint mRightAxisID;
    GLuint mBlurTimesUniformID;
    GLuint mResolutionUniformID;

    QOpenGLShaderProgram mProgram;
    
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
    QString mTexturePath;
    
    void initializeGLContent() override;
    void initShader() override;
    void initGLBuffer() override;

    void initTextures();
    void resetGL();
    void resetParameters();

    void sortParticle();
    void updateParticles();
    void genStartPos(Particle& p);
    void genPhysicalForce(float sec, Particle& p);
    void changeColorOverTime(Particle& p);
    void reGenerateSizeCurve();
};

#endif // PARTICLEWIDGET_H
