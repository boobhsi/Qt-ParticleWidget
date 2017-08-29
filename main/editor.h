#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include "particlewidget.h"
#include "CurveEditor/curveeditor/graphiceditor.h"
#include "colorwidget.h"

class ParticleWidget;

namespace Ui {
class Editor;
}

class Editor : public QWidget
{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();

    void setControlled(ParticleWidget* toControll);

signals:
    void needRestart(void);

private slots:
    void onShowSizeClicked(void);
    void onEmitterShapeChange(void);
    void onParametersChange(void);
    void onEnvironmentChange(void);
    void onColorChange(void);
    void drawPalettePanel(int);
    void brushPaletteLabel(int);
    void onBlurChange(double);

private:
    Ui::Editor *ui;
    ParticleWidget* nowControll;
    graphiceditor* ge;
    QPainter* painter;

    void initUI();
    void setUItoInitControll();
    void connectChangeSignals();
};

#endif // EDITOR_H
