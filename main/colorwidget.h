#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QColorDialog>
#include <QBrush>
#include <QFrame>
#include <QLabel>
#include "gradientdescriber.h"

class ColorWidget : public QFrame
{
    Q_OBJECT
    
public:
    explicit ColorWidget(QWidget *parent = Q_NULLPTR);
    virtual const QColor& getColor();
    virtual ~ColorWidget();

protected:
    virtual void mousePressEvent(QMouseEvent* event);  
    QBrush* mBrusher;
    QPalette* mPalette;
    QLabel* mUpper;

    virtual void drawPalette();

signals:
    void colorChange();
public slots:

private:
    QColor mCurrent;
};

class GradientWidget : public ColorWidget
{
    Q_OBJECT

public:
    explicit GradientWidget(QWidget *parent = Q_NULLPTR);
    ~GradientWidget();
    const GradientDescriber& getGradient();
    const QColor& getColor() override;

signals:
    void gradientChange();

protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void drawPalette();

private:
    GradientDescriber* mCurrent;
};


#endif // COLORWIDGET_H
