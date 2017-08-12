#ifndef COLORNOTIFIER_H
#define COLORNOTIFIER_H

#include <QObject>
#include <QColor>

class ColorNotifier : public QObject
{
    Q_OBJECT
public:
    explicit ColorNotifier(QObject *parent = nullptr);

public slots:
    void setColor(const QColor & color);
    void setBack (const QColor & color);

};

#endif // COLORNOTIFIER_H
