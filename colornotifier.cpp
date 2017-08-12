#include "colornotifier.h"
#include <iostream>

ColorNotifier::ColorNotifier(QObject *parent) : QObject(parent)
{

}

void ColorNotifier::setColor(const QColor & color)
{
    std::cout << "col: " << color.red() << ":" << color.green() << ":" << color.blue() << std::endl;
}

void ColorNotifier::setBack(const QColor & color)
{
    std::cout << "bck: " << color.red() << ":" << color.green() << ":" << color.blue() << std::endl;
}
