/****************************************************************************
** Meta object code from reading C++ file 'config_widget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../config_widget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'config_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSconfig_widgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSconfig_widgetENDCLASS = QtMocHelpers::stringData(
    "config_widget",
    "SignalStartRender",
    "",
    "SignalStopRender",
    "SignalChangeModel",
    "config_save",
    "return_default",
    "software_restart",
    "StartRender",
    "StopRender"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSconfig_widgetENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[14];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[17];
    char stringdata4[18];
    char stringdata5[12];
    char stringdata6[15];
    char stringdata7[17];
    char stringdata8[12];
    char stringdata9[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSconfig_widgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSconfig_widgetENDCLASS_t qt_meta_stringdata_CLASSconfig_widgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 13),  // "config_widget"
        QT_MOC_LITERAL(14, 17),  // "SignalStartRender"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 16),  // "SignalStopRender"
        QT_MOC_LITERAL(50, 17),  // "SignalChangeModel"
        QT_MOC_LITERAL(68, 11),  // "config_save"
        QT_MOC_LITERAL(80, 14),  // "return_default"
        QT_MOC_LITERAL(95, 16),  // "software_restart"
        QT_MOC_LITERAL(112, 11),  // "StartRender"
        QT_MOC_LITERAL(124, 10)   // "StopRender"
    },
    "config_widget",
    "SignalStartRender",
    "",
    "SignalStopRender",
    "SignalChangeModel",
    "config_save",
    "return_default",
    "software_restart",
    "StartRender",
    "StopRender"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSconfig_widgetENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x06,    1 /* Public */,
       3,    0,   63,    2, 0x06,    2 /* Public */,
       4,    0,   64,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   65,    2, 0x0a,    4 /* Public */,
       6,    0,   66,    2, 0x0a,    5 /* Public */,
       7,    0,   67,    2, 0x0a,    6 /* Public */,
       8,    0,   68,    2, 0x0a,    7 /* Public */,
       9,    0,   69,    2, 0x0a,    8 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject config_widget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSconfig_widgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSconfig_widgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSconfig_widgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<config_widget, std::true_type>,
        // method 'SignalStartRender'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SignalStopRender'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SignalChangeModel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'config_save'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'return_default'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'software_restart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'StartRender'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'StopRender'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void config_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<config_widget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->SignalStartRender(); break;
        case 1: _t->SignalStopRender(); break;
        case 2: _t->SignalChangeModel(); break;
        case 3: _t->config_save(); break;
        case 4: _t->return_default(); break;
        case 5: _t->software_restart(); break;
        case 6: _t->StartRender(); break;
        case 7: _t->StopRender(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (config_widget::*)();
            if (_t _q_method = &config_widget::SignalStartRender; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (config_widget::*)();
            if (_t _q_method = &config_widget::SignalStopRender; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (config_widget::*)();
            if (_t _q_method = &config_widget::SignalChangeModel; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *config_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *config_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSconfig_widgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int config_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void config_widget::SignalStartRender()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void config_widget::SignalStopRender()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void config_widget::SignalChangeModel()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSconfig_cardENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSconfig_cardENDCLASS = QtMocHelpers::stringData(
    "config_card"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSconfig_cardENDCLASS_t {
    uint offsetsAndSizes[2];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSconfig_cardENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSconfig_cardENDCLASS_t qt_meta_stringdata_CLASSconfig_cardENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11)   // "config_card"
    },
    "config_card"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSconfig_cardENDCLASS[] = {

 // content:
      11,       // revision
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

Q_CONSTINIT const QMetaObject config_card::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSconfig_cardENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSconfig_cardENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSconfig_cardENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<config_card, std::true_type>
    >,
    nullptr
} };

void config_card::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *config_card::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *config_card::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSconfig_cardENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int config_card::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSAppWidgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSAppWidgetENDCLASS = QtMocHelpers::stringData(
    "AppWidget",
    "folder_choose",
    ""
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSAppWidgetENDCLASS_t {
    uint offsetsAndSizes[6];
    char stringdata0[10];
    char stringdata1[14];
    char stringdata2[1];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSAppWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSAppWidgetENDCLASS_t qt_meta_stringdata_CLASSAppWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 9),  // "AppWidget"
        QT_MOC_LITERAL(10, 13),  // "folder_choose"
        QT_MOC_LITERAL(24, 0)   // ""
    },
    "AppWidget",
    "folder_choose",
    ""
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSAppWidgetENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   20,    2, 0x0a,    1 /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject AppWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<config_card::staticMetaObject>(),
    qt_meta_stringdata_CLASSAppWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSAppWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSAppWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AppWidget, std::true_type>,
        // method 'folder_choose'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void AppWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AppWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->folder_choose(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *AppWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSAppWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return config_card::qt_metacast(_clname);
}

int AppWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = config_card::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSModelWidgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSModelWidgetENDCLASS = QtMocHelpers::stringData(
    "ModelWidget",
    "QuickkeyUpdate",
    "",
    "clicked",
    "expression_israndom",
    "motion_israndom"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSModelWidgetENDCLASS_t {
    uint offsetsAndSizes[12];
    char stringdata0[12];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[20];
    char stringdata5[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSModelWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSModelWidgetENDCLASS_t qt_meta_stringdata_CLASSModelWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "ModelWidget"
        QT_MOC_LITERAL(12, 14),  // "QuickkeyUpdate"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 7),  // "clicked"
        QT_MOC_LITERAL(36, 19),  // "expression_israndom"
        QT_MOC_LITERAL(56, 15)   // "motion_israndom"
    },
    "ModelWidget",
    "QuickkeyUpdate",
    "",
    "clicked",
    "expression_israndom",
    "motion_israndom"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSModelWidgetENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x0a,    1 /* Public */,
       4,    1,   35,    2, 0x0a,    3 /* Public */,
       5,    1,   38,    2, 0x0a,    5 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject ModelWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<config_card::staticMetaObject>(),
    qt_meta_stringdata_CLASSModelWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSModelWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSModelWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ModelWidget, std::true_type>,
        // method 'QuickkeyUpdate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'expression_israndom'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'motion_israndom'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void ModelWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModelWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->QuickkeyUpdate((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->expression_israndom((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->motion_israndom((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *ModelWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModelWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSModelWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return config_card::qt_metacast(_clname);
}

int ModelWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = config_card::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
