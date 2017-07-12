#include "dvrprotocol.h"
#include <QObject>
#include <console.h>
#include <QtDebug>
#include <QMessageBox>
#include <QFile>
#include <QTimer>

#include <dvrprotocol.h>
#include <kenwooddvr.h>

/*
DA->DVR
1. Frame format
    0x0C    LEN     CMD1    CMD2    ...     CHECKSUM    0xAA
2. CMD1 CMD2
   0x01 ****    Hand shake
   0x03 0x30    Heart beat, includ current date and timer
   0x03 0x13    Button FILE REPLAY
   0x03 0x14    Button SETUP
   0x03 0x31    Button MANUAL RECORD
   0x03 0x32    Button CAPTURE PICTURE
   0x04 ****    Replay Control
   0x05 ****    File Management Operation
   0x06 ****    Setup Operation


MARK:
   All 0x03 **** Commands include DATA-TIME info



DVR->DA
1. Frame format
    0xC0    LEN     CMD1    CMD2    ...     CHECKSUM    0xAA
2. CMD1 CMD2
   0x03 0x30    Heart beat, includ current date and timer




MARK:
1. LEN是包括0x0C 和 0xAA 的总长度
2. CHECKSUM = LEN + CMA1 + ... + DATAN

    0xC0   ( LEN     CMD1    CMD2    ... DATAN )   CHECKSUM    0xAA






Handshake Process:

"[T]0c0c011000000001000321aa"
"[R]c02601100000000100000301030000000000000000000089c1599907325500000000023c47aa"
"[T]0c0c011100000100000322aa"
"[R]c01b03110001000000000003e80a0514000000000000640000a2aa"
"[R]c01b03110001000000000003e80a0514000000000000640000a2aa"
"[R]c01b03300001b90100000003e80a0514002a00010000640000a6aa"
"[R]c01b03300001b90100000003e80a0514002a00010000640000a6aa"
"[T]0c1e033000001101010c090fffffffffffffffff04ffffffffff00007faa"
"[T]0c1e033000001101010c090fffffffffffffffff04ffffffffff00007faa"
"[R]c01b03300001b90100000003e80a0514002a00010000640000a6aa"
"[T]0c1e033000001101010c0910ffffffffffffffff04ffffffffff000080aa"
"[T]0c1e033000001101010c0910ffffffffffffffff04ffffffffff000080aa"
"[R]c01b03300001b90100000003e80a0514002a00010000640000a6aa"



"[T]0c0c011000000001000321aa"
"[R]c02601100000000100000301030000000000000000000089c1599907325500000000023c47aa"
"[T]0c0c011100000100000322aa"
"[R]c02601000000000100000301030000000000000000000089c1599907325500000000023c37aa"
"[R]c02601000000000100000301030000000000000000000089c1599907325500000000023c37aa"
"[R]c01b03110001b90100000003e80a0514002b0001000064000088aa"
"[R]c01b03300001b90100000003e80a0514002b00010000640000a7aa"
"[T]0c1e033000001101010c0a13ffffffffffffffff04ffffffffff000084aa"
"[T]0c1e033000001101010c0a13ffffffffffffffff04ffffffffff000084aa"
"[R]c01b03300001b90100000003e80a0514002b00010000640000a7aa"
"[T]0c1e033000001101010c0a14ffffffffffffffff04ffffffffff000085aa"
"[T]0c1e033000001101010c0a14ffffffffffffffff04ffffffffff000085aa"
"[R]c01b03300001b90100000003e80a0514002b00010000640000a7aa"



"[T]0c0c011000000001000321aa"
"[R]c02601100000000100000301030000000000000000000089c1599907325500000000023c47aa"
"[T]0c0c011100000100000322aa"
"[R]c02601000000000100000301030000000000000000000089c1599907325500000000023c37aa"
"[R]c01b03110001b90000000003e80a0514002c0001000064000088aa"
"[R]c01b03300001b90100000003e80a0514002c00010000640000a8aa"
"[T]0c1e033000001101010c0b00ffffffffffffffff04ffffffffff000072aa"
"[T]0c1e033000001101010c0b01ffffffffffffffff04ffffffffff000073aa"
"[R]c01b03300001b90100000003e80a0514002c00010000640000a8aa"
"[T]0c1e033000001101010c0b01ffffffffffffffff04ffffffffff000073aa"
"[T]0c1e033000001101010c0b02ffffffffffffffff04ffffffffff000074aa"
"[R]c01b03300001b90100000003e80a0514002c00010000640000a8aa"



*/

KenwoodDvr *kenwooddvr;

QByteArray responsedata;
int datime = 0, newtime = 0;

DvrProtocol::DvrProtocol(QWidget *parent) : QObject(parent)
{
    kenwooddvr = (KenwoodDvr*) parent;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TestHeartBeatFrame()));
    timer->start(1000);
}

void DvrProtocol::analysetData(const QByteArray &data, Console *console){

    switch (data[0]&0x000000ff){

    case 0x0c:

//                console->putText(QString(data.toHex().data())+"\r\n");

        //[0c xx 03 xx]
        switch (data[2]&0x000000ff){

            case 0x01:
//            console->putText("[HAND SHAKE]");
//            console->putText(QString(data.toHex().data())+"\r\n");
              if   ((data[3]&0x000000ff) == 0x10){
                  responsedata = QByteArray::fromHex("c02601100000000100000301030000000000000000000089c1599907325500000000023c47aa");

                  SendToDVR(responsedata);
              }
              if   ((data[3]&0x000000ff) == 0x11){
                  responsedata = QByteArray::fromHex("c01b03110001b90000000003e80a0514002c0001000064000088aa");

                  SendToDVR(responsedata);
                }
            break;

            case 0x03:

                switch (data[3]&0x000000ff){
                    case 0x30:
/*
                    newtime = data.at(9)&0x000000ff * 60*60;
                    newtime += data.at(10)&0x000000ff * 60*60;
                    newtime += data.at(111)&0x000000ff;

                    if (newtime != datime){
                        datime=newtime;
//                        responsedata = QByteArray::fromHex("c01b03300001b90100000003e80a0514002b00010000640000a7aa");
//                        SendToDVR(responsedata);
                    }
*/
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
                /*
                                    console->putText("[DATE TIME]");
                                    console->putText("20" + QString::number(data.at(6)&0x000000ff) + "-");    //Year
                                    console->putText(QString::number(data.at(7)&0x000000ff) + "-");    //Year
                                    console->putText(QString::number(data.at(8)&0x000000ff) + " ");    //Year
                                    console->putText(QString::number(data.at(9)&0x000000ff) + ":");    //Year
                                    console->putText(QString::number(data.at(10)&0x000000ff) + ":");    //Year
                                    console->putText(QString::number(data.at(11)&0x000000ff) + "\r\n");    //Year
                */

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
                    console->putText(QString(data.toHex().data())+"\r\n");
                switch (data[3]&0x000000ff){
                    case 0x30:

                    break;
                default:


                break;
                }
            break;
        default:

//            console->putText(QString(data.toHex().data())+"\r\n");
        break;

        }
        break;

    default:

        console->putText(QString(data.toHex().data())+"\r\n");
        break;




    }

}

void DvrProtocol::SendToDVR(QByteArray &data)
{

    kenwooddvr->sendData(data);

}

void DvrProtocol::TestHeartBeatFrame(){

    QByteArray data;
    data = QByteArray::fromHex("c01b03300001b90100000003e80a0514002b00010000640000a7aa");
    SendToDVR(data);
}
