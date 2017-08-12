#include "colornotifier.h"
#include <QDebug>

ColorNotifier::ColorNotifier(QObject *parent) : QObject(parent)
{

}

void ColorNotifier::setColor(QColor color)
{
    qDebug() << "col: " << color.red() << ":" << color.green() << ":" << color.blue();
}

void ColorNotifier::setBack(QColor color)
{
    qDebug() << "bck: " << color.red() << ":" << color.green() << ":" << color.blue();
}
