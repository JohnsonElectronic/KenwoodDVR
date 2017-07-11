#include "dvrprotocol.h"
#include <QObject>
#include <console.h>
#include <QtDebug>
#include <QMessageBox>
#include <QFile>
#include "dvrprotocol.h"

DvrProtocol::DvrProtocol()
{

}

void DvrProtocol::analysetData(const QByteArray &data, Console *console){
    switch (data[0]&0x000000ff){

    case 0x0c:
//[0c xx 03 xx]
        switch (data[2]&0x000000ff){

            case 0x01:
//            console->putText("[HAND SHAKE]");
//            console->putText(QString(data.toHex().data())+"\r\n");
            break;

            case 0x03:

                switch (data[3]&0x000000ff){
                    case 0x30:

                    break;
                case 0x13:
                    console->putText("[BUTTON PLY]");
                    console->putText(QString(data.toHex().data())+"\r\n");

                break;
                case 0x14:
                    console->putText("[BUTTON SET]");
                    console->putText(QString(data.toHex().data())+"\r\n");

                break;
                case 0x31:
                    console->putText("[BUTTON REC]");
                    console->putText(QString(data.toHex().data())+"\r\n");

                break;
                case 0x32:
                    console->putText("[BUTTON PIC]");
                    console->putText(QString(data.toHex().data())+"\r\n");

                break;
                default:
                    console->putText(QString(data.toHex().data())+"\r\n");

                    break;
                }

            break;
            case 0x04:
            console->putText("[PLAY CTRL]");
            console->putText(QString(data.toHex().data())+"\r\n");

            break;
            case 0x05:
            console->putText("[FILE CTRL]");
            console->putText(QString(data.toHex().data())+"\r\n");


            break;
            case 0x06:
            console->putText("[SET  CTRL]");
            console->putText(QString(data.toHex().data())+"\r\n");


            break;

            default:
                console->putText(QString(data.toHex().data())+"\r\n");
            break;
            }
        break;

    case 0xc0:
        switch (data[2]&0x000000ff){

            case 0x03:

                switch (data[3]&0x000000ff){
                    case 0x30:

                    break;
                default:

                    console->putText(QString(data.toHex().data())+"\r\n");
                break;
                }
            break;
        default:

            console->putText(QString(data.toHex().data())+"\r\n");
        break;

        }
        break;

    default:

        console->putText(QString(data.toHex().data())+"\r\n");
        break;




    }

}
