#include "particlewidget.h"
#include <QRgb>
#include <cassert>

ParticleWidget::ParticleWidget(CompositionWidget *parent) :
    Solid(parent),
    mSizeSpline("Size"),
    mGradient(Qt::white, Qt::red),
    mColor(Qt::red)
{
    mParticleContainer = new Particle[mParameter.maxParticleNum];
    mParticlePosSizeData = new GLfloat[mParameter.maxParticleNum * 4];
    mParticleColorData = new GLfloat[mParameter.maxParticleNum * 4];

    mIsPlaying = false;
    mNeedEmit = false;
    mCurrentParticleNum = 0;

    mTurbu = new Turbulence();

    resetParameters();
}

ParticleWidget::~ParticleWidget()
{
    delete mTextureImage;
    delete mTextureLImage;
    delete [] mParticleContainer;
    delete [] mParticlePosSizeData;
    delete [] mParticleColorData;

    delete mTurbu;
}

void ParticleWidget::initializeGLContent()
{
    mProjectionMatrixID = OPENGLFUNC->glGetUniformLocation(mProgram.programId(), "mvp");
    mUpAxisID = OPENGLFUNC->glGetUniformLocation(mProgram.programId(), "upAxis");
    mRightAxisID = OPENGLFUNC->glGetUniformLocation(mProgram.programId(), "rightAxis");

    resetParameters();
    initTextures();
}

void ParticleWidget::initGLBuffer()
{
    OPENGLFUNC->glGenVertexArrays(1, &mVertexArrayID);
    OPENGLFUNC->glBindVertexArray(mVertexArrayID);

    OPENGLFUNC->glGenBuffers(1, &mParticleVertexBuffer);
    OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, mParticleVertexBuffer);
    OPENGLFUNC->glBufferData(GL_ARRAY_BUFFER, sizeof(mParticleVertexData), mParticleVertexData, GL_STATIC_DRAW);

    OPENGLFUNC->glGenBuffers(1, &mParticlePosSizeBuffer);
    OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, mParticlePosSizeBuffer);
    OPENGLFUNC->glBufferData(GL_ARRAY_BUFFER, mParameter.maxParticleNum * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    OPENGLFUNC->glGenBuffers(1, &mParticleColorBuffer);
    OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, mParticleColorBuffer);
    OPENGLFUNC->glBufferData(GL_ARRAY_BUFFER, mParameter.maxParticleNum * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
}

void ParticleWidget::play()
{
    mIsPlaying = true;
    mFromStartTime.start();
}

void ParticleWidget::stop() {
    mIsPlaying = false;
}

void ParticleWidget::reset(bool replay) {
    if(replay) stop();
    resetGL();
    resetParameters();
    if(replay) play();
}

void ParticleWidget::resetGL() {
    if(cParent->isGLReady()) {
        OPENGLFUNC->glDeleteBuffers(1, &mParticleVertexBuffer);
        OPENGLFUNC->glDeleteBuffers(1, &mParticlePosSizeBuffer);
        OPENGLFUNC->glDeleteBuffers(1, &mParticleColorBuffer);
        OPENGLFUNC->glDeleteVertexArrays(1, &mVertexArrayID);

        initGLBuffer();
    }
}

void ParticleWidget::resetParameters() {
    reGenerateSizeCurve();
}

bool ParticleWidget::isPlaying() {
    return mIsPlaying;
}

void ParticleWidget::render() {
    if(mIsPlaying) {
        updateParticles();

        OPENGLFUNC->glUseProgram(mProgram.programId());
        OPENGLFUNC->glActiveTexture(GL_TEXTURE0);
        OPENGLFUNC->glUniform1i(mTextureUniformID, 0);

        if(mParameter.colorCustom) {
            OPENGLFUNC->glBindTexture(GL_TEXTURE_2D, mTextureLID);
        }
        else {
            OPENGLFUNC->glBindTexture(GL_TEXTURE_2D, mTextureID);
        }

        OPENGLFUNC->glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, (GLfloat*)cParent->getActiveCamera().getProjectionMatrix().data());

        OPENGLFUNC->glUniform3fv(mUpAxisID, 1, (GLfloat*)(cParent->getActiveCamera().getCameraRotation().data()+4));
        OPENGLFUNC->glUniform3fv(mRightAxisID, 1, (GLfloat*)(cParent->getActiveCamera().getCameraRotation()).data());

        OPENGLFUNC->glEnableVertexAttribArray(0);
        OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, mParticleVertexBuffer);
        OPENGLFUNC->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        OPENGLFUNC->glEnableVertexAttribArray(1);
        OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, mParticlePosSizeBuffer);
        OPENGLFUNC->glBufferData(GL_ARRAY_BUFFER, mParameter.maxParticleNum * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        OPENGLFUNC->glBufferSubData(GL_ARRAY_BUFFER, 0, mCurrentParticleNum * sizeof(GLfloat) * 4, mParticlePosSizeData);
        OPENGLFUNC->glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        OPENGLFUNC->glEnableVertexAttribArray(2);
        OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, mParticleColorBuffer);
        OPENGLFUNC->glBufferData(GL_ARRAY_BUFFER, mParameter.maxParticleNum * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        OPENGLFUNC->glBufferSubData(GL_ARRAY_BUFFER, 0, mCurrentParticleNum * sizeof(GLfloat) * 4 , mParticleColorData);
        OPENGLFUNC->glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        OPENGLFUNC->glVertexAttribDivisor(0, 0);
        OPENGLFUNC->glVertexAttribDivisor(1, 1);
        OPENGLFUNC->glVertexAttribDivisor(2, 1);

        OPENGLFUNC->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mCurrentParticleNum);

        OPENGLFUNC->glDisableVertexAttribArray(0);
        OPENGLFUNC->glDisableVertexAttribArray(1);
        OPENGLFUNC->glDisableVertexAttribArray(2);
    }
}

