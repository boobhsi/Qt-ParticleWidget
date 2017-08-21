#ifndef GRAPHICEDITOR_H
#define GRAPHICEDITOR_H

#include <QDialog>
#include "ui_graphiceditor.h"

#include "../libspline/aaCurve.h"
#include <vector>

class SplineDisplayerWidget;

//typedef std::vector<aaAaa::aaSpline> SplineVec;

class graphiceditor : public QDialog
{
    Q_OBJECT

public:
    graphiceditor(QWidget *parent = 0);
    ~graphiceditor();

    //void clearSplines();
    //void addSpline(const aaAaa::aaSpline &knot);
    void setSpline(aaAaa::aaSpline *spline);
    void addKnot(int index, const aaAaa::aaPoint &pt);
    void addKnot(int index, float t);

    //int getCurrentSplineIndex();

    const aaAaa::aaSpline &getSpline(void) const{
        return *m_spline;
    }

signals:
    void graphChange(void);

private:
    Ui::graphiceditorClass ui;

    SplineDisplayerWidget *m_viewer;

    aaAaa::aaSpline* m_spline;

private:
    void setupSpline();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    //void onCurveSelectChanged(int index);
    void onCurveTypeChanged(int index);
    void onSelectPosChanged(float t, float v);
};

#endif // GRAPHICEDITOR_H
