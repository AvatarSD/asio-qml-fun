#ifndef UIPROVIDER_H
#define UIPROVIDER_H

#include <QObject>

class UiProvider : public QObject
{
	Q_OBJECT
public:
	explicit UiProvider(QObject *parent = 0);

	void setBackgroundColor(const QColor&);


signals:

public slots:
};

#endif // UIPROVIDER_H
