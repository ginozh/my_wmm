/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../examples/player/MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[86];
    char stringdata0[1093];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 5), // "ready"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 4), // "play"
QT_MOC_LITERAL(4, 23, 4), // "name"
QT_MOC_LITERAL(5, 28, 3), // "url"
QT_MOC_LITERAL(6, 32, 8), // "openFile"
QT_MOC_LITERAL(7, 41, 15), // "togglePlayPause"
QT_MOC_LITERAL(8, 57, 11), // "showNextOSD"
QT_MOC_LITERAL(9, 69, 10), // "stopUnload"
QT_MOC_LITERAL(10, 80, 5), // "about"
QT_MOC_LITERAL(11, 86, 4), // "help"
QT_MOC_LITERAL(12, 91, 7), // "openUrl"
QT_MOC_LITERAL(13, 99, 18), // "initAudioTrackMenu"
QT_MOC_LITERAL(14, 118, 17), // "updateChannelMenu"
QT_MOC_LITERAL(15, 136, 17), // "switchAspectRatio"
QT_MOC_LITERAL(16, 154, 8), // "QAction*"
QT_MOC_LITERAL(17, 163, 6), // "action"
QT_MOC_LITERAL(18, 170, 12), // "toggleRepeat"
QT_MOC_LITERAL(19, 183, 13), // "setRepeateMax"
QT_MOC_LITERAL(20, 197, 8), // "changeVO"
QT_MOC_LITERAL(21, 206, 13), // "changeChannel"
QT_MOC_LITERAL(22, 220, 16), // "changeAudioTrack"
QT_MOC_LITERAL(23, 237, 13), // "onTVMenuClick"
QT_MOC_LITERAL(24, 251, 15), // "playOnlineVideo"
QT_MOC_LITERAL(25, 267, 15), // "onPlayListClick"
QT_MOC_LITERAL(26, 283, 3), // "key"
QT_MOC_LITERAL(27, 287, 5), // "value"
QT_MOC_LITERAL(28, 293, 21), // "processPendingActions"
QT_MOC_LITERAL(29, 315, 10), // "initPlayer"
QT_MOC_LITERAL(30, 326, 7), // "setupUi"
QT_MOC_LITERAL(31, 334, 16), // "onSpinBoxChanged"
QT_MOC_LITERAL(32, 351, 1), // "v"
QT_MOC_LITERAL(33, 353, 11), // "onStartPlay"
QT_MOC_LITERAL(34, 365, 10), // "onStopPlay"
QT_MOC_LITERAL(35, 376, 8), // "onPaused"
QT_MOC_LITERAL(36, 385, 1), // "p"
QT_MOC_LITERAL(37, 387, 13), // "onSpeedChange"
QT_MOC_LITERAL(38, 401, 5), // "speed"
QT_MOC_LITERAL(39, 407, 12), // "setFrameRate"
QT_MOC_LITERAL(40, 420, 4), // "seek"
QT_MOC_LITERAL(41, 425, 17), // "showHideVolumeBar"
QT_MOC_LITERAL(42, 443, 9), // "setVolume"
QT_MOC_LITERAL(43, 453, 17), // "tryHideControlBar"
QT_MOC_LITERAL(44, 471, 17), // "tryShowControlBar"
QT_MOC_LITERAL(45, 489, 8), // "showInfo"
QT_MOC_LITERAL(46, 498, 16), // "onPositionChange"
QT_MOC_LITERAL(47, 515, 3), // "pos"
QT_MOC_LITERAL(48, 519, 14), // "repeatAChanged"
QT_MOC_LITERAL(49, 534, 1), // "t"
QT_MOC_LITERAL(50, 536, 14), // "repeatBChanged"
QT_MOC_LITERAL(51, 551, 17), // "onTimeSliderHover"
QT_MOC_LITERAL(52, 569, 17), // "onTimeSliderLeave"
QT_MOC_LITERAL(53, 587, 11), // "handleError"
QT_MOC_LITERAL(54, 599, 13), // "QtAV::AVError"
QT_MOC_LITERAL(55, 613, 1), // "e"
QT_MOC_LITERAL(56, 615, 20), // "onMediaStatusChanged"
QT_MOC_LITERAL(57, 636, 16), // "onBufferProgress"
QT_MOC_LITERAL(58, 653, 7), // "percent"
QT_MOC_LITERAL(59, 661, 22), // "onVideoEQEngineChanged"
QT_MOC_LITERAL(60, 684, 19), // "onBrightnessChanged"
QT_MOC_LITERAL(61, 704, 1), // "b"
QT_MOC_LITERAL(62, 706, 17), // "onContrastChanged"
QT_MOC_LITERAL(63, 724, 1), // "c"
QT_MOC_LITERAL(64, 726, 12), // "onHueChanged"
QT_MOC_LITERAL(65, 739, 1), // "h"
QT_MOC_LITERAL(66, 741, 19), // "onSaturationChanged"
QT_MOC_LITERAL(67, 761, 1), // "s"
QT_MOC_LITERAL(68, 763, 14), // "onSeekFinished"
QT_MOC_LITERAL(69, 778, 22), // "onCaptureConfigChanged"
QT_MOC_LITERAL(70, 801, 28), // "onAVFilterVideoConfigChanged"
QT_MOC_LITERAL(71, 830, 28), // "onAVFilterAudioConfigChanged"
QT_MOC_LITERAL(72, 859, 20), // "onBufferValueChanged"
QT_MOC_LITERAL(73, 880, 23), // "onAbortOnTimeoutChanged"
QT_MOC_LITERAL(74, 904, 19), // "onUserShaderChanged"
QT_MOC_LITERAL(75, 924, 6), // "donate"
QT_MOC_LITERAL(76, 931, 5), // "setup"
QT_MOC_LITERAL(77, 937, 22), // "handleFullscreenChange"
QT_MOC_LITERAL(78, 960, 22), // "toggoleSubtitleEnabled"
QT_MOC_LITERAL(79, 983, 22), // "toggleSubtitleAutoLoad"
QT_MOC_LITERAL(80, 1006, 12), // "openSubtitle"
QT_MOC_LITERAL(81, 1019, 18), // "setSubtitleCharset"
QT_MOC_LITERAL(82, 1038, 7), // "charSet"
QT_MOC_LITERAL(83, 1046, 17), // "setSubtitleEngine"
QT_MOC_LITERAL(84, 1064, 15), // "changeClockType"
QT_MOC_LITERAL(85, 1080, 12) // "syncVolumeUi"

    },
    "MainWindow\0ready\0\0play\0name\0url\0"
    "openFile\0togglePlayPause\0showNextOSD\0"
    "stopUnload\0about\0help\0openUrl\0"
    "initAudioTrackMenu\0updateChannelMenu\0"
    "switchAspectRatio\0QAction*\0action\0"
    "toggleRepeat\0setRepeateMax\0changeVO\0"
    "changeChannel\0changeAudioTrack\0"
    "onTVMenuClick\0playOnlineVideo\0"
    "onPlayListClick\0key\0value\0"
    "processPendingActions\0initPlayer\0"
    "setupUi\0onSpinBoxChanged\0v\0onStartPlay\0"
    "onStopPlay\0onPaused\0p\0onSpeedChange\0"
    "speed\0setFrameRate\0seek\0showHideVolumeBar\0"
    "setVolume\0tryHideControlBar\0"
    "tryShowControlBar\0showInfo\0onPositionChange\0"
    "pos\0repeatAChanged\0t\0repeatBChanged\0"
    "onTimeSliderHover\0onTimeSliderLeave\0"
    "handleError\0QtAV::AVError\0e\0"
    "onMediaStatusChanged\0onBufferProgress\0"
    "percent\0onVideoEQEngineChanged\0"
    "onBrightnessChanged\0b\0onContrastChanged\0"
    "c\0onHueChanged\0h\0onSaturationChanged\0"
    "s\0onSeekFinished\0onCaptureConfigChanged\0"
    "onAVFilterVideoConfigChanged\0"
    "onAVFilterAudioConfigChanged\0"
    "onBufferValueChanged\0onAbortOnTimeoutChanged\0"
    "onUserShaderChanged\0donate\0setup\0"
    "handleFullscreenChange\0toggoleSubtitleEnabled\0"
    "toggleSubtitleAutoLoad\0openSubtitle\0"
    "setSubtitleCharset\0charSet\0setSubtitleEngine\0"
    "changeClockType\0syncVolumeUi"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      67,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  349,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,  350,    2, 0x0a /* Public */,
       3,    1,  353,    2, 0x0a /* Public */,
       6,    0,  356,    2, 0x0a /* Public */,
       7,    0,  357,    2, 0x0a /* Public */,
       8,    0,  358,    2, 0x0a /* Public */,
       9,    0,  359,    2, 0x08 /* Private */,
      10,    0,  360,    2, 0x08 /* Private */,
      11,    0,  361,    2, 0x08 /* Private */,
      12,    0,  362,    2, 0x08 /* Private */,
      13,    0,  363,    2, 0x08 /* Private */,
      14,    0,  364,    2, 0x08 /* Private */,
      15,    1,  365,    2, 0x08 /* Private */,
      18,    1,  368,    2, 0x08 /* Private */,
      19,    1,  371,    2, 0x08 /* Private */,
      20,    1,  374,    2, 0x08 /* Private */,
      21,    1,  377,    2, 0x08 /* Private */,
      22,    1,  380,    2, 0x08 /* Private */,
      23,    0,  383,    2, 0x08 /* Private */,
      24,    1,  384,    2, 0x08 /* Private */,
      25,    2,  387,    2, 0x08 /* Private */,
      28,    0,  392,    2, 0x08 /* Private */,
      29,    0,  393,    2, 0x08 /* Private */,
      30,    0,  394,    2, 0x08 /* Private */,
      31,    1,  395,    2, 0x08 /* Private */,
      33,    0,  398,    2, 0x08 /* Private */,
      34,    0,  399,    2, 0x08 /* Private */,
      35,    1,  400,    2, 0x08 /* Private */,
      37,    1,  403,    2, 0x08 /* Private */,
      39,    0,  406,    2, 0x08 /* Private */,
      40,    0,  407,    2, 0x08 /* Private */,
      40,    1,  408,    2, 0x08 /* Private */,
      41,    0,  411,    2, 0x08 /* Private */,
      42,    0,  412,    2, 0x08 /* Private */,
      43,    0,  413,    2, 0x08 /* Private */,
      44,    0,  414,    2, 0x08 /* Private */,
      45,    0,  415,    2, 0x08 /* Private */,
      46,    1,  416,    2, 0x08 /* Private */,
      48,    1,  419,    2, 0x08 /* Private */,
      50,    1,  422,    2, 0x08 /* Private */,
      51,    2,  425,    2, 0x08 /* Private */,
      52,    0,  430,    2, 0x08 /* Private */,
      53,    1,  431,    2, 0x08 /* Private */,
      56,    0,  434,    2, 0x08 /* Private */,
      57,    1,  435,    2, 0x08 /* Private */,
      59,    0,  438,    2, 0x08 /* Private */,
      60,    1,  439,    2, 0x08 /* Private */,
      62,    1,  442,    2, 0x08 /* Private */,
      64,    1,  445,    2, 0x08 /* Private */,
      66,    1,  448,    2, 0x08 /* Private */,
      68,    1,  451,    2, 0x08 /* Private */,
      69,    0,  454,    2, 0x08 /* Private */,
      70,    0,  455,    2, 0x08 /* Private */,
      71,    0,  456,    2, 0x08 /* Private */,
      72,    0,  457,    2, 0x08 /* Private */,
      73,    0,  458,    2, 0x08 /* Private */,
      74,    0,  459,    2, 0x08 /* Private */,
      75,    0,  460,    2, 0x08 /* Private */,
      76,    0,  461,    2, 0x08 /* Private */,
      77,    0,  462,    2, 0x08 /* Private */,
      78,    1,  463,    2, 0x08 /* Private */,
      79,    1,  466,    2, 0x08 /* Private */,
      80,    0,  469,    2, 0x08 /* Private */,
      81,    1,  470,    2, 0x08 /* Private */,
      83,    1,  473,    2, 0x08 /* Private */,
      84,    1,  476,    2, 0x08 /* Private */,
      85,    1,  479,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QUrl,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   26,   27,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   36,
    QMetaType::Void, QMetaType::QReal,   38,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   47,
    QMetaType::Void, QMetaType::QTime,   49,
    QMetaType::Void, QMetaType::QTime,   49,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   47,   27,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 54,   55,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal,   58,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   61,
    QMetaType::Void, QMetaType::Int,   63,
    QMetaType::Void, QMetaType::Int,   65,
    QMetaType::Void, QMetaType::Int,   67,
    QMetaType::Void, QMetaType::LongLong,   47,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   27,
    QMetaType::Void, QMetaType::Bool,   27,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   82,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, QMetaType::QReal,   27,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ready(); break;
        case 1: _t->play((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->play((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 3: _t->openFile(); break;
        case 4: _t->togglePlayPause(); break;
        case 5: _t->showNextOSD(); break;
        case 6: _t->stopUnload(); break;
        case 7: _t->about(); break;
        case 8: _t->help(); break;
        case 9: _t->openUrl(); break;
        case 10: _t->initAudioTrackMenu(); break;
        case 11: _t->updateChannelMenu(); break;
        case 12: _t->switchAspectRatio((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 13: _t->toggleRepeat((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->setRepeateMax((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->changeVO((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 16: _t->changeChannel((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 17: _t->changeAudioTrack((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 18: _t->onTVMenuClick(); break;
        case 19: _t->playOnlineVideo((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 20: _t->onPlayListClick((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 21: _t->processPendingActions(); break;
        case 22: _t->initPlayer(); break;
        case 23: _t->setupUi(); break;
        case 24: _t->onSpinBoxChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 25: _t->onStartPlay(); break;
        case 26: _t->onStopPlay(); break;
        case 27: _t->onPaused((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: _t->onSpeedChange((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 29: _t->setFrameRate(); break;
        case 30: _t->seek(); break;
        case 31: _t->seek((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->showHideVolumeBar(); break;
        case 33: _t->setVolume(); break;
        case 34: _t->tryHideControlBar(); break;
        case 35: _t->tryShowControlBar(); break;
        case 36: _t->showInfo(); break;
        case 37: _t->onPositionChange((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 38: _t->repeatAChanged((*reinterpret_cast< const QTime(*)>(_a[1]))); break;
        case 39: _t->repeatBChanged((*reinterpret_cast< const QTime(*)>(_a[1]))); break;
        case 40: _t->onTimeSliderHover((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 41: _t->onTimeSliderLeave(); break;
        case 42: _t->handleError((*reinterpret_cast< const QtAV::AVError(*)>(_a[1]))); break;
        case 43: _t->onMediaStatusChanged(); break;
        case 44: _t->onBufferProgress((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 45: _t->onVideoEQEngineChanged(); break;
        case 46: _t->onBrightnessChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 47: _t->onContrastChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 48: _t->onHueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 49: _t->onSaturationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 50: _t->onSeekFinished((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 51: _t->onCaptureConfigChanged(); break;
        case 52: _t->onAVFilterVideoConfigChanged(); break;
        case 53: _t->onAVFilterAudioConfigChanged(); break;
        case 54: _t->onBufferValueChanged(); break;
        case 55: _t->onAbortOnTimeoutChanged(); break;
        case 56: _t->onUserShaderChanged(); break;
        case 57: _t->donate(); break;
        case 58: _t->setup(); break;
        case 59: _t->handleFullscreenChange(); break;
        case 60: _t->toggoleSubtitleEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 61: _t->toggleSubtitleAutoLoad((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 62: _t->openSubtitle(); break;
        case 63: _t->setSubtitleCharset((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 64: _t->setSubtitleEngine((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 65: _t->changeClockType((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 66: _t->syncVolumeUi((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::ready)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 67)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 67;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 67)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 67;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::ready()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
