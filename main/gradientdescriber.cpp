#include "gradientdescriber.h"

GradientDescriber::GradientDescriber(QColor initHead, QColor initTail)
{
    mNodeList.push_back(ColorNode(0.0f, initHead));
    mNodeList.push_back(ColorNode(1.0f, initTail));
}

QColor GradientDescriber::getColor(float t) {
    int indexr = 0, indexl = 0;
    for(int i=0; i<mNodeList.size(); i++) {
        if(t > mNodeList[i].first) indexr++;
    }
    if(indexr == mNodeList.size()) indexr = indexl = mNodeList.size() - 1;
    else if(indexr == 0) indexl = 0;
    else indexl = indexr - 1;
    
    if(indexl == indexr) {
        return mNodeList[indexl].second;
    }
    else {
        ColorNode& l = mNodeList[indexl];
        ColorNode& r = mNodeList[indexr];
        float temp = (t - l.first) / (r.first - l.first);
        QColor tempC(
                    (r.second.red() - l.second.red()) * temp + l.second.red(),
                    (r.second.green() - l.second.green()) * temp + l.second.green(),
                    (r.second.blue() - l.second.blue()) * temp + l.second.blue(),
                    (r.second.alpha() - l.second.alpha()) * temp + l.second.alpha()
                    );
        return tempC;
    }
}

void GradientDescriber::insertColor(const QColor& iColor, float t) {
    std::vector<ColorNode>::iterator it;
    ColorNode temp(t, iColor);

    for(it=mNodeList.begin(); it != mNodeList.end(); it++) {
        if(t < (*it).first) {
            mNodeList.insert(it, temp);
            return;
        }
        mNodeList.push_back(temp);
    }
}
