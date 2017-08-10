#ifndef SOLID_H
#define SOLID_H

#include "compositionwidget.h"

class CompositionWidget;

class Solid
{
public:
    Solid(CompositionWidget* parent);
    virtual ~Solid();
    virtual void render() = 0;
    virtual void initializeGL() = 0;
protected:
    CompositionWidget* parent;
};

#endif // SOLID_H
