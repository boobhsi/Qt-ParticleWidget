#include "particlewidget.h"

#define OPENGLFUNC parent->context()->extraFunctions()

ParticleWidget::ParticleWidget(CompositionWidget *parent) :
    Solid(parent)
{
    mParticleContainer = new Particle[mParameter.maxParticleNum];
    mParticlePosSizeData = new GLfloat[mParameter.maxParticleNum * 4];
    mParticleColorData = new GLubyte[mParameter.maxParticleNum * 4];

    mIsPlaying = false;
    mCurrentParticleNum = 0;
}

ParticleWidget::~ParticleWidget()
{
    delete mTexture;
    delete [] mParticleContainer;
    delete [] mParticlePosSizeData;
    delete [] mParticleColorData;
}

void ParticleWidget::initializeGL()
{
    qDebug() << "Initialize shaders";
    initShaders();
    qDebug() << "Initialize texture";
    initTextures();

    mProjectionMatrixID = OPENGLFUNC->glGetUniformLocation(mProgram.programId(), "mvp");
    mTextureID = OPENGLFUNC->glGetUniformLocation(mProgram.programId(), "textureSampler");

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
    OPENGLFUNC->glBufferData(GL_ARRAY_BUFFER, mParameter.maxParticleNum * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

    qDebug() << "Particle Initialized!";
}

void ParticleWidget::stop() {
    OPENGLFUNC->glDeleteBuffers(1, &mParticleVertexBuffer);
    OPENGLFUNC->glDeleteBuffers(1, &mParticlePosSizeBuffer);
    OPENGLFUNC->glDeleteBuffers(1, &mParticleColorBuffer);
    OPENGLFUNC->glDeleteProgram(mProgram.programId());
    OPENGLFUNC->glDeleteVertexArrays(1, &mVertexArrayID);
    mIsPlaying = false;
}

void ParticleWidget::render()
{
    if(mIsPlaying) {
        updateParticles();

        OPENGLFUNC->glUseProgram(mProgram.programId());

        OPENGLFUNC->glActiveTexture(GL_TEXTURE0);
        mTexture->bind();
        OPENGLFUNC->glUniform1i(mTextureID, 0);

        OPENGLFUNC->glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, (GLfloat*)parent->getActiveCamera().getProjectionMatrix().data());

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
        OPENGLFUNC->glBufferSubData(GL_ARRAY_BUFFER, 0, mCurrentParticleNum * sizeof(GLubyte) * 4 , mParticleColorData);
        OPENGLFUNC->glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

        OPENGLFUNC->glVertexAttribDivisor(0, 0);
        OPENGLFUNC->glVertexAttribDivisor(1, 1);
        OPENGLFUNC->glVertexAttribDivisor(2, 1);

        OPENGLFUNC->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mCurrentParticleNum);

        OPENGLFUNC->glDisableVertexAttribArray(0);
        OPENGLFUNC->glDisableVertexAttribArray(1);
        OPENGLFUNC->glDisableVertexAttribArray(2);
    }
    return;
}

void ParticleWidget::play()
{
    mFromStartTime.start();
    mIsPlaying = true;
}

void ParticleWidget::initShaders()
{

    // Compile vertex shader
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "../shaders/ParticleVertex.glsl"))
        parent->close();

    // Compile fragment shader
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "../shaders/ParticleFragment.glsl"))
        parent->close();

    // Link shader pipeline
    if (!mProgram.link())
        parent->close();

    // Bind shader pipeline for use
    if (!mProgram.bind())
        parent->close();

    qDebug() << "Shaders initialized!";
}

void ParticleWidget::initTextures()
{
    // Load texture image

    QImage textureImage;
    if(!textureImage.load(mParameter.texturePath)) {
        qDebug() << "Generate fake texture";
        textureImage = QImage(100, 100, QImage::Format_ARGB32);
        textureImage.fill(QColor(Qt::white).rgba());
    }
    else
    {
        textureImage = textureImage.scaled(100, 100, Qt::IgnoreAspectRatio);
    }

    mTexture = new QOpenGLTexture(textureImage.mirrored());

    // Set nearest filtering mode for texture minification
    mTexture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    mTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    mTexture->setWrapMode(QOpenGLTexture::Repeat);

    qDebug() << "Texture initialized!";
}

