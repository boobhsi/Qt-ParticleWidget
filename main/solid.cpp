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
