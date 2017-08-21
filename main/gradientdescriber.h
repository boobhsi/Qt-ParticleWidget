#ifndef GRADIENTDESCRIBER_H
#define GRADIENTDESCRIBER_H

#include <QColor>
#include <vector>

class GradientDescriber
{
public:
    GradientDescriber(QColor initHead, QColor initTail);
    QColor getColor(float t);

    void insertColor(const QColor& iColor, float t);

private:
    typedef std::pair<float, QColor> ColorNode;

    std::vector<ColorNode> mNodeList;
};

#endif // GRADIENTDESCRIBER_H
