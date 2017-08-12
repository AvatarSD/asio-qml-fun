#ifndef COLORNOTIFIER_H
#define COLORNOTIFIER_H

#include <QObject>
#include <QColor>

class ColorNotifier : public QObject
{
    Q_OBJECT
public:
    explicit ColorNotifier(QObject *parent = nullptr);

    Q_PROPERTY(QColor color)

signals:

public slots:
    void setColor(QColor color);
    void setBack(QColor color);

};

#endif // COLORNOTIFIER_H
