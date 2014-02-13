#ifndef PEERSTER_MAIN_HH
#define PEERSTER_MAIN_HH

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QUdpSocket>
#include <QKeyEvent>
#include <peerlist.h>
#include <QHostInfo>
#include <QHash>
#include <QListWidget>
#include <QFileDialog>
#include <QStringList>
#include <QtCrypto>
#include <QMessageBox>



class ipLookUpHelper : public QObject
{
    Q_OBJECT
public:
    ipLookUpHelper(quint16 port);
    QString IP_address;
    quint16 UDP_port;
};


class NetSocket : public QUdpSocket
{
    Q_OBJECT

public:
    NetSocket();
    // Bind this socket to a Peerster-specific default port.
    bool bind();

    int getMyPort() {return myPort;}
    int getMyPortMin() {return myPortMin;}
    int getMyPortMax() {return myPortMax;}

private:
    int myPortMin, myPortMax, myPort;

};



class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    ChatDialog();
    bool cmdForward;
    QTimer *timer_E5;
    NetSocket *mySocket;
    QList<peerlist> peerList;
    peerlist peer;
    ipLookUpHelper *ipLoopupHelper;
    QFileDialog *fileDialog;
    QHash<QString, QPair<QString, quint16> > nextHopTable;
    quint32 hopLimit;

    //file share
    QHash<QByteArray, QVariantMap> metafileList;
    QHash<QByteArray,QByteArray> datafileHash;
    //fileHashedId, currentByte, current number
    QHash<QByteArray,QHash<QByteArray,int> > requestMetabyte;
    //fileHashedId, metafile
    QHash<QByteArray,QByteArray> requestMetafile;
    //fileHashedId, dataByte
    QHash<QByteArray,QByteArray> requestDatafile;
    //Origin, <fileName, fileHashedId>
    QHash<QString,QHash<QString,QByteArray> > downloadSource;
    //fileId, fileName
    QHash<QByteArray,QString> mapIDtoName;
    //fileId
    QHash<QByteArray,int> fileIdList;


    void sendDatagrams();
    void sendBackToSender(QVariantMap msg,QHostAddress sender,quint16 senderPort);
    //void choosePort(QByteArray d);
    void randomPickandSend(QVariantMap message);
  //  void msgProcess();
    int SeqNo;

    QVariantMap localMsg;
    QVariantMap localRoutMsg;
    QMap<QString, QMap<int, QString> > localDataBase;
    void addPeerComamnd(QString ip, quint16 port);
    void insertNextHopTable(QVariantMap msg,QString sender,quint16 senderPort);
    void forwardPriMsg(QVariantMap msg);
    void sendToAll(QVariantMap message);
    void scanAndCatalog(QStringList fileNames);
    void sendToNode(QVariantMap msg, QString destOrigin);
    void readBlockRequest(QVariantMap msg);
    void readBlockReply(QVariantMap msg);
    void readSearchRequest(QVariantMap msg);
    void readSearchReply(QVariantMap msg);

public slots:
    void readPendingDatagrams();
    void choosePort1();
    void sendRouteMsg();
    void sendNewMessage_FlipCoin();
    void lookedUp(const QHostInfo &host);
    void gotReturnPressed();
    void private_dialogue(QListWidgetItem *item);
    void downloadFileFromWidget(QListWidgetItem *item);
    void showPeerlist();
    void showLocalDatabase();
    void shareFileDialog();
    void downloadFile();
    void sendKeyWord();
    void sendKeyWordPeriodic();


private:
    QTextEdit *textview;
    QTextEdit *textline;
    QLineEdit *ipaddrline;
    QLineEdit *nodeId;
    QLineEdit *fileId;
    QTextEdit *fileShare;
    QTextEdit *testDownload;
    QListWidget *listNodes;
    QListWidget *listFiles;
    QTextEdit *fileSearch;
    QString hostName;
    QString keyWord;
    quint32 budget;
    int numRelaventFile;
    QList<QVariantMap>  localMap;


protected:
    bool eventFilter(QObject *obj, QEvent *ev);

};

class PrivDialog : public QDialog
{
    Q_OBJECT

public:
    PrivDialog(QString dest,QString origin, QHash<QString, QPair<QString, quint16> > NextHopTable, NetSocket *MySocket);
    QString destOrigin;
    QString Origin;
    QHash<QString, QPair<QString, quint16> > hopTable;
    NetSocket *mySocket;
    void sendDatagrams();

public slots:
    void gotReturnPressed();

private:
    QTextEdit *textview;
    QLineEdit *textline;
    quint32 hopLimit;

};

// SelectFileDirDialog
//class SFDFileDialog : public QFileDialog
//{
//    Q_OBJECT
//public:
//    SFDFileDialog(QWidget * parent, Qt::WindowFlags flags );
//    SFDFileDialog(QWidget * parent = 0, const QString & caption = QString(),
//                  const QString & directory = QString(),
//                  const QString & filter = QString() );
//    ~SFDFileDialog();

//public slots:
//    void accept ();
//};

//class ShareFile : public QDialog
//{

//}

#endif // PEERSTER_MAIN_HH
