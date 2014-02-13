
#include <unistd.h>

#include <QGridLayout>
#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include <QHostInfo>
#include <QMap>
#include <QTimer>
#include <peerlist.cpp>
#include <QDateTime>

#include "main.hh"


ipLookUpHelper::ipLookUpHelper(quint16 port) {
    UDP_port = port;
}



ChatDialog::ChatDialog()
{
    setWindowTitle("Peerster");

    // Read-only text box where we display messages from everyone.
    // This widget expands both horizontally and vertically.
    textview = new QTextEdit(this);
    textview->setReadOnly(true);

    // Small text-entry box the user can enter messages.
    // This widget normally expands only horizontally,
    // leaving extra vertical space for the textview widget.
    //
    // You might change this into a read/write QTextEdit,
    // so that the user can easily enter multi-line messages.
    textline = new QTextEdit(this);
    textline->setFocus();

    textline->installEventFilter(this);
    ipaddrline = new QLineEdit(this);
    nodeId = new QLineEdit(this);
    fileId = new QLineEdit(this);
    testDownload = new QTextEdit(this);
    fileShare = new QTextEdit(this);
    fileSearch = new QTextEdit(this);
    listNodes = new QListWidget(this);
    listFiles = new QListWidget(this);
    hopLimit = 10;
    numRelaventFile = 0;

    QPushButton *button1 = new QPushButton("show Peerlist", this);
    QPushButton *button2 = new QPushButton("show local Database", this);
    QPushButton *button3 = new QPushButton("Share File..", this);
    QPushButton *button4 = new QPushButton("Download File..", this);
    QPushButton *button5 = new QPushButton("Search File", this);

    // Lay out the widgets to appear in the main window.
    // For Qt widget and layout concepts see:
    // http://doc.qt.nokia.com/4.7-snapshot/widgets-and-layouts.html
    QGridLayout *layout = new QGridLayout();

    layout->addWidget(textview,0,0);
    layout->addWidget(textline,1,0);
    layout->addWidget(ipaddrline,2,0);
    layout->addWidget(listNodes,0,1);
    layout->addWidget(button1, 3,0);
    layout->addWidget(button2, 4,0);
    layout->addWidget(fileShare, 1,1);
    layout->addWidget(nodeId, 2,1);
    layout->addWidget(fileId, 3,1);
    layout->addWidget(testDownload,0,2);
    layout->addWidget(button4, 4,1);
    layout->addWidget(button3, 5,1);

    layout->addWidget(listFiles,0,3);
    layout->addWidget(fileSearch,1,3);
    layout->addWidget(button5,2,3);
    //layout->
    setLayout(layout);

    connect(ipaddrline, SIGNAL(returnPressed()),
            this, SLOT(gotReturnPressed()));

    connect(button1, SIGNAL(clicked()),this,SLOT(showPeerlist()));
    connect(button2, SIGNAL(clicked()),this,SLOT(showLocalDatabase()));
    connect(button3, SIGNAL(clicked()),this,SLOT(shareFileDialog()));
    connect(button4, SIGNAL(clicked()),this,SLOT(downloadFile()));
    connect(button5, SIGNAL(clicked()), this, SLOT(sendKeyWord()));
    // Register a callback on the textline's returnPressed signal
    // so that we can send the message entered by the user.

    mySocket = new NetSocket();
    if (!mySocket->bind())
        exit(1);

    qint64  currentTime = QDateTime::currentMSecsSinceEpoch();

    //build hostName to hold hostName+port
    QHostInfo hostInfo;
    hostInfo = QHostInfo::fromName(QHostInfo::localHostName());
    hostName = QHostInfo::localHostName();
    hostName.append(":").append(QString::number(mySocket->getMyPort()));
    hostName.append(":").append(QString::number(currentTime));
    connect(mySocket, SIGNAL(readyRead()),
              this, SLOT(readPendingDatagrams()));

    connect(listNodes, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(private_dialogue(QListWidgetItem*)));
    connect(listFiles, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(downloadFileFromWidget(QListWidgetItem *)));

    QTimer *entiEntro_timer = new QTimer(this);
    connect(entiEntro_timer, SIGNAL(timeout()), this, SLOT(choosePort1()));
        entiEntro_timer->start(10000);

    QTimer *route_timer = new QTimer(this);
    connect(route_timer, SIGNAL(timeout()), this, SLOT(sendRouteMsg()));
            route_timer->start(10000);
    QTimer *search_timer = new QTimer(this);
    connect(search_timer, SIGNAL(timeout()), this, SLOT(sendKeyWordPeriodic()));
    search_timer->start(2000);

    QString ipaddr_port;

     int i;
     for (i=0; i<4; i++)
     {
         ipaddr_port = QHostAddress("127.0.0.1").toString();
         if(mySocket->getMyPort()!= mySocket->getMyPortMin()+i)
         { ipaddr_port.append(":").append(QString::number(mySocket->getMyPortMin()+i)) ;
             if (!peer.contain(ipaddr_port,peerList)){
                 peer.addpeer(ipaddr_port);
                 peerList.append(peer);

             }
         }

     }
      sendRouteMsg();
}
void ChatDialog::sendKeyWordPeriodic()
{

    if ((budget<100)&&(numRelaventFile<=10)){
        if (keyWord != ""){
            budget = budget*2;
            QVariantMap message;
            message.insert("Origin",hostName);
            message.insert("Search",keyWord);
            message.insert("Budget", budget);
            int n = nextHopTable.size();
            if (n != 0){
                int m = qrand() % n;
                int l;
                QHash<QString, QPair<QString,quint16> >::iterator i = nextHopTable.begin();
                for (l = 1;l<m;l++){
                    i++;
                }
                QString destIp = i.value().first;
                quint16 destPort = i.value().second;
                QByteArray datagram;
                QDataStream stream(&datagram,QIODevice::ReadWrite);
                stream << message;
                mySocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(destIp),
                                        destPort);
            }
        }

   }


}
void ChatDialog::sendKeyWord()
{
    keyWord = fileSearch->toPlainText();
    fileSearch->clear();
    budget = 2;
    QVariantMap message;
    message.insert("Origin",hostName);
    message.insert("Search",keyWord);
    message.insert("Budget",budget);
    int n = nextHopTable.size();
    if (n != 0){
        int m = qrand() % n;
        int l;
        QHash<QString, QPair<QString,quint16> >::iterator i = nextHopTable.begin();
        for (l = 1;l<m;l++){
            i++;
        }
        QString destIp = i.value().first;
        quint16 destPort = i.value().second;
        QByteArray datagram;
        QDataStream stream(&datagram,QIODevice::ReadWrite);
        stream << message;
        mySocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(destIp),
                                destPort);
    }
}

