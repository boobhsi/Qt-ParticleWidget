#include "solid.h"

Solid::Solid(CompositionWidget *parent) : QObject(parent), glReady(false)
{
    cParent = parent;
}

Solid::~Solid()
{

}

void Solid::restart() {
    stop();
    reset(true);
}

void Solid::initializeGL() {
    initShader();;
    initTexture();
    initializeGLContent();
    initGLBuffer();
}

void Solid::close() {
    cParent->close();
}
