#ifndef CAMERADRAGGER_H
#define CAMERADRAGGER_H

#include <QWidget>
#include "compositionwidget.h"
#include <QMouseEvent>
#include "camera.h"

class CompositionWidget;

class CameraDragger : public QWidget
{
public:
    CameraDragger(QWidget* parent = Q_NULLPTR);
    void setControlled(CompositionWidget* input);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    enum MousePressed{LEFT, MID, RIGHT, NOINPUT};

    CompositionWidget* mNowControll;

    MousePressed mState;
    QPoint mousePos;
    QVector2D displacement;

    void controllCamera();
};

#endif // CAMERADRAGGER_H
