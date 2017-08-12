#include "colornotifier.h"
#include <iostream>
#include <QtSerialPort/QSerialPort>
#include <thread>
#include <chrono>

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


SerialNotifier::SerialNotifier(ColorNotifier * notifier, const QString & port) :
    notifier(notifier), port(port) {}

void SerialNotifier::run()
{
    QSerialPort serialport(port);

    serialport.setBaudRate(QSerialPort::Baud115200);
    serialport.setDataBits(QSerialPort::Data8);
    serialport.setParity(QSerialPort::NoParity);
    serialport.setStopBits(QSerialPort::OneStop);
    serialport.setFlowControl(QSerialPort::NoFlowControl);

    while(true){
        if(serialport.open(QIODevice::Unbuffered|QIODevice::ReadWrite)){
            while(serialport.isOpen()){
            serialport.write(this->makeCmd());
            std::this_thread::sleep_for(std::chrono_literals::operator""ms(1));
            }
            serialport.close();
        }
        std::this_thread::sleep_for(std::chrono_literals::operator""ms(500));
    }
}

QByteArray SerialNotifier::makeCmd()
{
    QString out("$SD");
    out += ":";
    out += notifier->getColor().green();
    out += ":";
    out += notifier->getColor().red();
    out += ":";
    out += notifier->getColor().blue();
    out += "\r\n";

    return QByteArray(out.toUtf8());
}