void ChatDialog::readSearchRequest(QVariantMap msg){
    QString str = msg.value("Search").toString();
    QStringList list = str.split(" ") ;
    QStringList matchNames;
    QVariantList matchIds;

    int order= 0;
    for (int listIndex = 0;listIndex < list.length(); listIndex++){
        QHash<QByteArray, QVariantMap>::iterator i = metafileList.begin();

        while (i != metafileList.end()) {
            QHash<QByteArray, QVariantMap >::iterator prev = i;
            if (prev.value().value("filename").toString().contains(list[listIndex])) {
                matchNames.insert(order, prev.value().value("filename").toString());
                order++;
                matchIds.insert(order,prev.key());
            }
            ++i;
        }

    }

    QVariantMap searchReply;
    searchReply.insert("Origin",hostName);
    searchReply.insert("Dest",msg.value("Origin"));
    searchReply.insert("SearchReply",msg.value("Search"));
    searchReply.insert("Hoplimit",hopLimit);
    searchReply.insert("MatchNames",matchNames);
    searchReply.insert("MatchIDs",matchIds);
    sendToNode(searchReply, msg.value("Origin").toString());

    //forward the request to neighbours and subdivide budgets
    quint32 budget_new = msg.value("Budget").toUInt() - 1;
    if (budget_new > 0){
        int n = nextHopTable.size();
        int modBudget = budget_new % n;
        int divBudget = budget_new / n;
        int l = 0;
        int budgetNew;
        QHash<QString, QPair<QString,quint16> >::iterator j = nextHopTable.begin();

        for (l = 0;l < n; l++){
            QHash<QString, QPair<QString,quint16>  >::iterator prev = j;
            ++j;
            if (l < modBudget){
              budgetNew = divBudget+1;
            }
            if (budgetNew > 0){
                QString destIp = prev.value().first;
                quint16 destPort = prev.value().second;
                msg["Budget"] = budgetNew;
                QByteArray datagram;
                QDataStream stream(&datagram,QIODevice::ReadWrite);
                stream << msg;
                mySocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(destIp),
                                         destPort);
            }
        }
    }

}
void ChatDialog::readSearchReply(QVariantMap msg)
{

    QString widgetName;
    QHash<QString, QByteArray> fileAndHash;
    QStringList matchNames = msg.value("MatchNames").toStringList();
    QVariantList matchIds = msg.value("MatchIDs").toList();
    if (msg.value("Dest") == hostName)
    {

        if (downloadSource.contains(msg.value("Origin").toString())){
            for (int i = 0; i < matchNames.length(); i++){
                widgetName = msg.value("Origin").toString();
                widgetName.append(matchNames[i]);
                if (downloadSource.value(msg.value("Origin").toString()).contains(widgetName)){
                   qDebug()<<"Already get this file from this origin";
                }
                else{
                    widgetName = msg.value("Origin").toString();
                    widgetName.append(matchNames[i]);
                    new QListWidgetItem(widgetName ,listFiles);
                    mapIDtoName.insert(matchIds[i].toByteArray(),matchNames[i]);
                    numRelaventFile ++;
                    QHash<QString,QHash<QString,QByteArray> >::iterator j = downloadSource.find(msg.value("Origin").toString());
                    j.value().insert(widgetName, matchIds[i].toByteArray());
                }

            }

        }
        else{
            for (int i = 0; i < matchNames.length(); i++){
                widgetName = msg.value("Origin").toString();
                widgetName.append(matchNames[i]);
                new QListWidgetItem(widgetName ,listFiles);
                mapIDtoName.insert(matchIds[i].toByteArray(),matchNames[i]);
                numRelaventFile ++;
                fileAndHash.insert(widgetName, matchIds[i].toByteArray());
            }
            downloadSource.insert(msg.value("Origin").toString(),fileAndHash);
       }

    }
    else
    {
        if (msg.value("HopLimit") == 0)
        {
            qDebug()<<"Discard the message!";

        }
        else
        {
            if (!cmdForward)
            {
                msg["HopLimit"] = msg.value("HopLimit").toInt()-1;

                forwardPriMsg(msg);
            }

        }
    }
}

