#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QColorDialog>
#include <QBrush>
#include <QFrame>
#include <QLabel>
#include <QMouseEvent>
#include "gradientdescriber.h"
#include "colorlabel.h"

class ColorWidget : public QFrame
{
    Q_OBJECT
    
public:
    explicit ColorWidget(QWidget *parent = Q_NULLPTR);
    virtual const QColor& getColor();
    virtual ~ColorWidget();

    void setColor(const QColor& iColor);

protected:
    virtual void mousePressEvent(QMouseEvent* event);

    //for transaparent icon
    QBrush* mBrusher;
    QPalette* mPalette;

    //
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

    void setGradient(const GradientDescriber ig);

signals:
    void gradientChange();

private slots:
    void onColorChange(const QColor& c, float t);
    void onColorDelete(float t);

protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void drawPalette();

private:
    typedef std::pair<float, ColorLabel*> Cl;

    GradientDescriber* mCurrent;
    void initGradientPalette();
    void addGradientLabel(float t, const QColor& color);
    std::map<float, ColorLabel*> mColorInterfaces;
};


#endif // COLORWIDGET_H
