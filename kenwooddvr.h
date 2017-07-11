#ifndef KENWOODDVR_H
#define KENWOODDVR_H

#include <QObject>
#include <console.h>
#include <dvrprotocol.h>

class Console;
class DvrProtocol;

class KenwoodDvr : public QObject
{
    Q_OBJECT
public:
    explicit KenwoodDvr(QObject *parent = 0);
    void putData(int port, const QByteArray &data, Console *console);

signals:

public slots:

private:

};

#endif // KENWOODDVR_H