void ChatDialog::shareFileDialog()
{
   // shareFile = new ShareFile();
    fileDialog = new QFileDialog();
    fileDialog->setFileMode(QFileDialog::ExistingFiles);

   // fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->show();

    if ( !fileDialog->exec() ) {
        qDebug() << "You select nothing.";
        return;
    }

    QStringList fileNames = fileDialog->selectedFiles();
    scanAndCatalog(fileNames);


}

void ChatDialog::scanAndCatalog(QStringList fileNames)
{
    int i;
    for (i = 0; i< fileNames.length(); i++){
        QFile file(fileNames.at(i));
        if(!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error", file.errorString());
        }
        QDataStream in(&file);
        QByteArray metabyte;
        while(!in.atEnd()) {
            QByteArray bytes = file.read(8192);
            QByteArray hashByte = QCA::Hash("sha256").hash(bytes).toByteArray();
            metabyte.push_back(hashByte);
            datafileHash.insert(hashByte,bytes);
        }
        QByteArray hashMetafile = QCA::Hash("sha256").hash(metabyte).toByteArray();
        fileShare->append(hashMetafile.toHex());
        fileShare->append(hostName);

        QVariantMap metafile;

        metafile.insert("filename",fileNames.at(i));
        metafile.insert("filesize",file.size());
        metafile.insert("metafile",metabyte);
        metafileList.insert(hashMetafile,metafile);
    }
}
void ChatDialog::downloadFileFromWidget(QListWidgetItem *item)
{
    QString filename = item->text();
    QHash<QString, QHash<QString,QByteArray> >::iterator i = downloadSource.begin();
    while (i != downloadSource.end()) {
    QHash<QString, QHash<QString,QByteArray> >::iterator prev = i;
    ++i;
    if (prev.value().contains(filename)){
        qDebug() << "============================================================== double clicked";
        QVariantMap blockRequest;
        QByteArray fileHashId = prev.value().value(filename);
        fileIdList.insert(fileHashId,1);
        blockRequest.insert("Dest",prev.key());
        blockRequest.insert("Origin", hostName);
        blockRequest.insert("HopLimit",hopLimit);
        blockRequest.insert("BlockRequest",fileHashId);
        sendToNode(blockRequest, prev.key());
    }
   }
}
void ChatDialog::downloadFile()
{
    if (nextHopTable.contains(nodeId->text())){
        QVariantMap blockRequest;
        QByteArray fileHashId = QByteArray::fromHex(fileId->text().toUtf8());
        fileIdList.insert(fileHashId,1);
        blockRequest.insert("Dest",nodeId->text());
        blockRequest.insert("Origin", hostName);
        blockRequest.insert("HopLimit",hopLimit);
        blockRequest.insert("BlockRequest",fileHashId);
        sendToNode(blockRequest, nodeId->text());

    }
    else
    {
        qDebug()<<"No such node exists!"<<endl;
    }

    nodeId->clear();
    fileId->clear();
}

void ChatDialog::readBlockRequest(QVariantMap msg)
{
    qDebug() << "================================================ hahahaah";
    if (msg.value("Dest") == hostName)
    {
        QString senderInfo = "BlockRequest Message From:";
        senderInfo.append(msg.value("Origin").toString());
        fileShare->append(senderInfo);
        fileShare->append(msg.value("BlockRequest").toString());
        QByteArray hashedId  = msg.value("BlockRequest").toByteArray();
        qDebug() << "================================================ " << msg;
        qDebug()<<  "================ hashId"   <<hashedId;
        if (!metafileList.contains(hashedId)
                && !datafileHash.contains(hashedId)){
            fileShare->append("No such fileId exists!");
        }
        else{
            //***assume the dest is already in nextHopTable
            if (!nextHopTable.contains(msg.value("Origin").toString())){
                fileShare->append("dest is not contained in the table!");
            }
            else{
                QByteArray data;
                if (metafileList.contains(hashedId)){
                    data = metafileList.value(hashedId).value("metafile").toByteArray();
                }
                else{
                    data = datafileHash.value(hashedId);
                }
                QVariantMap blockReply;
                blockReply.insert("Dest",msg.value("Origin"));
                blockReply.insert("Origin", hostName);
                blockReply.insert("HopLimit",hopLimit);
                blockReply.insert("BlockReply", hashedId);
                blockReply.insert("Data",data);
                sendToNode(blockReply, msg.value("Origin").toString());
            }

        }

    }
    else
    {
        if (msg.value("HopLimit") == 0)
        {
            qDebug()<<"Discard the message!";
        }
        else
        {
            if (!cmdForward)
            {
                msg["HopLimit"] = msg.value("HopLimit").toInt()-1;
                forwardPriMsg(msg);
            }

        }
    }
}

