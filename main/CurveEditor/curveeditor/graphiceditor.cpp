#include "graphiceditor.h"
//#include "smGraphicsScene.h"
#include "GlobalData.h"
//#include "SplineCurveItem.h"
#include "SplineDisplayerWidget.h"
#include <QDebug>

#include <QVBoxLayout>

graphiceditor::graphiceditor(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    GlobalData::instance().addGe(this);

    for(int i=0; i<aaAaa::aaSpline::SPLINE_COUNT; ++i){
        ui.comboBox_type->addItem(aaAaa::aaSpline::spline_name[i].c_str());
    }
    
//    m_pgv = ui.graphicsView_spline;
//    setupScene();
    m_viewer = new SplineDisplayerWidget(0.1, ui.frame_spline);
    QVBoxLayout *layout = new QVBoxLayout;
    setFocusPolicy(Qt::ClickFocus);
    layout->addWidget(m_viewer);
    ui.frame_spline->setLayout(layout);

    

    //connect(ui.comboBox_item, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurveSelectChanged(int)));
    connect(ui.comboBox_type, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurveTypeChanged(int)));
    connect(m_viewer, SIGNAL(selectValuesChanged(float, float)), this, SLOT(onSelectPosChanged(float, float)));
    connect(m_viewer, SIGNAL(graphChange(void)), this, SIGNAL(graphChange(void)));
}

graphiceditor::~graphiceditor()
{

}
/*
int graphiceditor::getCurrentSplineIndex(){
    return ui.comboBox_item->currentIndex();
}
*/
void graphiceditor::setupSpline(){
    //ui.comboBox_item->clear();
    /*
    m_viewer->setAE(0);

    SplineVec &splines = m_splines;
    for(int i=0; i<splines.size(); ++i){
        splines[i].type = m_curve_type;
        //ui.comboBox_item->addItem(splines[i].name.c_str());
    }
*/

    m_viewer->setAE(m_spline);
    /*
    if(!splines.empty())
        m_viewer->setAE(&(splines[0]));
        */
}
/*
void graphiceditor::clearSplines(){
    m_splines.clear();
    //ui.comboBox_item->clear();
    m_viewer->setAE(0);
}
*/
void graphiceditor::setSpline(aaAaa::aaSpline* spline){
    m_spline = spline;
    setupSpline();
}
/*
void graphiceditor::addSpline(const aaAaa::aaSpline &knot){
    m_splines.push_back(knot);
    setupSplines();
}
*/
/*
void graphiceditor::(int index){
    if(index != -1){
        m_viewer->setAE(&(m_splines[index]));
    }
}
*/
void graphiceditor::onCurveTypeChanged(int index){
    if(index != -1){
        m_spline->type = index;
        m_viewer->update();
    }
}

void graphiceditor::onSelectPosChanged(float t, float v){
    ui.lineEdit_input->setText(QString("%1").arg(t));
    ui.lineEdit_output->setText(QString("%1").arg(v));
}

void graphiceditor::closeEvent(QCloseEvent *event){
    //parentWidget()->parentWidget()->activateWindow();
    //parentWidget()->parentWidget()->raise();
    //parentWidget()->parentWidget()->setFocus();
    GlobalData::instance().rmGe(this);
}

void graphiceditor::addKnot(int index, const aaAaa::aaPoint &pt){
    /*
    if(index < 0 || index >= m_splines.size())
        return ;
        */

    aaAaa::aaSpline &cur = *m_spline;
    cur.addKnots(pt);
    setupSpline();

    //ui.comboBox_item->setCurrentIndex(index);
}


void graphiceditor::addKnot(int index, float t){
    /*
    if(index < 0 || index >= m_splines.size())
        return ;
        */

    aaAaa::aaSpline &cur = *m_spline;
    aaAaa::aaCurvePtr curve = aaAaa::aaCurveFactory::createCurve(cur);

    if(!curve.get())
        return ;

    double v;
    curve->getValue(t, v);

    int selpoint = cur.addKnots(aaAaa::aaPoint(t, v));
    setupSpline();

    //ui.comboBox_item->setCurrentIndex(index);
    if(selpoint > 0)
        m_viewer->setCurrentSelected(selpoint);
}


