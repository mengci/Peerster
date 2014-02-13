/****************************************************************************
** Meta object code from reading C++ file 'dhtserver.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dhtserver.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dhtserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HostNameLookup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_HostNameLookup[] = {
    "HostNameLookup\0"
};

void HostNameLookup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData HostNameLookup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HostNameLookup::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HostNameLookup,
      qt_meta_data_HostNameLookup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HostNameLookup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HostNameLookup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HostNameLookup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HostNameLookup))
        return static_cast<void*>(const_cast< HostNameLookup*>(this));
    return QObject::qt_metacast(_clname);
}

int HostNameLookup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_DHTServer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      36,   10,   10,   10, 0x0a,
      64,   10,   10,   10, 0x0a,
      93,   10,   10,   10, 0x0a,
     127,  122,   10,   10, 0x0a,
     154,   10,   10,   10, 0x0a,
     178,  171,   10,   10, 0x0a,
     229,  224,   10,   10, 0x0a,
     263,  258,   10,   10, 0x0a,
     299,  294,   10,   10, 0x0a,
     330,   10,   10,   10, 0x0a,
     347,   10,   10,   10, 0x0a,
     371,   10,   10,   10, 0x0a,
     389,   10,   10,   10, 0x0a,
     405,   10,   10,   10, 0x0a,
     440,  434,  426,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DHTServer[] = {
    "DHTServer\0\0keyValInsertionHandler()\0"
    "nodeJoinBtnClickedHandler()\0"
    "deleteKeyBtnClickedHandler()\0"
    "searchKeyBtnClickedHandler()\0host\0"
    "lookedupHandler(QHostInfo)\0receiveMessage()\0"
    "m,ip,p\0sendMessage(QVariantMap,QHostAddress,quint16)\0"
    "succ\0updateSuccessor(QVariantMap)\0"
    "pred\0updatePredecessor(QVariantMap)\0"
    "node\0updateFingerTable(QVariantMap)\0"
    "displayThisDHT()\0neighboursOpenHandler()\0"
    "keysOpenHandler()\0ftOpenHandler()\0"
    "kvCacheOpenHandler()\0QString\0keyId\0"
    "searchFinTable(quint64)\0"
};

void DHTServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DHTServer *_t = static_cast<DHTServer *>(_o);
        switch (_id) {
        case 0: _t->keyValInsertionHandler(); break;
        case 1: _t->nodeJoinBtnClickedHandler(); break;
        case 2: _t->deleteKeyBtnClickedHandler(); break;
        case 3: _t->searchKeyBtnClickedHandler(); break;
        case 4: _t->lookedupHandler((*reinterpret_cast< const QHostInfo(*)>(_a[1]))); break;
        case 5: _t->receiveMessage(); break;
        case 6: _t->sendMessage((*reinterpret_cast< QVariantMap(*)>(_a[1])),(*reinterpret_cast< QHostAddress(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 7: _t->updateSuccessor((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 8: _t->updatePredecessor((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 9: _t->updateFingerTable((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 10: _t->displayThisDHT(); break;
        case 11: _t->neighboursOpenHandler(); break;
        case 12: _t->keysOpenHandler(); break;
        case 13: _t->ftOpenHandler(); break;
        case 14: _t->kvCacheOpenHandler(); break;
        case 15: { QString _r = _t->searchFinTable((*reinterpret_cast< quint64(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DHTServer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DHTServer::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DHTServer,
      qt_meta_data_DHTServer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DHTServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DHTServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DHTServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DHTServer))
        return static_cast<void*>(const_cast< DHTServer*>(this));
    return QDialog::qt_metacast(_clname);
}

int DHTServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