void ChatDialog::readBlockReply(QVariantMap msg)
{
    QByteArray metafile;
    QByteArray metabyte;

    if (msg.value("Dest") == hostName)
    {
       QString senderInfo = "BlockReply Message From:";
       senderInfo.append(msg.value("Origin").toString());
       fileShare->append(senderInfo);
       fileShare->append(msg.value("BlockReply").toString());
       if (fileIdList.contains(msg.value("BlockReply").toByteArray())){
           QHash<QByteArray, int> currentByte;
           metafile = msg.value("Data").toByteArray();

           qDebug() << "========================================= &&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << metafile;
           QVariantMap blockRequest;
           int m = 32;
           metabyte = metafile.mid(0, m);
           currentByte.insert(metabyte,m);

           requestMetabyte.insert(msg.value("BlockReply").toByteArray(),currentByte);
           requestMetafile.insert(msg.value("BlockReply").toByteArray(), metafile);

           blockRequest.insert("Dest",msg.value("Origin"));
           blockRequest.insert("Origin", hostName);
           blockRequest.insert("HopLimit",hopLimit);
           blockRequest.insert("BlockRequest",metabyte);
           blockRequest.insert("Chen", 1);
           sendToNode(blockRequest, msg.value("Origin").toString());
       } else {

            QHash<QByteArray, QHash<QByteArray,int> >::iterator i = requestMetabyte.begin();
            while (i != requestMetabyte.end()) {
            QHash<QByteArray, QHash<QByteArray,int> >::iterator prev = i;
            ++i;
            if (prev.value().contains(msg.value("BlockReply").toByteArray())){
                qDebug() << "============================================1";
                if (requestDatafile.contains(prev.key())){
                   //strange><
                    QHash<QByteArray,QByteArray>::iterator j = requestDatafile.find(prev.key());
                    j.value().push_back(msg.value("Data").toByteArray());
                    testDownload->append(msg.value("Data").toString());
                }
                else{
                    requestDatafile.insert(prev.key(),msg.value("Data").toByteArray());
                    testDownload->append(msg.value("Data").toString());
                }
                metafile = requestMetafile.value(prev.key());
                int currentByteNum = prev.value().value(msg.value("BlockReply").toByteArray());
                qDebug()<<"______________num___________"<<currentByteNum;
                QHash<QByteArray, int> currentByte;
                int m = 32;
                if (metafile.size() >= (currentByteNum + m)){
                    metabyte = metafile.mid(currentByteNum, m);
                    currentByte.insert(metabyte,currentByteNum + m);
                    prev.value() = currentByte;
                    QVariantMap blockRequest;
                    blockRequest.insert("Dest",msg.value("Origin"));
                    blockRequest.insert("Origin", hostName);
                    blockRequest.insert("HopLimit",hopLimit);
                    blockRequest.insert("BlockRequest",metafile.mid(currentByteNum,32));
                    sendToNode(blockRequest, msg.value("Origin").toString());
                    break;
                }
                else if (metafile.size() == currentByteNum){
                    QString fileName;
                    fileName = mapIDtoName.value(prev.key());
                    QStringList pathAndFile = fileName.split("/");
                    fileName = pathAndFile.last();
                    qint64 timenow = QDateTime::currentMSecsSinceEpoch();
                    QString downloadFileName = QString("%1_%2").arg(timenow).arg(fileName);

                    QFile file(downloadFileName);
                    if (!file.open(QIODevice::WriteOnly))
                           return;
                       file.write(requestDatafile.value(prev.key()));
                       file.close();
                       break;
                }
            }

        }
       }
    }
    else
    {
        if (msg.value("HopLimit") == 0)
        {
            qDebug()<<"Discard the message!";

        }
        else
        {
            if (!cmdForward)
            {
                msg["HopLimit"] = msg.value("HopLimit").toInt()-1;

                forwardPriMsg(msg);
            }

        }
    }
}

void ChatDialog::showPeerlist()
{

    for (int i = 0; i < peerList.size(); ++i)
 {
   peerlist emp;
   emp = peerList.at(i);

 }
    qDebug()<<"hop table"<<nextHopTable;

}