void ParticleWidget::initShader()
{

    // Compile vertex shader
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "../shaders/ParticleVertex.glsl"))
        cParent->close();

    // Compile fragment shader
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "../shaders/ParticleFragment.glsl"))
        cParent->close();

    // Link shader pipeline
    if (!mProgram.link())
        cParent->close();

    // Bind shader pipeline for use
    if (!mProgram.bind())
        cParent->close();
}

void ParticleWidget::initTextures()
{
    mTextureUniformID = OPENGLFUNC->glGetUniformLocation(mProgram.programId(), "textureSampler");

    // Load texture image

    mTextureImage = new QImage();
    if(!mTextureImage->load(mParameter.texturePath)) {
        qDebug() << "Generate fake texture";
        delete mTextureImage;
        mTextureImage = new QImage(100, 100, QImage::Format_ARGB32);
        mTextureImage->fill(QColor(Qt::white).rgba());
    }
    else
    {
        *mTextureImage = mTextureImage->scaled(100, 100, Qt::IgnoreAspectRatio);
    }

    mTextureLImage = new QImage(100, 100, QImage::Format_ARGB32);
    RGBtoLuv(*mTextureImage, *mTextureLImage);

    OPENGLFUNC->glGenTextures(1, &mTextureID);
    OPENGLFUNC->glBindTexture(GL_TEXTURE_2D, mTextureID);
    OPENGLFUNC->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureImage->width(), mTextureImage->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)mTextureImage->bits());
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    OPENGLFUNC->glGenerateMipmap(GL_TEXTURE_2D);

    OPENGLFUNC->glGenTextures(1, &mTextureLID);
    OPENGLFUNC->glBindTexture(GL_TEXTURE_2D, mTextureLID);
    OPENGLFUNC->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureLImage->width(), mTextureLImage->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)mTextureLImage->bits());
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    OPENGLFUNC->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    OPENGLFUNC->glGenerateMipmap(GL_TEXTURE_2D);

    qDebug() << "Texture initialized!";
}

void ParticleWidget::updateParticles()
{
    int elapsedMsec = mFromStartTime.elapsed();
    float sec = elapsedMsec / 1000.0f;

    if(mParameter.emissionRate < 1000/60+1) {
        if(mFromStartTime.elapsed() > 1000) {
            mFromStartTime.restart();
            mNeedEmit = true;
        }
    }
    else {
        mFromStartTime.restart();
    }

    mTurbu->update(sec);
    
    unsigned constraint = mCurrentParticleNum;
    unsigned particlePointer = 0;
    
    while(constraint > 0) {
        Particle& p = mParticleContainer[particlePointer];
        if(!p.checkLife(sec)) {
            mCurrentParticleNum --;
        }
        else {
            genPhysicalForce(sec, p);
            if(mParameter.colorOverLife) changeColorOverTime(p);
        }
        particlePointer ++;
        constraint --;
    }

    sortParticle();

    unsigned newParticlesNum = 0;

    if(mParameter.emissionRate > 1000/16+1) {
        newParticlesNum = elapsedMsec > 16 ? floor((16/1000.0f) * (float)mParameter.emissionRate) : floor((elapsedMsec/1000.0f) * (float)mParameter.emissionRate);
        //Limit number of new generated particles to 16 * rate/msec in a frame(updated every 16 mesc, based on 60fps)
        //Avoid too many new particles being generated due to unexpected long frame to make next frame longer
    }
    else if(mNeedEmit) {
        newParticlesNum = mParameter.emissionRate;
        mNeedEmit = false;
    }

    if(mCurrentParticleNum + newParticlesNum > mParameter.maxParticleNum){
        newParticlesNum = mParameter.maxParticleNum - mCurrentParticleNum;
    }

    constraint = newParticlesNum;
    particlePointer = mCurrentParticleNum;

    QColor initColor;
    if(mParameter.colorCustom) {
        if(mParameter.colorOverLife) initColor = mGradient.getColor(0.0f);
        else initColor = mColor;
    }
    else initColor = Qt::white;

    while(constraint > 0) {
        Particle& p = mParticleContainer[particlePointer];

        p.life = mParameter.startLifeTime; // This particle will live seconds.
        genStartPos(p);

        p.r = initColor.red();
        p.g = initColor.green();
        p.b = initColor.blue();
        p.a = initColor.alpha();

        p.size = mParameter.startSize;

        particlePointer++;
        constraint --;
    }

    sortParticle();
    mCurrentParticleNum += newParticlesNum;

    for(int i = 0; i < mCurrentParticleNum; i++) {
    //to iterate every particle***************************************************
        Particle& p = mParticleContainer[i];
        mParticlePosSizeData[4*i+0] = p.pos.x();
        mParticlePosSizeData[4*i+1] = p.pos.y();
        mParticlePosSizeData[4*i+2] = p.pos.z();
        mParticlePosSizeData[4*i+3] = p.size;

        //update size
        float ela = mParameter.startLifeTime - p.life;
        int elt = (int)floor((ela) * (float)60);
        if(elt >= mSizeCurve.size()) elt = mSizeCurve.size()-1;
        mParticlePosSizeData[4*i+3] = p.size * mSizeCurve[elt].second;

        //update color
        mParticleColorData[4*i+0] = (float)p.r / 255.0f;
        mParticleColorData[4*i+1] = (float)p.g / 255.0f;
        mParticleColorData[4*i+2] = (float)p.b / 255.0f;
        mParticleColorData[4*i+3] = (float)p.a / 255.0f;
    //****************************************************************************
    }
}

