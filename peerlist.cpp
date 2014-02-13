#include "peerlist.h"
#include <QStringList>


peerlist::peerlist(){}

void peerlist::addpeer(QString s)
{
   // qDebug()<<"string"<<s;
    if (s.contains(":")){
        QStringList list = s.split(":",QString::SkipEmptyParts) ;
        IP_address = list[0];
        UDP_port = list[1].toInt();
    }
    else
    {
        qDebug()<<"Invalid Ip Address!"<<endl;
    }


}

peerlist peerlist::pickNeighbour(int length,QList<peerlist> peerList)
{

    int neighbourNo = qrand() % length;
   // qDebug()<<"the picked IP" << neighbourNo<<endl;

    peerlist peer;

    peer.IP_address = peerList[neighbourNo].IP_address;
    peer.UDP_port = peerList[neighbourNo].UDP_port;
    return peer;
}

bool peerlist::hasPeer(QList<peerlist> peerList, peerlist peer1) {
    for (int i = 0; i < peerList.length(); i++) {
        if (peerList[i].IP_address == peer1.IP_address && peerList[i].UDP_port == peer1.UDP_port ){
            return true;
        }
    }
    return false;
}


bool peerlist::contain(QString s,QList<peerlist> peerList)
{

    peerlist peer;
    peer.addpeer(s);
    if (!peer.hasPeer(peerList, peer))
    {

        return false;
    }
    else
        return true;

}

