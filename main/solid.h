#ifndef SOLID_H
#define SOLID_H

#include "compositionwidget.h"
#include <QObject>

#define OPENGLFUNC this->cParent->context()->extraFunctions()

class CompositionWidget;

class Solid : public QObject
{
    Q_OBJECT
public:
    explicit Solid(CompositionWidget *parent = Q_NULLPTR);
    ~Solid();
    virtual void render() = 0;
    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void reset(bool replay) = 0;
    virtual bool isPlaying() = 0;
    void initializeGL();

protected slots:
    void restart();

protected:
    CompositionWidget* cParent;

    bool glReady;

    void close();

private:
    virtual void initializeGLContent() = 0;
    virtual void initShader() = 0;
    virtual void initGLBuffer() = 0;
};

#endif // SOLID_H
