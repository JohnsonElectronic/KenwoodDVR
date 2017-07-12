#ifndef DVRPROTOCOL_H
#define DVRPROTOCOL_H

#include <QObject>
#include <console.h>
#include <kenwooddvr.h>

class DvrProtocol : public QObject
{
    Q_OBJECT

public:
    explicit DvrProtocol(QWidget *parent = 0);
    virtual ~DvrProtocol(){};
    void analysetData(const QByteArray &data, Console *console);

public slots:
    void TestHeartBeatFrame();

private:
void SendToDVR(QByteArray &data);


};

#endif // DVRPROTOCOL_H
