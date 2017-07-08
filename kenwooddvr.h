#ifndef KENWOODDVR_H
#define KENWOODDVR_H

#include <QObject>
#include <console.h>

class Console;

class KenwoodDvr : public QObject
{
    Q_OBJECT
public:
    explicit KenwoodDvr(QObject *parent = 0);
    void putData(int port, const QByteArray &data, Console *console);

signals:

public slots:

};

#endif // KENWOODDVR_H
