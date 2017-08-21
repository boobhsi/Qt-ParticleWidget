#ifndef SOLID_H
#define SOLID_H

#include "compositionwidget.h"
#include <QObject>

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
    virtual void initializeGL() = 0;

protected slots:
    void restart();

protected:
    CompositionWidget* cParent;

    bool glReady;
};

#endif // SOLID_H