void ParticleWidget::sortParticle() {
    std::sort(&mParticleContainer[0], &mParticleContainer[mParameter.maxParticleNum]);
}

void ParticleWidget::setEmitterPosition(QVector3D input) {
    mEmitterPosition = input;
}

void ParticleWidget::setEmitterShape(const EmitterShape& shape) {
    mShape = shape;
}

void ParticleWidget::setEnviromentPhysic(const Physic& enviroment) {
    mParticlePhysic = enviroment;
}

void ParticleWidget::setEmitParameter(const EmitParameter& parameter) {

    if(mParameter.maxParticleNum != parameter.maxParticleNum) {
        delete [] mParticleContainer;
        delete [] mParticlePosSizeData;
        delete [] mParticleColorData;
        mParticleContainer = new Particle[parameter.maxParticleNum];
        mParticlePosSizeData = new GLfloat[parameter.maxParticleNum * 4];
        mParticleColorData = new GLfloat[parameter.maxParticleNum * 4];
        mCurrentParticleNum = 0;
    }

    mParameter = parameter;

    reset(isPlaying());
}

void ParticleWidget::genStartPos(Particle& p) {
    if(mShape.type == CONE) {
        QVector2D twoD = genRandom2D(mShape.radius) * mShape.scale;
        QVector3D threeD(mEmitterPosition.x() + twoD.x(), mEmitterPosition.y(), mEmitterPosition.z() + twoD.y());
        p.pos = threeD;
        QVector3D temp = genRandomDir3D(mShape.angle) * mParameter.startSpeed;
        p.speed = temp;
        p.cameraDistance = (p.pos - cParent->getActiveCamera().getCameraPosition()).length();
    }
    else{
        //...
    }
    return;
}

void ParticleWidget::genPhysicalForce(float sec, Particle& p) {
    QVector3D force(0.0f, 0.0f, 0.0f);
    force += Gravity * mParticlePhysic.gravityModifier + mTurbu->calculate(p);
    p.speed += force * sec * 0.5; //assume mass of particle is 1 and the force is constant.
    p.speed *= mParticlePhysic.damping;
    p.speed = p.speed.length() > mParticlePhysic.terminalSpeed ? p.speed.normalized() * mParticlePhysic.terminalSpeed : p.speed;
    p.pos += p.speed * sec; //displacement equal to average speed multiply duration.
    p.cameraDistance = (p.pos - cParent->getActiveCamera().getCameraPosition()).length();
}

void ParticleWidget::changeColorOverTime(Particle& p) {
    float t = (mParameter.startLifeTime - p.life) / mParameter.startLifeTime;
    QColor temp = mGradient.getColor(t);
    p.r = temp.red();
    p.g = temp.green();
    p.b = temp.blue();
    p.a = temp.alpha();
}

void ParticleWidget::reGenerateSizeCurve(void) {
    aaAaa::aaCurvePtr ct = aaAaa::aaCurveFactory::createCurve(mSizeSpline);
    mSizeCurve.clear();
    ct->getValueList(mSizeCurve, mParameter.startLifeTime * 60, true);
}

aaAaa::aaSpline* ParticleWidget::getSizeSpline() {
    return &mSizeSpline;
}

const EmitterShape& ParticleWidget::getShape() {
    return mShape;
}

const EmitParameter& ParticleWidget::getParameter() {
    return mParameter;
}

const Physic& ParticleWidget::getPhysic() {
    return mParticlePhysic;
}

const QColor& ParticleWidget::getColor() {
    return mColor;
}
const GradientDescriber& ParticleWidget::getGradient() {
    return mGradient;
}

void ParticleWidget::setGradient(const GradientDescriber& gradient) {
    mGradient = gradient;
}

void ParticleWidget::setColor(const QColor& color) {
    mColor = color;
}

Turbulence* ParticleWidget::getTurbulenceField(){
    return mTurbu;
}

const QVector3D ParticleWidget::Gravity = QVector3D(0.0f, -9.8f, 0.0f);
