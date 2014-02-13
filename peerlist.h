#ifndef PEERLIST_H
#define PEERLIST_H
#include<QHostInfo>

class peerlist
{
public:
    peerlist();
    void addpeer(QString s);
  bool contain(QString s,QList<peerlist> peerList);
    peerlist pickNeighbour(int length,QList<peerlist> peerList);
    bool hasPeer(QList<peerlist> peerList, peerlist peer1);

    QString IP_address;
    quint16 UDP_port;
};

#endif // PEERLIST_H
