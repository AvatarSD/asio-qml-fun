#include "colornotifier.h"
#include <iostream>

ColorNotifier::ColorNotifier(QObject *parent) : QObject(parent),
    textColor("black"), backColor("black") {}

void ColorNotifier::setColor(const QColor & color)
{
    std::cout << "col: " <<
                 color.red() << ":" <<
                 color.green() << ":" <<
                 color.blue() << std::endl;
    QMutexLocker locker(&textColMutex);
    textColor = color;
}

void ColorNotifier::setBack(const QColor & color)
{
    std::cout << "bck: " <<
                 color.red() << ":" <<
                 color.green() << ":" <<
                 color.blue() << std::endl;
    QMutexLocker locker(&backColMutex);
    backColor = color;
}

QColor ColorNotifier::getColor()
{
    QMutexLocker locker(&textColMutex);
    return textColor;
}

QColor ColorNotifier::getBack()
{
    QMutexLocker locker(&backColMutex);
    return backColor;
}
