#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLContext>
#include "editor.h"
#include "particlewidget.h"
#include "compositionwidget.h"
#include "cameradragger.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    CameraDragger* ctrl;
    CompositionWidget* composition;
    Editor* comEditor;

    QLabel* testBackground;
};

#endif // MAINWINDOW_H
