#include "kenwooddvr.h"

//Serial Buffer0
QByteArray buffer[2];
int ptr[2];

KenwoodDvr::KenwoodDvr(QObject *parent) : QObject(parent)
{
    ptr[0]=0;
    ptr[1]=0;
}

void SerialBuffer(int port, const QByteArray &data, Console *console){
    int i;

    //Add to buffer
    if (ptr[port]==0){
        if (((data[0]&0x000000ff) == 0x0c)||((data[0]&0x000000ff) == 0xc0)){
                for (i=0;i<data.size();i++){
                    buffer[port][i] = data[i];
                }
                ptr[port] = data.size();
//                console->putText("ptr[port]="+ QString::number(ptr[port])+"\r\n");
//                console->putText("Frame start.\r\n");
            }
        else
            console->putText("Err Frame.\r\n");
        }
    else{
        for (i=0;i<data.size();i++){
            buffer[port][i+ptr[port]]=data[i];
        }
        ptr[port] += data.size();
//        console->putText("ptr[port]="+ QString::number(ptr[port])+"\r\n");
//        console->putText("Frame continue.\r\n");
    }



    if (ptr[port]>=(buffer[port][1]&0x000000ff)){
        if ((buffer[port][buffer[port][1]-1]&0x000000ff) == 0xAA){
//            console->putText("Frame end, corrent.\r\n");

            if ((buffer[port][0]&0x000000ff) == 0xc0)
                console->putText("[R]" + QString(buffer[port].mid(0,ptr[port]).toHex().data()) + "\r\n");
            if ((buffer[port][0]&0x000000ff) == 0x0c)
                console->putText("[T]" + QString(buffer[port].mid(0,ptr[port]).toHex().data()) + "\r\n");

            ptr[port] = 0;
        }
        else{  
            console->putText("ptr[port]="+ QString::number(ptr[port])+"\r\n");
            console->putText("buffer[port][1]="+ QString::number(buffer[port][1]&0x000000ff)+"\r\n");
            console->putText("Frame end, wrong.\r\n");
            console->putText("[==E==]" + QString(buffer[port].mid(0,ptr[port]).toHex().data()) + "\r\n");


            ptr[port] = 0;

        }
    }
}


QString ByteArrayToHexStr(QByteArray data)
 {
    QString temp="";
    QString hex=data.toHex();
    for (int i=0;i<=hex.length();i=i+2)
    {
        if ((i + 2) < hex.size()+1) {
           temp+=hex.mid(i,2)+".";
        }
    }
    return temp.trimmed().toLower();
}


void KenwoodDvr::putData(int port, const QByteArray &data, Console *console){

//    console->putText("[" + QString::number(port, 10) +"]" + QString(data.toHex().data()) + "\r\n");

    SerialBuffer(port, data, console);
}