void ParticleWidget::updateParticles()
{
    int elapsedMsec = mFromStartTime.elapsed();
    mFromStartTime.restart();
    
    unsigned constraint = mCurrentParticleNum;
    unsigned particlePointer = 0;
    
    while(constraint > 0) {
        Particle& p = mParticleContainer[particlePointer];
        float sec = elapsedMsec / 1000.0f;
        p.life -= sec;
        //qDebug() << p.life;
        if(!p.checkLife(sec)) {
            mCurrentParticleNum --;
        }
        else {
            //calculate physical force
            genPhysicalForce(sec, p);
        }
        particlePointer ++;
        constraint --;
    }

    sortParticle();

    unsigned newParticlesNum = elapsedMsec > 16 ? floor((16/1000.0f) * (float)mParameter.emissionRate) : floor((elapsedMsec/1000.0f) * (float)mParameter.emissionRate);
    //Limit number of new generated particles to 160 in a frame(updated every 16 mesc, based on 60fps)
    //Avoid too many new particles being generated due to unexpected long frame to make next frame longer
    if(mCurrentParticleNum + newParticlesNum > mParameter.maxParticleNum){
        newParticlesNum = mParameter.maxParticleNum - mCurrentParticleNum;
    }

    constraint = newParticlesNum;
    particlePointer = mCurrentParticleNum;

    while(constraint > 0) {
        Particle& p = mParticleContainer[particlePointer];

        p.life = mParameter.startLifeTime; // This particle will live seconds.
        genStartPos(p);

        // Very bad way to generate a random color
        p.r = qrand() % 256;
        p.g = qrand() % 256;
        p.b = qrand() % 256;
        p.a = (qrand() % 256) / 3;

        p.size = ((qrand()%1000)/2000.0f + 0.1f) * mParameter.startSize;
        //p.size = 1;

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

        //update size
        mParticlePosSizeData[4*i+3] = p.size;

        //update color
        mParticleColorData[4*i+0] = p.r;
        mParticleColorData[4*i+1] = p.g;
        mParticleColorData[4*i+2] = p.b;
        mParticleColorData[4*i+3] = p.a;
    //****************************************************************************
    }
}

void ParticleWidget::sortParticle() {
    std::sort(&mParticleContainer[0], &mParticleContainer[mParameter.maxParticleNum]);
}

void ParticleWidget::setEmitterPosition(QVector3D input) {
    mEmitterPosition = input;
}

void ParticleWidget::setEmitterShape(EmitterShape& shape) {
    mShape = shape;
}

void ParticleWidget::setEnviromentPhysic(Physic& enviroment) {
    mParticlePhysic = enviroment;
}

void ParticleWidget::setEmitParameter(EmitParameter& parameter) {
    if(mParameter.maxParticleNum != parameter.maxParticleNum) {
        delete [] mParticleContainer;
        delete [] mParticlePosSizeData;
        delete [] mParticleColorData;
        mParticleContainer = new Particle[parameter.maxParticleNum];
        mParticlePosSizeData = new GLfloat[parameter.maxParticleNum * 4];
        mParticleColorData = new GLubyte[parameter.maxParticleNum * 4];
    }

    mParameter = parameter;
}

void ParticleWidget::genStartPos(Particle& p) {
    if(mShape.type == CONE) {
        QVector2D twoD = genRandom2D(mShape.radius);
        QVector3D threeD(twoD.x(), twoD.y(), mEmitterPosition.z());
        p.pos = threeD;
        p.speed = genRandomDir3D(mShape.angle) * mParameter.startSpeed;
        p.cameraDistance = (p.pos - parent->getActiveCamera().getCameraPosition()).length();
    }
    else{
        //...
    }
    return;
}

void ParticleWidget::genPhysicalForce(float sec, Particle& p) {
    QVector3D force(0.0f, 0.0f, 0.0f);
    force += Gravity * mParticlePhysic.gravityModifier;
    p.speed += force * sec * 0.5; //assume mass of particle is 1 and the force is constant.
    p.pos += p.speed * sec; //displacement equal to average speed multiply duration.
    p.cameraDistance = (p.pos - parent->getActiveCamera().getCameraPosition()).length();
}

const QVector3D ParticleWidget::Gravity = QVector3D(0.0f, -9.8f, 0.0f);
