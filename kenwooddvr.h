#ifndef KENWOODDVR_H
#define KENWOODDVR_H

#include <QObject>
#include <console.h>
#include <dvrprotocol.h>


class Console;
class DvrProtocol;

class KenwoodDvr
{


public:
    KenwoodDvr(QWidget *parent = 0);
    void putData(int port, const QByteArray &data, Console *console);
    void sendData(QByteArray &data);

signals:

public slots:

private:

};

#endif // KENWOODDVR_H
