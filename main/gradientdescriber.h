#ifndef GRADIENTDESCRIBER_H
#define GRADIENTDESCRIBER_H

#include <QColor>
#include <vector>

class GradientDescriber
{
public:
    typedef std::pair<float, QColor> ColorNode;

    GradientDescriber(QColor initHead, QColor initTail);
    QColor getColor(float t);

    void insertColor(const QColor& iColor, float t);
    bool deleteColor(float t);
    void changeColor(const QColor& iColor, float t);

    const std::vector<GradientDescriber::ColorNode>& getList();

private:
    std::vector<ColorNode> mNodeList;
};

#endif // GRADIENTDESCRIBER_H