void ChatDialog::showLocalDatabase()
{
    qDebug()<<"local DB";
    qDebug()<<localDataBase;


}
void ChatDialog::private_dialogue(QListWidgetItem *item)
{

    // open privte chat window
    PrivDialog *privateDialog = new PrivDialog(item->text(),hostName,nextHopTable, mySocket);
    connect(textline, SIGNAL(returnPressed()),
            this, SLOT(gotReturnPressed()));
    privateDialog->show();
}

bool ChatDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == textline) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Enter  || keyEvent->key() == Qt::Key_Return)
            {
                if (!(textline->toPlainText() == ""))
                {
                   sendDatagrams();
                }

                textline->clear();
                return true;
            }
            return false;
        }

    } else {
        // pass the event on to the parent class
        return ChatDialog::eventFilter(obj, event);
    }
}


 void ChatDialog::gotReturnPressed()
{

    // Initially, just echo the string locally.
    // Insert some networking code here...
    qDebug() << "FIX: send message to other peers: " << ipaddrline->text();

// transfer the ip:host
    if (!peer.contain(ipaddrline->text(),peerList)){
        QString hp = ipaddrline->text();
        if (hp.contains(":")){
            QStringList list = hp.split(":",QString::SkipEmptyParts) ;
            ipLoopupHelper = new ipLookUpHelper(list[1].toInt());
          QHostInfo::lookupHost(list[0], this, SLOT(lookedUp(QHostInfo)));
        }
        else
        {
            qDebug()<<"Invalid Ip Address!"<<endl;
        }
    }

    // Clear the textline to get ready for the next input message.
    ipaddrline->clear();
}

NetSocket::NetSocket()
{
    // Pick a range of four UDP ports to try to allocate by default,
    // computed based on my Unix user ID.
    // This makes it trivial for up to four Peerster instances per user
    // to find each other on the same host,
    // barring UDP port conflicts with other applications
    // (which are quite possible).
    // We use the range from 32768 to 49151 for this purpose.
    myPortMin = 32768 + (getuid() % 4096)*4;
    myPortMax = myPortMin + 3;
}


//E3
void ChatDialog::sendDatagrams()
{

    SeqNo+=1;

    QVariantMap msg;
    msg.insert("ChatText",textline->toPlainText());
    msg.insert("Origin", hostName);
    msg.insert("SeqNo",  SeqNo);

    //save local message to local database

    localMap.append(msg);
    if(localDataBase.find(hostName) == localDataBase.end()){
        QMap <int, QString>innerMap;
        innerMap.insert(1,textline->toPlainText());
        localDataBase.insert(hostName, innerMap);
        localMsg.insert(hostName,2);
    }
    else{
        localDataBase[hostName].insert(SeqNo, textline->toPlainText());
        localMsg[hostName]=SeqNo+1;

    }


    textview->append(textline->toPlainText());

    randomPickandSend(msg);
}
void ChatDialog::sendToNode(QVariantMap msg, QString destOrigin)
{

    QByteArray datagram;
    QDataStream stream(&datagram,QIODevice::ReadWrite);
    stream << msg;
    mySocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(nextHopTable.value(destOrigin).first),
                            nextHopTable.value(destOrigin).second);

}

void ChatDialog::randomPickandSend(QVariantMap message)
{
    QByteArray datagram;
    QDataStream stream(&datagram,QIODevice::ReadWrite);
    stream << message;

    peerlist neighbour = peer.pickNeighbour(peerList.length(), peerList);
    mySocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(neighbour.IP_address),
                                                neighbour.UDP_port);
}

void ChatDialog::sendToAll(QVariantMap message)
{
    QByteArray datagram;
    QDataStream stream(&datagram,QIODevice::ReadWrite);
    stream << message;
    for (int i = 0; i < peerList.size(); ++i){
     peerlist emp = peerList.at(i);
     mySocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(emp.IP_address),
                                               emp.UDP_port);
    }
}

void ChatDialog::lookedUp(const QHostInfo &host)
 {
     if (host.error() != QHostInfo::NoError) {
         qDebug() << host.hostName();
         qDebug() << "Lookup failed:" << host.errorString();
         return;
     }

     QString ip = host.addresses().first().toString();
     ipLoopupHelper->IP_address = ip;

     peerlist peer;
     peer.IP_address = ipLoopupHelper->IP_address;
     peer.UDP_port = ipLoopupHelper->UDP_port;

     peerList.append(peer);
 }


//if time out, trigger this slot to send the new message in your database to your neighbour
void ChatDialog::sendNewMessage_FlipCoin()
{
   QVariantMap wantMsg;
   wantMsg.insert("Want",localMsg);
    if (qrand()%1000>500)
    {
        randomPickandSend(wantMsg);
    }
    else
    {
       // qDebug()<<"stop"<<endl;
    }
}


void ChatDialog::choosePort1()
{
    QVariantMap wantMsg;
    wantMsg.insert("Want",localMsg);
    QByteArray datagram;
    QDataStream stream(&datagram,QIODevice::ReadWrite);
    stream << wantMsg;
   randomPickandSend(wantMsg);

}

