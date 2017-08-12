#ifndef COLORNOTIFIER_H
#define COLORNOTIFIER_H

#include <QObject>
#include <QColor>
#include <QMutex>
#include <QString>
#include <QByteArray>

class ColorNotifier : public QObject
{
    Q_OBJECT
public:
    explicit ColorNotifier(QObject *parent = nullptr);

public slots:
    void setColor(const QColor & color);
    void setBack (const QColor & color);

public:
    QColor getColor();
    QColor getBack();

private:
    QColor textColor, backColor;
    QMutex textColMutex, backColMutex;

};


class SerialNotifier
{
public:
    SerialNotifier(ColorNotifier * notifier, const QString & port);
    void run();

private:
    QByteArray makeCmd();

    ColorNotifier * notifier;
    QString port;
};



#endif // COLORNOTIFIER_H
