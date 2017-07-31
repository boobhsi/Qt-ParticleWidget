#include "particlewidget.h"

ParticleWidget::ParticleWidget(QWidget* parent, unsigned pnInput, float scaleInput) :
    QOpenGLWidget(parent),
    mMaxParticleNum(pnInput),
    mCurrentParticleNum(0),
    mIsPlaying(false),
    mParticleScale(scaleInput)
{
    mParticleContainer = new Particle[mMaxParticleNum];
    mParticlePosSizeData = new GLfloat[mMaxParticleNum * 4];
    mParticleColorData = new GLubyte[mMaxParticleNum * 4];
    for(unsigned i=0;i<mMaxParticleNum;i++) {
        mParticleContainer[i].life = -1.0;
        mParticleContainer[i].cameraDistance = -1.0f;
    }
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(17); //60fps
}

ParticleWidget::~ParticleWidget()
{
    delete mTexture;
    delete [] mParticleContainer;
    delete [] mParticlePosSizeData;
    delete [] mParticleColorData;
    delete timer;
}

void ParticleWidget::initializeGL()
{
    //Initialize QOpenGL(Extra)Functions for current context
    initializeOpenGLFunctions();
    
    //Set background to black
    glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

    qDebug() << "Initialize shaders";
    initShaders();
    qDebug() << "Initialize texture";
    initTextures();

    mProjectionMatrixID = glGetUniformLocation(mProgram.programId(), "mvp");
    mTextureID = glGetUniformLocation(mProgram.programId(), "textureSampler");
    
    //Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glGenVertexArrays(1, &mVertexArrayID);
    glBindVertexArray(mVertexArrayID);
    
    glGenBuffers(1, &mParticleVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mParticleVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mParticleVertexData), mParticleVertexData, GL_STATIC_DRAW);
    
    glGenBuffers(1, &mParticlePosSizeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mParticlePosSizeBuffer);
    glBufferData(GL_ARRAY_BUFFER, mMaxParticleNum * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    
    glGenBuffers(1, &mParticleColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mParticleColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, mMaxParticleNum * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

    qDebug() << "Initialized!";
}

void ParticleWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    aspect = float(w) / float(h ? h : 1);

    calculateProjectionMatrix();
}

void ParticleWidget::calculateProjectionMatrix() {
    // Reset projection
    mProjectionMatrix.setToIdentity();

    // Set perspective projection
    mProjectionMatrix.perspective(fov, aspect, zNear, zFar);
    //mProjectionMatrix.ortho(-10, 10, -10, 10, zNear, zFar);

    qDebug() << mProjectionMatrix;

    QMatrix4x4 matrix;
    matrix.translate(-mCameraPosition);

    qDebug() << matrix;

    mProjectionMatrix = mProjectionMatrix * matrix;

    qDebug() << mProjectionMatrix;
}

void ParticleWidget::stop() {
    glDeleteBuffers(1, &mParticleVertexBuffer);
    glDeleteBuffers(1, &mParticlePosSizeBuffer);
    glDeleteBuffers(1, &mParticleColorBuffer);
    glDeleteProgram(mProgram.programId());
    //glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &mVertexArrayID);

    mIsPlaying = false;
}

void ParticleWidget::paintGL()
{
    if(mIsPlaying) {
        updateParticles();

        qDebug() << "painting";
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(mProgram.programId());

        glActiveTexture(GL_TEXTURE0);
        mTexture->bind();
        glUniform1i(mTextureID, 0);

        glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, (GLfloat*)mProjectionMatrix.data());

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, mParticleVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mParticlePosSizeBuffer);
        glBufferData(GL_ARRAY_BUFFER, mMaxParticleNum * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, mCurrentParticleNum * sizeof(GLfloat) * 4, mParticlePosSizeData);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, mParticleColorBuffer);
        glBufferData(GL_ARRAY_BUFFER, mMaxParticleNum * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, mCurrentParticleNum * sizeof(GLubyte) * 4 , mParticleColorData);
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glVertexAttribDivisor(0, 0);
        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mCurrentParticleNum);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
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
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/ParticleVertex.glsl"))
        close();

    // Compile fragment shader
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/ParticleFragment.glsl"))
        close();

    // Link shader pipeline
    if (!mProgram.link())
        close();

    // Bind shader pipeline for use
    if (!mProgram.bind())
        close();

    qDebug() << "Shaders initialized!";
}

void ParticleWidget::initTextures()
{
    // Load texture image

    QImage textureImage;
    if(!textureImage.load("")) {
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
        if(p.life > 0.0f) {
            p.speed += QVector3D(0.0f, -9.8f, 0.0f) * sec * 0.5f;
            p.pos += p.speed * sec;
            p.cameraDistance = (p.pos - mCameraPosition).length();

            mParticlePosSizeData[4*particlePointer+0] = p.pos.x();
            mParticlePosSizeData[4*particlePointer+1] = p.pos.y();
            mParticlePosSizeData[4*particlePointer+2] = p.pos.z();

            mParticlePosSizeData[4*particlePointer+3] = p.size;

            mParticleColorData[4*particlePointer+0] = p.r;
            mParticleColorData[4*particlePointer+1] = p.g;
            mParticleColorData[4*particlePointer+2] = p.b;
            mParticleColorData[4*particlePointer+3] = p.a;
        }
        else {
            p.cameraDistance = -1.0f;
            p.life = -1.0f;
            mCurrentParticleNum --;
        }
        particlePointer ++;
        constraint --;
    }

    sortParticle();

    unsigned newParticlesNum = elapsedMsec > 16 ? 16 * 20 : elapsedMsec * 20;
    //Limit number of new generated particles to 160 in a frame(updated every 16 mesc, based on 60fps)
    //Avoid too many new particles being generated due to unexpected long frame to make next frame longer
    if(mCurrentParticleNum + newParticlesNum > mMaxParticleNum){
        newParticlesNum = mMaxParticleNum - mCurrentParticleNum;
    }

    qDebug() << "New Particles Num =" <<newParticlesNum;

    constraint = newParticlesNum;
    particlePointer = mCurrentParticleNum;

    while(constraint > 0) {
        Particle& p = mParticleContainer[particlePointer];

        p.life = 8.0f; // This particle will live 5 seconds.
        p.pos = mEmitterPosition;
        float spread = 1.5f;
        QVector3D maindir = QVector3D(0.0f, 10.0f, 0.0f);
        // Very bad way to generate a random direction;
        // See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
        // combined with some user-controlled parameters (main direction, spread, etc)
        QVector3D randomdir = QVector3D(
                    (qrand()%2000 - 1000.0f)/1000.0f,
                    (qrand()%2000 - 1000.0f)/1000.0f,
                    (qrand()%2000 - 1000.0f)/1000.0f
                    );

        p.speed = maindir + randomdir * spread;

        // Very bad way to generate a random color
        p.r = qrand() % 256;
        p.g = qrand() % 256;
        p.b = qrand() % 256;
        p.a = (qrand() % 256) / 3;

        p.size = ((qrand()%1000)/2000.0f + 0.1f) * mParticleScale;
        //p.size = 1;

        particlePointer++;
        constraint --;
    }

    mCurrentParticleNum += newParticlesNum;
}

void ParticleWidget::sortParticle() {
    std::sort(&mParticleContainer[0], &mParticleContainer[mMaxParticleNum]);
}

void ParticleWidget::setEmitterPosition(QVector3D input) {
    mEmitterPosition = input;
}
void ParticleWidget::setCameraPosition(QVector3D input) {
    mCameraPosition = input;
    calculateProjectionMatrix();
}