void ChatDialog::sendRouteMsg()
{
    QMap<QString, QVariant> route_msg;
    SeqNo += 1;
    route_msg.insert("Origin",hostName);
    route_msg.insert("SeqNo",SeqNo);

    localMap.append(route_msg);
    if(localDataBase.find(hostName) == localDataBase.end()){
        QMap <int, QString>innerMap;
        innerMap.insert(SeqNo,"");
        localDataBase.insert(hostName, innerMap);
        localMsg.insert(hostName,2);
    }
    else{
        localDataBase[hostName].insert(SeqNo,"");
        localMsg[hostName]=SeqNo+1;

    }
    
    sendToAll(route_msg);
}

void ChatDialog::sendBackToSender(QVariantMap msg,QHostAddress sender, quint16 senderPort)
{


    QByteArray wantDatagram;
    QDataStream stream(&wantDatagram,QIODevice::ReadWrite);
    stream << msg;

    mySocket->writeDatagram(wantDatagram.data(), wantDatagram.size(), sender,
                                                 senderPort);
}

void ChatDialog::addPeerComamnd(QString ip, quint16 port)
{
    ipLoopupHelper = new ipLookUpHelper(port);
    QHostInfo::lookupHost(ip, this, SLOT(lookedUp(QHostInfo)));
}

void ChatDialog::insertNextHopTable(QVariantMap msg,QString sender,quint16 senderPort)
{
    QPair<QString, quint16> nextHop;
    nextHop.first = sender;
    nextHop.second = senderPort;
    if (msg.value("Origin").toString()!= hostName)
    {
        if (nextHopTable.contains(msg.value("Origin").toString()))
        {
            nextHopTable[msg.value("Origin").toString()] = nextHop;
        }
        else
        {
           nextHopTable.insert(msg.value("Origin").toString(),nextHop);
           QString origin = msg.value("Origin").toString();
           new QListWidgetItem(origin,listNodes );
        }
    }
}

