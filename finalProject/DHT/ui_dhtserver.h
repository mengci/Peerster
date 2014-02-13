/********************************************************************************
** Form generated from reading UI file 'dhtserver.ui'
**
** Created
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DHTSERVER_H
#define UI_DHTSERVER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_DHTServer
{
public:

    void setupUi(QDialog *DHTServer)
    {
        if (DHTServer->objectName().isEmpty())
            DHTServer->setObjectName(QString::fromUtf8("DHTServer"));
        DHTServer->resize(400, 300);

        retranslateUi(DHTServer);

        QMetaObject::connectSlotsByName(DHTServer);
    } // setupUi

    void retranslateUi(QDialog *DHTServer)
    {
        DHTServer->setWindowTitle(QApplication::translate("DHTServer", "DHTServer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DHTServer: public Ui_DHTServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DHTSERVER_H
