#ifndef DVRPROTOCOL_H
#define DVRPROTOCOL_H

#include <QObject>
#include <console.h>

class DvrProtocol
{
public:
    DvrProtocol();
    void analysetData(const QByteArray &data, Console *console);

};

#endif // DVRPROTOCOL_H