void ChatDialog::readPendingDatagrams()
{

    while (mySocket->hasPendingDatagrams())  {
        timer_E5 = new QTimer(this);
        timer_E5->stop();

        QByteArray datagram;
        datagram.resize(mySocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        mySocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        QString ipaddr_port = sender.toString();
        ipaddr_port.append(":").append(QString::number(senderPort)) ;
        if (!peer.contain(ipaddr_port,peerList)){
            peerlist p;
            p.IP_address = sender.toString();
            p.UDP_port = senderPort;
            peerList.append(p);
        }
        QVariantMap msg;
        bool directRouteFlag = true;

        QDataStream stream(&datagram,QIODevice::ReadWrite);
        stream >> msg;
        if (msg.contains("LastIP")||msg.contains("LastPort")){

            QString lastIP_Port = msg.value("LastIP").toString();
            lastIP_Port.append(":").append(msg.value("LastPort").toString()) ;

            if (!peer.contain(lastIP_Port,peerList)){
               peerlist p;
               p.IP_address = msg.value("LastIP").toString();
               p.UDP_port = msg.value("LastPort").toInt();
               peerList.append(p);
            }
            directRouteFlag = false;
//            lastIP = msg.value("LastIP").toString();
//            lastPort = msg.value("LastPort").toInt();
            msg.remove("LastIP");
            msg.remove("LastPort");

        }

        // WantMsg
        QVariantMap wantMsg;
        QMap<int, QString> innerMap;

        wantMsg.insert("Want",localMsg);



        QMap<QString, QVariant>::iterator j = msg.find("Want");

        if (j != msg.end())
        {
            // the status msg

            //compare the SeqNo with local  message to see if the sender has anything new

            QVariantMap sendNewMsg;

            QMapIterator<QString, QVariant> k(msg.value("Want").toMap());
            int flag1 = 0;
            int flag2 = 0;
            while (k.hasNext()) {
                k.next();
                if(localDataBase.contains(k.key()))
                {
                    innerMap = localDataBase.value(k.key()) ;

                    if (localMsg.value(k.key()).toInt()>k.value().toInt())
                    {
                        if (!(innerMap.value(k.value().toInt())==""))
                        {
                        sendNewMsg.insert("ChatText",innerMap.value(k.value().toInt()));
                        sendNewMsg.insert("Origin", k.key());
                        sendNewMsg.insert("SeqNo",  k.value());
                        if (!cmdForward)
                        {
                            sendBackToSender(sendNewMsg, sender, senderPort);
                        }
                        }
                        else
                        {
                            sendNewMsg.insert("Origin", k.key());
                            sendNewMsg.insert("SeqNo",  k.value());
                            sendBackToSender(sendNewMsg, sender, senderPort);
                        }


                        flag1 =1;
                    }
                    else if (localMsg.value(k.key()).toInt()<(k.value().toInt()-1))
                    {
                        sendBackToSender(wantMsg, sender, senderPort);
                        flag1 = 1;
                    }
                }
                else
                {
                 //   qDebug()<<"=======status msg sent====="<<localMsg<<endl;
                    localMsg.insert(k.key(),1);
                    wantMsg["Want"] = localMsg;

                   sendBackToSender(wantMsg, sender, senderPort);
                    flag1 = 1;
                }

            }
           //check if there are origins on local DB not in the Status message received
            if (flag1 == 0){
                QMapIterator<QString, QVariant> m(localMsg);
                while (m.hasNext())
                {
                    m.next();
                    if ( !msg.value("Want").toMap().contains(m.key()))
                    {
                        innerMap = localDataBase.value(m.key());
                        sendNewMsg.clear();

                        if (!(innerMap.value(m.value().toInt())==""))
                        {
                        sendNewMsg.insert("ChatText",innerMap.value(m.value().toInt()));
                        sendNewMsg.insert("Origin", m.key());
                        sendNewMsg.insert("SeqNo", 1);
                        if (!cmdForward)
                        {
                            sendBackToSender(sendNewMsg, sender, senderPort);
                        }
                        }
                        else
                        {
                            sendNewMsg.insert("Origin", m.key());
                            sendNewMsg.insert("SeqNo", 1);
                            sendBackToSender(sendNewMsg,sender, senderPort);
                        }

                        flag2 = 1;
                    }
                }
                if (flag2 == 0) {
                    sendNewMessage_FlipCoin();
                }
            }
        }
        else if (msg.contains("Search")){
            readSearchRequest(msg);
        }
        else if(msg.contains("SearchReply")){
            readSearchReply(msg);
        }
        else if (msg.contains("BlockRequest")){
            readBlockRequest(msg);
        }
        else if (msg.contains("BlockReply")){

           readBlockReply(msg);
        }
        else if (!msg.contains("Dest"))
        {   //the rumor msg
            if(localMsg.contains(msg.value("Origin").toString()) )
                {
                             //@@@accept the msg only when status msg seq = rumor msg seq,
                            //otherwise discard this message
                   // qDebug() << localMsg.value(msg.value("Origin").toString());
                if (msg.value("SeqNo").toInt() < localMsg.value(msg.value("Origin").toString()).toInt()){
                    if (directRouteFlag == true)
                        insertNextHopTable(msg,sender.toString(),senderPort);
                }
                 else if (localMsg.value(msg.value("Origin").toString())==msg.value("SeqNo"))
                    {

                                localMsg[msg.value("Origin").toString()]=msg.value("SeqNo").toInt()+1;
                                localMap.append(msg);

                                // qDebug()<<"rumor msg sent out flip coin call2"<<endl;]
                                connect(timer_E5, SIGNAL(timeout()), this, SLOT(sendNewMessage_FlipCoin()));
                                timer_E5->start(1500);
                               // textview->append(msg.value("ChatText").toString());
                                insertNextHopTable(msg,sender.toString(),senderPort);

                                wantMsg["Want"]=localMsg;
                                //sendback the Want QVariantMap
                                if (senderPort != mySocket->getMyPort())
                                {
                                    sendBackToSender(wantMsg,sender, senderPort);
                                }
                                if (msg.contains("ChatText"))
                                {

                                    textview->append(msg.value("ChatText").toString());
                                    innerMap = localDataBase.value(msg.value("Origin").toString());
                                    qDebug()<<innerMap;
                                    innerMap.insert(msg.value("SeqNo").toInt(),msg.value("ChatText").toString());
                                    localDataBase[msg.value("Origin").toString()]=innerMap;
                                    if (!cmdForward)
                                    {
                                        msg.insert("LastIP",sender.toString());
                                        msg.insert("LastPort",senderPort);
                                        randomPickandSend(msg);
                                    }
                                }
                                else
                                {
                                    innerMap = localDataBase.value(msg.value("Origin").toString());
                                    innerMap.insert(msg.value("SeqNo").toInt(),"");
                                    localDataBase.insert(msg.value("Origin").toString(),innerMap);
                                    msg.insert("LastIP",sender.toString());
                                    msg.insert("LastPort",senderPort);
                                    //randomPickandSend(msg);
                                    sendToAll(msg);
                                }

                        }

                }
                else
                {
            if (msg.value("SeqNo")==1)
            {
               // qDebug() << "==================================== 1" ;
                    localMsg.insert(msg.value("Origin").toString(),2);
                    localMap.append(msg);
                     // qDebug()<<"rumor msg sent out flip coin call"<<endl;
                    connect(timer_E5, SIGNAL(timeout()),
                            this, SLOT(sendNewMessage_FlipCoin()));
                    timer_E5->start(1500);

                    insertNextHopTable(msg,sender.toString(),senderPort);

                    wantMsg["Want"]=localMsg;
                      //sendback the Want QVariantMap

                    if (msg.contains("ChatText"))
                    {
                        textview->append(msg.value("ChatText").toString());
                        innerMap.clear();
                        innerMap.insert(msg.value("SeqNo").toInt(),msg.value("ChatText").toString());
                        localDataBase.insert(msg.value("Origin").toString(),innerMap);
                        if (!cmdForward)
                        {
                            msg.insert("LastIP",sender.toString());
                            msg.insert("LastPort",senderPort);
                            randomPickandSend(msg);
                        }

                    }
                    else
                    {
                        innerMap.clear();
                        innerMap.insert(msg.value("SeqNo").toInt(),"");
                        localDataBase.insert(msg.value("Origin").toString(),innerMap);
                        msg.insert("LastIP",sender.toString());
                        msg.insert("LastPort",senderPort);
                        //randomPickandSend(msg);
                        sendToAll(msg);
                    }
                    if (senderPort != mySocket->getMyPort())
                    {
                        sendBackToSender(wantMsg,sender, senderPort);
                    }

            }
            else
            {

                        localMsg.insert(msg.value("Origin").toString(),1);

                        wantMsg["Want"]=localMsg;

                        //sendback the Want QVariantMap
                        if (senderPort != mySocket->getMyPort())
                        {
                           sendBackToSender(wantMsg,sender, senderPort);
                        }
                }
               }


           }

        else{

                if (msg.value("Dest") == hostName)
                {
                    QString senderInfo = "PriMsgFrom:";
                    senderInfo.append(ipaddr_port);
                    textview->append(senderInfo);
                    textview->append(msg.value("ChatText").toString());
                }
                else
                {
                    if (msg.value("HopLimit") == 0)
                    {
                        qDebug()<<"Discard the message!";

                    }
                    else
                    {
                        if (!cmdForward)
                        {
                            msg["HopLimit"] = msg.value("HopLimit").toInt()-1;

                            forwardPriMsg(msg);
                        }

                    }
                }

        }

    }
 }



void ChatDialog::forwardPriMsg(QVariantMap msg)
{

    QByteArray datagram;
    QDataStream stream(&datagram,QIODevice::ReadWrite);
    stream << msg;

    mySocket->writeDatagram(datagram.data(), datagram.size(),
                            QHostAddress(nextHopTable.value(msg.value("Dest").toString()).first),
                            nextHopTable.value(msg.value("Dest").toString()).second);
}

bool NetSocket::bind()
{
    // Try to bind to each of the range myPortMin..myPortMax in turn.
    for (int p = myPortMin; p <= myPortMax; p++) {
        if (QUdpSocket::bind(p)) {
            myPort = p;
            qDebug() << "bound to UDP port " << p;
            return true;
        }
    }

    qDebug() << "Oops, no ports in my default range " << myPortMin
        << "-" << myPortMax << " available";
    return false;
}

PrivDialog::PrivDialog(QString dest,QString origin, QHash<QString, QPair<QString, quint16> > NextHopTable, NetSocket *MySocket)
{
    destOrigin = dest;
    Origin = origin;
    hopLimit = 10;
    hopTable = NextHopTable;
    mySocket = MySocket;
    setWindowTitle("Private Dialogue");
    textview = new QTextEdit(this);
    textview->setReadOnly(true);

    textline = new QLineEdit(this);
    textline->setFocus();

    QGridLayout *layout = new QGridLayout();

    layout->addWidget(textview,0,0);
    layout->addWidget(textline,1,0);
    qDebug()<<"----------------privdialog";
    setLayout(layout);

    connect(textline, SIGNAL(returnPressed()),
              this, SLOT(gotReturnPressed()));

}

void PrivDialog::gotReturnPressed()
{
    textview->append(textline->text());
    sendDatagrams();
       // Clear the textline to get ready for the next input message.
    textline->clear();
}

void PrivDialog::sendDatagrams()
{

    QVariantMap msg;
    msg.insert("ChatText",textline->text());
    msg.insert("Dest", destOrigin);
    msg.insert("HopLimit",  hopLimit);
    msg.insert("Origin",Origin);

    //****save local message to local database***///
    QByteArray datagram;
    QDataStream stream(&datagram,QIODevice::ReadWrite);
    stream << msg;
    mySocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(hopTable.value(destOrigin).first),
                            hopTable.value(destOrigin).second);

    textview->append(textline->text());

}



int main(int argc, char **argv)
{
    // Initialize Qt toolkit
    QApplication app(argc,argv);
    //
    QCA::Initializer qcainit;
    // Create an initial chat dialog window
    ChatDialog dialog;
    dialog.cmdForward = false;
    dialog.SeqNo = 0;

    if (argc == 2) {
        QString cmdLine = argv[1];
        qDebug()<<"commandline"<<argv[1];
        if (cmdLine == "-noforward")
        {
           dialog.cmdForward = true;
        } else {
            QString hp = cmdLine;
            QStringList list = hp.split(":",QString::SkipEmptyParts) ;
            dialog.ipLoopupHelper = new ipLookUpHelper(list[1].toInt());
            dialog.addPeerComamnd(list[0], list[1].toInt());
        }
    }

    dialog.show();

    // Create a UDP network socket


    // Enter the Qt main loop; everything else is event driven
    return app.exec();
}
