#ifndef COLORLABEL_H
#define COLORLABEL_H

#include <QLabel>
#include <QMouseEvent>

class ColorLabel : public QFrame
{
    Q_OBJECT

public:
    ColorLabel(const QColor& iColor, float iTime, QWidget *parent = Q_NULLPTR);
    ~ColorLabel();

signals:
    void colorElementChange(const QColor& c, float t);
    void colorElementDelete(float t);


protected:
    virtual void mousePressEvent(QMouseEvent* event);

private:
    float mTime;
    QColor mColor;
};

#endif // COLORLABEL_H
