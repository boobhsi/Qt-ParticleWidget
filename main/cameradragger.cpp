#include "cameradragger.h"

CameraDragger::CameraDragger(QWidget *parent) : QWidget(parent)
{
    mState = NOINPUT;
    mNowControll = 0;
    setFocusPolicy( Qt::StrongFocus );
}

void CameraDragger::setControlled(CompositionWidget* input) {
    mNowControll = input;
}

void CameraDragger::mousePressEvent(QMouseEvent *event)
{
    Q_ASSERT(mNowControll != 0);

    qDebug() << "mousePressed!";
    if(mState == NOINPUT){
        if(event->button() == Qt::LeftButton) {
            mState = LEFT;
        }
        else if(event->button() == Qt::MidButton) {
            mState = MID;
        }
        else if(event->button() == Qt::RightButton) {
            mState = RIGHT;
        }
        if(mState != NOINPUT) {
            mousePos = event->pos();
        }
    }
    else return;
}

void CameraDragger::mouseReleaseEvent(QMouseEvent *event)
{
    if(mState == LEFT && event->button() == Qt::LeftButton) {
        mState = NOINPUT;
    }
    else if(mState == MID && event->button() == Qt::MidButton) {
        mState = NOINPUT;
    }
    else if(mState == RIGHT && event->button() == Qt::RightButton) {
        mState = NOINPUT;
    }
    return;
}

void CameraDragger::mouseMoveEvent(QMouseEvent *event)
{
    if(mState == NOINPUT) return;

    qDebug() << "mouse dragging";
    QPoint temp = event->pos() - mousePos;
    mousePos = event->pos();
    displacement.setX(temp.x());
    displacement.setY(-temp.y());
    controllCamera();
}

void CameraDragger::controllCamera()
{
    Camera* cm = &mNowControll->activeCamera;
    switch(mState) {
    case LEFT:{
        QVector3D temp(displacement.y() / 5, -displacement.x() / 5, 0.0f);
        cm->rotateCamera(temp);
        break;
    }
    case RIGHT:{
        cm->forwardCamera(-displacement.y() / 5);
        break;
    }
    case MID:{
        QVector3D temp(displacement.x() / 5, displacement.y() / 5, 0.0f);
        cm->translateCamera(temp);
        break;
    }
    case NOINPUT:
        break;
    default:
        break;
    }
    return;
}
