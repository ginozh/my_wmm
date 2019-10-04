/****************************************************************************
** Meta object code from reading C++ file 'AVPlayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/QtAV/AVPlayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AVPlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtAV__AVPlayer_t {
    QByteArrayData data[138];
    char stringdata0[1786];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtAV__AVPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtAV__AVPlayer_t qt_meta_stringdata_QtAV__AVPlayer = {
    {
QT_MOC_LITERAL(0, 0, 14), // "QtAV::AVPlayer"
QT_MOC_LITERAL(1, 15, 21), // "bufferProgressChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 23), // "relativeTimeModeChanged"
QT_MOC_LITERAL(4, 62, 15), // "autoLoadChanged"
QT_MOC_LITERAL(5, 78, 16), // "asyncLoadChanged"
QT_MOC_LITERAL(6, 95, 11), // "muteChanged"
QT_MOC_LITERAL(7, 107, 13), // "sourceChanged"
QT_MOC_LITERAL(8, 121, 6), // "loaded"
QT_MOC_LITERAL(9, 128, 18), // "mediaStatusChanged"
QT_MOC_LITERAL(10, 147, 17), // "QtAV::MediaStatus"
QT_MOC_LITERAL(11, 165, 6), // "status"
QT_MOC_LITERAL(12, 172, 21), // "mediaEndActionChanged"
QT_MOC_LITERAL(13, 194, 20), // "QtAV::MediaEndAction"
QT_MOC_LITERAL(14, 215, 6), // "action"
QT_MOC_LITERAL(15, 222, 15), // "durationChanged"
QT_MOC_LITERAL(16, 238, 5), // "error"
QT_MOC_LITERAL(17, 244, 13), // "QtAV::AVError"
QT_MOC_LITERAL(18, 258, 1), // "e"
QT_MOC_LITERAL(19, 260, 6), // "paused"
QT_MOC_LITERAL(20, 267, 1), // "p"
QT_MOC_LITERAL(21, 269, 7), // "started"
QT_MOC_LITERAL(22, 277, 7), // "stopped"
QT_MOC_LITERAL(23, 285, 9), // "stoppedAt"
QT_MOC_LITERAL(24, 295, 8), // "position"
QT_MOC_LITERAL(25, 304, 12), // "stateChanged"
QT_MOC_LITERAL(26, 317, 21), // "QtAV::AVPlayer::State"
QT_MOC_LITERAL(27, 339, 5), // "state"
QT_MOC_LITERAL(28, 345, 12), // "speedChanged"
QT_MOC_LITERAL(29, 358, 5), // "speed"
QT_MOC_LITERAL(30, 364, 13), // "repeatChanged"
QT_MOC_LITERAL(31, 378, 1), // "r"
QT_MOC_LITERAL(32, 380, 20), // "currentRepeatChanged"
QT_MOC_LITERAL(33, 401, 20), // "startPositionChanged"
QT_MOC_LITERAL(34, 422, 19), // "stopPositionChanged"
QT_MOC_LITERAL(35, 442, 15), // "seekableChanged"
QT_MOC_LITERAL(36, 458, 12), // "seekFinished"
QT_MOC_LITERAL(37, 471, 15), // "positionChanged"
QT_MOC_LITERAL(38, 487, 23), // "interruptTimeoutChanged"
QT_MOC_LITERAL(39, 511, 25), // "interruptOnTimeoutChanged"
QT_MOC_LITERAL(40, 537, 21), // "notifyIntervalChanged"
QT_MOC_LITERAL(41, 559, 17), // "brightnessChanged"
QT_MOC_LITERAL(42, 577, 3), // "val"
QT_MOC_LITERAL(43, 581, 15), // "contrastChanged"
QT_MOC_LITERAL(44, 597, 10), // "hueChanged"
QT_MOC_LITERAL(45, 608, 17), // "saturationChanged"
QT_MOC_LITERAL(46, 626, 15), // "chaptersChanged"
QT_MOC_LITERAL(47, 642, 21), // "subtitleStreamChanged"
QT_MOC_LITERAL(48, 664, 5), // "value"
QT_MOC_LITERAL(49, 670, 26), // "internalAudioTracksChanged"
QT_MOC_LITERAL(50, 697, 6), // "tracks"
QT_MOC_LITERAL(51, 704, 26), // "internalVideoTracksChanged"
QT_MOC_LITERAL(52, 731, 26), // "externalAudioTracksChanged"
QT_MOC_LITERAL(53, 758, 29), // "internalSubtitleTracksChanged"
QT_MOC_LITERAL(54, 788, 26), // "internalSubtitleHeaderRead"
QT_MOC_LITERAL(55, 815, 5), // "codec"
QT_MOC_LITERAL(56, 821, 4), // "data"
QT_MOC_LITERAL(57, 826, 26), // "internalSubtitlePacketRead"
QT_MOC_LITERAL(58, 853, 5), // "track"
QT_MOC_LITERAL(59, 859, 12), // "QtAV::Packet"
QT_MOC_LITERAL(60, 872, 6), // "packet"
QT_MOC_LITERAL(61, 879, 4), // "load"
QT_MOC_LITERAL(62, 884, 11), // "togglePause"
QT_MOC_LITERAL(63, 896, 5), // "pause"
QT_MOC_LITERAL(64, 902, 4), // "play"
QT_MOC_LITERAL(65, 907, 4), // "stop"
QT_MOC_LITERAL(66, 912, 11), // "stepForward"
QT_MOC_LITERAL(67, 924, 12), // "stepBackward"
QT_MOC_LITERAL(68, 937, 19), // "setRelativeTimeMode"
QT_MOC_LITERAL(69, 957, 9), // "setRepeat"
QT_MOC_LITERAL(70, 967, 3), // "max"
QT_MOC_LITERAL(71, 971, 16), // "setStartPosition"
QT_MOC_LITERAL(72, 988, 3), // "pos"
QT_MOC_LITERAL(73, 992, 15), // "setStopPosition"
QT_MOC_LITERAL(74, 1008, 12), // "setTimeRange"
QT_MOC_LITERAL(75, 1021, 5), // "start"
QT_MOC_LITERAL(76, 1027, 10), // "isSeekable"
QT_MOC_LITERAL(77, 1038, 11), // "setPosition"
QT_MOC_LITERAL(78, 1050, 4), // "seek"
QT_MOC_LITERAL(79, 1055, 11), // "seekForward"
QT_MOC_LITERAL(80, 1067, 12), // "seekBackward"
QT_MOC_LITERAL(81, 1080, 15), // "seekNextChapter"
QT_MOC_LITERAL(82, 1096, 19), // "seekPreviousChapter"
QT_MOC_LITERAL(83, 1116, 11), // "setSeekType"
QT_MOC_LITERAL(84, 1128, 8), // "SeekType"
QT_MOC_LITERAL(85, 1137, 4), // "type"
QT_MOC_LITERAL(86, 1142, 8), // "seekType"
QT_MOC_LITERAL(87, 1151, 14), // "bufferProgress"
QT_MOC_LITERAL(88, 1166, 11), // "bufferSpeed"
QT_MOC_LITERAL(89, 1178, 8), // "buffered"
QT_MOC_LITERAL(90, 1187, 13), // "setBufferMode"
QT_MOC_LITERAL(91, 1201, 10), // "BufferMode"
QT_MOC_LITERAL(92, 1212, 4), // "mode"
QT_MOC_LITERAL(93, 1217, 10), // "bufferMode"
QT_MOC_LITERAL(94, 1228, 14), // "setBufferValue"
QT_MOC_LITERAL(95, 1243, 11), // "bufferValue"
QT_MOC_LITERAL(96, 1255, 17), // "setNotifyInterval"
QT_MOC_LITERAL(97, 1273, 4), // "msec"
QT_MOC_LITERAL(98, 1278, 14), // "notifyInterval"
QT_MOC_LITERAL(99, 1293, 11), // "updateClock"
QT_MOC_LITERAL(100, 1305, 5), // "msecs"
QT_MOC_LITERAL(101, 1311, 13), // "setBrightness"
QT_MOC_LITERAL(102, 1325, 11), // "setContrast"
QT_MOC_LITERAL(103, 1337, 6), // "setHue"
QT_MOC_LITERAL(104, 1344, 13), // "setSaturation"
QT_MOC_LITERAL(105, 1358, 12), // "loadInternal"
QT_MOC_LITERAL(106, 1371, 12), // "playInternal"
QT_MOC_LITERAL(107, 1384, 21), // "stopFromDemuxerThread"
QT_MOC_LITERAL(108, 1406, 14), // "aboutToQuitApp"
QT_MOC_LITERAL(109, 1421, 16), // "startNotifyTimer"
QT_MOC_LITERAL(110, 1438, 15), // "stopNotifyTimer"
QT_MOC_LITERAL(111, 1454, 9), // "onStarted"
QT_MOC_LITERAL(112, 1464, 17), // "updateMediaStatus"
QT_MOC_LITERAL(113, 1482, 14), // "onSeekFinished"
QT_MOC_LITERAL(114, 1497, 22), // "tryClearVideoRenderers"
QT_MOC_LITERAL(115, 1520, 11), // "seekChapter"
QT_MOC_LITERAL(116, 1532, 4), // "incr"
QT_MOC_LITERAL(117, 1537, 16), // "relativeTimeMode"
QT_MOC_LITERAL(118, 1554, 8), // "autoLoad"
QT_MOC_LITERAL(119, 1563, 9), // "asyncLoad"
QT_MOC_LITERAL(120, 1573, 8), // "seekable"
QT_MOC_LITERAL(121, 1582, 8), // "duration"
QT_MOC_LITERAL(122, 1591, 13), // "startPosition"
QT_MOC_LITERAL(123, 1605, 12), // "stopPosition"
QT_MOC_LITERAL(124, 1618, 6), // "repeat"
QT_MOC_LITERAL(125, 1625, 13), // "currentRepeat"
QT_MOC_LITERAL(126, 1639, 16), // "interruptTimeout"
QT_MOC_LITERAL(127, 1656, 18), // "interruptOnTimeout"
QT_MOC_LITERAL(128, 1675, 10), // "brightness"
QT_MOC_LITERAL(129, 1686, 8), // "contrast"
QT_MOC_LITERAL(130, 1695, 10), // "saturation"
QT_MOC_LITERAL(131, 1706, 5), // "State"
QT_MOC_LITERAL(132, 1712, 11), // "mediaStatus"
QT_MOC_LITERAL(133, 1724, 14), // "mediaEndAction"
QT_MOC_LITERAL(134, 1739, 8), // "chapters"
QT_MOC_LITERAL(135, 1748, 12), // "StoppedState"
QT_MOC_LITERAL(136, 1761, 12), // "PlayingState"
QT_MOC_LITERAL(137, 1774, 11) // "PausedState"

    },
    "QtAV::AVPlayer\0bufferProgressChanged\0"
    "\0relativeTimeModeChanged\0autoLoadChanged\0"
    "asyncLoadChanged\0muteChanged\0sourceChanged\0"
    "loaded\0mediaStatusChanged\0QtAV::MediaStatus\0"
    "status\0mediaEndActionChanged\0"
    "QtAV::MediaEndAction\0action\0durationChanged\0"
    "error\0QtAV::AVError\0e\0paused\0p\0started\0"
    "stopped\0stoppedAt\0position\0stateChanged\0"
    "QtAV::AVPlayer::State\0state\0speedChanged\0"
    "speed\0repeatChanged\0r\0currentRepeatChanged\0"
    "startPositionChanged\0stopPositionChanged\0"
    "seekableChanged\0seekFinished\0"
    "positionChanged\0interruptTimeoutChanged\0"
    "interruptOnTimeoutChanged\0"
    "notifyIntervalChanged\0brightnessChanged\0"
    "val\0contrastChanged\0hueChanged\0"
    "saturationChanged\0chaptersChanged\0"
    "subtitleStreamChanged\0value\0"
    "internalAudioTracksChanged\0tracks\0"
    "internalVideoTracksChanged\0"
    "externalAudioTracksChanged\0"
    "internalSubtitleTracksChanged\0"
    "internalSubtitleHeaderRead\0codec\0data\0"
    "internalSubtitlePacketRead\0track\0"
    "QtAV::Packet\0packet\0load\0togglePause\0"
    "pause\0play\0stop\0stepForward\0stepBackward\0"
    "setRelativeTimeMode\0setRepeat\0max\0"
    "setStartPosition\0pos\0setStopPosition\0"
    "setTimeRange\0start\0isSeekable\0setPosition\0"
    "seek\0seekForward\0seekBackward\0"
    "seekNextChapter\0seekPreviousChapter\0"
    "setSeekType\0SeekType\0type\0seekType\0"
    "bufferProgress\0bufferSpeed\0buffered\0"
    "setBufferMode\0BufferMode\0mode\0bufferMode\0"
    "setBufferValue\0bufferValue\0setNotifyInterval\0"
    "msec\0notifyInterval\0updateClock\0msecs\0"
    "setBrightness\0setContrast\0setHue\0"
    "setSaturation\0loadInternal\0playInternal\0"
    "stopFromDemuxerThread\0aboutToQuitApp\0"
    "startNotifyTimer\0stopNotifyTimer\0"
    "onStarted\0updateMediaStatus\0onSeekFinished\0"
    "tryClearVideoRenderers\0seekChapter\0"
    "incr\0relativeTimeMode\0autoLoad\0asyncLoad\0"
    "seekable\0duration\0startPosition\0"
    "stopPosition\0repeat\0currentRepeat\0"
    "interruptTimeout\0interruptOnTimeout\0"
    "brightness\0contrast\0saturation\0State\0"
    "mediaStatus\0mediaEndAction\0chapters\0"
    "StoppedState\0PlayingState\0PausedState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtAV__AVPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      89,   14, // methods
      21,  652, // properties
       1,  736, // enums/sets
       0,    0, // constructors
       0,       // flags
      39,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  459,    2, 0x06 /* Public */,
       3,    0,  462,    2, 0x06 /* Public */,
       4,    0,  463,    2, 0x06 /* Public */,
       5,    0,  464,    2, 0x06 /* Public */,
       6,    0,  465,    2, 0x06 /* Public */,
       7,    0,  466,    2, 0x06 /* Public */,
       8,    0,  467,    2, 0x06 /* Public */,
       9,    1,  468,    2, 0x06 /* Public */,
      12,    1,  471,    2, 0x06 /* Public */,
      15,    1,  474,    2, 0x06 /* Public */,
      16,    1,  477,    2, 0x06 /* Public */,
      19,    1,  480,    2, 0x06 /* Public */,
      21,    0,  483,    2, 0x06 /* Public */,
      22,    0,  484,    2, 0x06 /* Public */,
      23,    1,  485,    2, 0x06 /* Public */,
      25,    1,  488,    2, 0x06 /* Public */,
      28,    1,  491,    2, 0x06 /* Public */,
      30,    1,  494,    2, 0x06 /* Public */,
      32,    1,  497,    2, 0x06 /* Public */,
      33,    1,  500,    2, 0x06 /* Public */,
      34,    1,  503,    2, 0x06 /* Public */,
      35,    0,  506,    2, 0x06 /* Public */,
      36,    1,  507,    2, 0x06 /* Public */,
      37,    1,  510,    2, 0x06 /* Public */,
      38,    0,  513,    2, 0x06 /* Public */,
      39,    0,  514,    2, 0x06 /* Public */,
      40,    0,  515,    2, 0x06 /* Public */,
      41,    1,  516,    2, 0x06 /* Public */,
      43,    1,  519,    2, 0x06 /* Public */,
      44,    1,  522,    2, 0x06 /* Public */,
      45,    1,  525,    2, 0x06 /* Public */,
      46,    1,  528,    2, 0x06 /* Public */,
      47,    1,  531,    2, 0x06 /* Public */,
      49,    1,  534,    2, 0x06 /* Public */,
      51,    1,  537,    2, 0x06 /* Public */,
      52,    1,  540,    2, 0x06 /* Public */,
      53,    1,  543,    2, 0x06 /* Public */,
      54,    2,  546,    2, 0x06 /* Public */,
      57,    2,  551,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      61,    0,  556,    2, 0x0a /* Public */,
      62,    0,  557,    2, 0x0a /* Public */,
      63,    1,  558,    2, 0x0a /* Public */,
      63,    0,  561,    2, 0x2a /* Public | MethodCloned */,
      64,    0,  562,    2, 0x0a /* Public */,
      65,    0,  563,    2, 0x0a /* Public */,
      66,    0,  564,    2, 0x0a /* Public */,
      67,    0,  565,    2, 0x0a /* Public */,
      68,    1,  566,    2, 0x0a /* Public */,
      69,    1,  569,    2, 0x0a /* Public */,
      71,    1,  572,    2, 0x0a /* Public */,
      73,    1,  575,    2, 0x0a /* Public */,
      73,    0,  578,    2, 0x2a /* Public | MethodCloned */,
      74,    2,  579,    2, 0x0a /* Public */,
      74,    1,  584,    2, 0x2a /* Public | MethodCloned */,
      76,    0,  587,    2, 0x0a /* Public */,
      77,    1,  588,    2, 0x0a /* Public */,
      78,    1,  591,    2, 0x0a /* Public */,
      78,    1,  594,    2, 0x0a /* Public */,
      79,    0,  597,    2, 0x0a /* Public */,
      80,    0,  598,    2, 0x0a /* Public */,
      81,    0,  599,    2, 0x0a /* Public */,
      82,    0,  600,    2, 0x0a /* Public */,
      83,    1,  601,    2, 0x0a /* Public */,
      86,    0,  604,    2, 0x0a /* Public */,
      87,    0,  605,    2, 0x0a /* Public */,
      88,    0,  606,    2, 0x0a /* Public */,
      89,    0,  607,    2, 0x0a /* Public */,
      90,    1,  608,    2, 0x0a /* Public */,
      93,    0,  611,    2, 0x0a /* Public */,
      94,    1,  612,    2, 0x0a /* Public */,
      95,    0,  615,    2, 0x0a /* Public */,
      96,    1,  616,    2, 0x0a /* Public */,
      98,    0,  619,    2, 0x0a /* Public */,
      99,    1,  620,    2, 0x0a /* Public */,
     101,    1,  623,    2, 0x0a /* Public */,
     102,    1,  626,    2, 0x0a /* Public */,
     103,    1,  629,    2, 0x0a /* Public */,
     104,    1,  632,    2, 0x0a /* Public */,
     105,    0,  635,    2, 0x08 /* Private */,
     106,    0,  636,    2, 0x08 /* Private */,
     107,    0,  637,    2, 0x08 /* Private */,
     108,    0,  638,    2, 0x08 /* Private */,
     109,    0,  639,    2, 0x08 /* Private */,
     110,    0,  640,    2, 0x08 /* Private */,
     111,    0,  641,    2, 0x08 /* Private */,
     112,    1,  642,    2, 0x08 /* Private */,
     113,    1,  645,    2, 0x08 /* Private */,
     114,    0,  648,    2, 0x08 /* Private */,
     115,    1,  649,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QReal,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::LongLong,    2,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, QMetaType::Bool,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   24,
    QMetaType::Void, 0x80000000 | 26,   27,
    QMetaType::Void, QMetaType::QReal,   29,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void, QMetaType::LongLong,   24,
    QMetaType::Void, QMetaType::LongLong,   24,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   24,
    QMetaType::Void, QMetaType::LongLong,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, QMetaType::UInt,   42,
    QMetaType::Void, QMetaType::Int,   48,
    QMetaType::Void, QMetaType::QVariantList,   50,
    QMetaType::Void, QMetaType::QVariantList,   50,
    QMetaType::Void, QMetaType::QVariantList,   50,
    QMetaType::Void, QMetaType::QVariantList,   50,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::QByteArray,   55,   56,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 59,   58,   60,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   48,
    QMetaType::Void, QMetaType::Int,   70,
    QMetaType::Void, QMetaType::LongLong,   72,
    QMetaType::Void, QMetaType::LongLong,   72,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,   75,   65,
    QMetaType::Void, QMetaType::LongLong,   75,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::LongLong,   24,
    QMetaType::Void, QMetaType::QReal,   31,
    QMetaType::Void, QMetaType::LongLong,   72,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 84,   85,
    0x80000000 | 84,
    QMetaType::QReal,
    QMetaType::QReal,
    QMetaType::LongLong,
    QMetaType::Void, 0x80000000 | 91,   92,
    0x80000000 | 91,
    QMetaType::Void, QMetaType::LongLong,   48,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,   97,
    QMetaType::Int,
    QMetaType::Void, QMetaType::LongLong,  100,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void, QMetaType::Int,   42,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::LongLong,   48,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,  116,

 // properties: name, type, flags
     117, QMetaType::Bool, 0x00495103,
     118, QMetaType::Bool, 0x00495103,
     119, QMetaType::Bool, 0x00495103,
      87, QMetaType::QReal, 0x00495001,
     120, QMetaType::Bool, 0x00495001,
     121, QMetaType::LongLong, 0x00495001,
      24, QMetaType::LongLong, 0x00495103,
     122, QMetaType::LongLong, 0x00495103,
     123, QMetaType::LongLong, 0x00495103,
     124, QMetaType::LongLong, 0x00495103,
     125, QMetaType::Int, 0x00495001,
     126, QMetaType::LongLong, 0x00495103,
     127, QMetaType::Bool, 0x00495103,
      98, QMetaType::Int, 0x00495103,
     128, QMetaType::Int, 0x00495103,
     129, QMetaType::Int, 0x00495103,
     130, QMetaType::Int, 0x00495103,
      27, 0x80000000 | 131, 0x0049510b,
     132, 0x80000000 | 10, 0x00495009,
     133, 0x80000000 | 13, 0x0049510b,
     134, QMetaType::UInt, 0x00495001,

 // properties: notify_signal_id
       1,
       2,
       3,
       0,
      21,
       9,
      23,
      19,
      20,
      17,
      18,
      24,
      25,
      26,
      27,
      28,
      30,
      15,
       7,
       8,
      31,

 // enums: name, flags, count, data
     131, 0x0,    3,  740,

 // enum data: key, value
     135, uint(QtAV::AVPlayer::StoppedState),
     136, uint(QtAV::AVPlayer::PlayingState),
     137, uint(QtAV::AVPlayer::PausedState),

       0        // eod
};

void QtAV::AVPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AVPlayer *_t = static_cast<AVPlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->bufferProgressChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: _t->relativeTimeModeChanged(); break;
        case 2: _t->autoLoadChanged(); break;
        case 3: _t->asyncLoadChanged(); break;
        case 4: _t->muteChanged(); break;
        case 5: _t->sourceChanged(); break;
        case 6: _t->loaded(); break;
        case 7: _t->mediaStatusChanged((*reinterpret_cast< QtAV::MediaStatus(*)>(_a[1]))); break;
        case 8: _t->mediaEndActionChanged((*reinterpret_cast< QtAV::MediaEndAction(*)>(_a[1]))); break;
        case 9: _t->durationChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 10: _t->error((*reinterpret_cast< const QtAV::AVError(*)>(_a[1]))); break;
        case 11: _t->paused((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->started(); break;
        case 13: _t->stopped(); break;
        case 14: _t->stoppedAt((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 15: _t->stateChanged((*reinterpret_cast< QtAV::AVPlayer::State(*)>(_a[1]))); break;
        case 16: _t->speedChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 17: _t->repeatChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->currentRepeatChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->startPositionChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 20: _t->stopPositionChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 21: _t->seekableChanged(); break;
        case 22: _t->seekFinished((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 23: _t->positionChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 24: _t->interruptTimeoutChanged(); break;
        case 25: _t->interruptOnTimeoutChanged(); break;
        case 26: _t->notifyIntervalChanged(); break;
        case 27: _t->brightnessChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->contrastChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->hueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->saturationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->chaptersChanged((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 32: _t->subtitleStreamChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->internalAudioTracksChanged((*reinterpret_cast< const QVariantList(*)>(_a[1]))); break;
        case 34: _t->internalVideoTracksChanged((*reinterpret_cast< const QVariantList(*)>(_a[1]))); break;
        case 35: _t->externalAudioTracksChanged((*reinterpret_cast< const QVariantList(*)>(_a[1]))); break;
        case 36: _t->internalSubtitleTracksChanged((*reinterpret_cast< const QVariantList(*)>(_a[1]))); break;
        case 37: _t->internalSubtitleHeaderRead((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 38: _t->internalSubtitlePacketRead((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QtAV::Packet(*)>(_a[2]))); break;
        case 39: { bool _r = _t->load();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 40: _t->togglePause(); break;
        case 41: _t->pause((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 42: _t->pause(); break;
        case 43: _t->play(); break;
        case 44: _t->stop(); break;
        case 45: _t->stepForward(); break;
        case 46: _t->stepBackward(); break;
        case 47: _t->setRelativeTimeMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 48: _t->setRepeat((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 49: _t->setStartPosition((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 50: _t->setStopPosition((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 51: _t->setStopPosition(); break;
        case 52: _t->setTimeRange((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 53: _t->setTimeRange((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 54: { bool _r = _t->isSeekable();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 55: _t->setPosition((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 56: _t->seek((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 57: _t->seek((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 58: _t->seekForward(); break;
        case 59: _t->seekBackward(); break;
        case 60: _t->seekNextChapter(); break;
        case 61: _t->seekPreviousChapter(); break;
        case 62: _t->setSeekType((*reinterpret_cast< SeekType(*)>(_a[1]))); break;
        case 63: { SeekType _r = _t->seekType();
            if (_a[0]) *reinterpret_cast< SeekType*>(_a[0]) = std::move(_r); }  break;
        case 64: { qreal _r = _t->bufferProgress();
            if (_a[0]) *reinterpret_cast< qreal*>(_a[0]) = std::move(_r); }  break;
        case 65: { qreal _r = _t->bufferSpeed();
            if (_a[0]) *reinterpret_cast< qreal*>(_a[0]) = std::move(_r); }  break;
        case 66: { qint64 _r = _t->buffered();
            if (_a[0]) *reinterpret_cast< qint64*>(_a[0]) = std::move(_r); }  break;
        case 67: _t->setBufferMode((*reinterpret_cast< BufferMode(*)>(_a[1]))); break;
        case 68: { BufferMode _r = _t->bufferMode();
            if (_a[0]) *reinterpret_cast< BufferMode*>(_a[0]) = std::move(_r); }  break;
        case 69: _t->setBufferValue((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 70: { int _r = _t->bufferValue();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 71: _t->setNotifyInterval((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 72: { int _r = _t->notifyInterval();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 73: _t->updateClock((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 74: _t->setBrightness((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 75: _t->setContrast((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 76: _t->setHue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 77: _t->setSaturation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 78: _t->loadInternal(); break;
        case 79: _t->playInternal(); break;
        case 80: _t->stopFromDemuxerThread(); break;
        case 81: _t->aboutToQuitApp(); break;
        case 82: _t->startNotifyTimer(); break;
        case 83: _t->stopNotifyTimer(); break;
        case 84: _t->onStarted(); break;
        case 85: _t->updateMediaStatus((*reinterpret_cast< QtAV::MediaStatus(*)>(_a[1]))); break;
        case 86: _t->onSeekFinished((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 87: _t->tryClearVideoRenderers(); break;
        case 88: _t->seekChapter((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::MediaStatus >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::MediaEndAction >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::AVError >(); break;
            }
            break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::AVPlayer::State >(); break;
            }
            break;
        case 85:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::MediaStatus >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AVPlayer::*_t)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::bufferProgressChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::relativeTimeModeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::autoLoadChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::asyncLoadChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::muteChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::sourceChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::loaded)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(QtAV::MediaStatus );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::mediaStatusChanged)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(QtAV::MediaEndAction );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::mediaEndActionChanged)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::durationChanged)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(const QtAV::AVError & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::error)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::paused)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::started)) {
                *result = 12;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::stopped)) {
                *result = 13;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::stoppedAt)) {
                *result = 14;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(QtAV::AVPlayer::State );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::stateChanged)) {
                *result = 15;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::speedChanged)) {
                *result = 16;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::repeatChanged)) {
                *result = 17;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::currentRepeatChanged)) {
                *result = 18;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::startPositionChanged)) {
                *result = 19;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::stopPositionChanged)) {
                *result = 20;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::seekableChanged)) {
                *result = 21;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::seekFinished)) {
                *result = 22;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::positionChanged)) {
                *result = 23;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::interruptTimeoutChanged)) {
                *result = 24;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::interruptOnTimeoutChanged)) {
                *result = 25;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::notifyIntervalChanged)) {
                *result = 26;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::brightnessChanged)) {
                *result = 27;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::contrastChanged)) {
                *result = 28;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::hueChanged)) {
                *result = 29;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::saturationChanged)) {
                *result = 30;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(unsigned int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::chaptersChanged)) {
                *result = 31;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::subtitleStreamChanged)) {
                *result = 32;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(const QVariantList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::internalAudioTracksChanged)) {
                *result = 33;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(const QVariantList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::internalVideoTracksChanged)) {
                *result = 34;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(const QVariantList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::externalAudioTracksChanged)) {
                *result = 35;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(const QVariantList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::internalSubtitleTracksChanged)) {
                *result = 36;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(const QByteArray & , const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::internalSubtitleHeaderRead)) {
                *result = 37;
                return;
            }
        }
        {
            typedef void (AVPlayer::*_t)(int , const QtAV::Packet & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AVPlayer::internalSubtitlePacketRead)) {
                *result = 38;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 19:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::MediaEndAction >(); break;
        case 18:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QtAV::MediaStatus >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        AVPlayer *_t = static_cast<AVPlayer *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->relativeTimeMode(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isAutoLoad(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->isAsyncLoad(); break;
        case 3: *reinterpret_cast< qreal*>(_v) = _t->bufferProgress(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->isSeekable(); break;
        case 5: *reinterpret_cast< qint64*>(_v) = _t->duration(); break;
        case 6: *reinterpret_cast< qint64*>(_v) = _t->position(); break;
        case 7: *reinterpret_cast< qint64*>(_v) = _t->startPosition(); break;
        case 8: *reinterpret_cast< qint64*>(_v) = _t->stopPosition(); break;
        case 9: *reinterpret_cast< qint64*>(_v) = _t->repeat(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->currentRepeat(); break;
        case 11: *reinterpret_cast< qint64*>(_v) = _t->interruptTimeout(); break;
        case 12: *reinterpret_cast< bool*>(_v) = _t->isInterruptOnTimeout(); break;
        case 13: *reinterpret_cast< int*>(_v) = _t->notifyInterval(); break;
        case 14: *reinterpret_cast< int*>(_v) = _t->brightness(); break;
        case 15: *reinterpret_cast< int*>(_v) = _t->contrast(); break;
        case 16: *reinterpret_cast< int*>(_v) = _t->saturation(); break;
        case 17: *reinterpret_cast< State*>(_v) = _t->state(); break;
        case 18: *reinterpret_cast< QtAV::MediaStatus*>(_v) = _t->mediaStatus(); break;
        case 19: *reinterpret_cast< QtAV::MediaEndAction*>(_v) = _t->mediaEndAction(); break;
        case 20: *reinterpret_cast< uint*>(_v) = _t->chapters(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        AVPlayer *_t = static_cast<AVPlayer *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setRelativeTimeMode(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setAutoLoad(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->setAsyncLoad(*reinterpret_cast< bool*>(_v)); break;
        case 6: _t->setPosition(*reinterpret_cast< qint64*>(_v)); break;
        case 7: _t->setStartPosition(*reinterpret_cast< qint64*>(_v)); break;
        case 8: _t->setStopPosition(*reinterpret_cast< qint64*>(_v)); break;
        case 9: _t->setRepeat(*reinterpret_cast< qint64*>(_v)); break;
        case 11: _t->setInterruptTimeout(*reinterpret_cast< qint64*>(_v)); break;
        case 12: _t->setInterruptOnTimeout(*reinterpret_cast< bool*>(_v)); break;
        case 13: _t->setNotifyInterval(*reinterpret_cast< int*>(_v)); break;
        case 14: _t->setBrightness(*reinterpret_cast< int*>(_v)); break;
        case 15: _t->setContrast(*reinterpret_cast< int*>(_v)); break;
        case 16: _t->setSaturation(*reinterpret_cast< int*>(_v)); break;
        case 17: _t->setState(*reinterpret_cast< State*>(_v)); break;
        case 19: _t->setMediaEndAction(*reinterpret_cast< QtAV::MediaEndAction*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject QtAV::AVPlayer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtAV__AVPlayer.data,
      qt_meta_data_QtAV__AVPlayer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QtAV::AVPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtAV::AVPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtAV__AVPlayer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QtAV::AVPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 89)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 89;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 89)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 89;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 21;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 21;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 21;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 21;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 21;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QtAV::AVPlayer::bufferProgressChanged(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtAV::AVPlayer::relativeTimeModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtAV::AVPlayer::autoLoadChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QtAV::AVPlayer::asyncLoadChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QtAV::AVPlayer::muteChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void QtAV::AVPlayer::sourceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void QtAV::AVPlayer::loaded()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void QtAV::AVPlayer::mediaStatusChanged(QtAV::MediaStatus _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void QtAV::AVPlayer::mediaEndActionChanged(QtAV::MediaEndAction _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void QtAV::AVPlayer::durationChanged(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void QtAV::AVPlayer::error(const QtAV::AVError & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void QtAV::AVPlayer::paused(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void QtAV::AVPlayer::started()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void QtAV::AVPlayer::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void QtAV::AVPlayer::stoppedAt(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void QtAV::AVPlayer::stateChanged(QtAV::AVPlayer::State _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void QtAV::AVPlayer::speedChanged(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void QtAV::AVPlayer::repeatChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void QtAV::AVPlayer::currentRepeatChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void QtAV::AVPlayer::startPositionChanged(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void QtAV::AVPlayer::stopPositionChanged(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void QtAV::AVPlayer::seekableChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void QtAV::AVPlayer::seekFinished(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void QtAV::AVPlayer::positionChanged(qint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void QtAV::AVPlayer::interruptTimeoutChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}

// SIGNAL 25
void QtAV::AVPlayer::interruptOnTimeoutChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 25, nullptr);
}

// SIGNAL 26
void QtAV::AVPlayer::notifyIntervalChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 26, nullptr);
}

// SIGNAL 27
void QtAV::AVPlayer::brightnessChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void QtAV::AVPlayer::contrastChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void QtAV::AVPlayer::hueChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}

// SIGNAL 30
void QtAV::AVPlayer::saturationChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void QtAV::AVPlayer::chaptersChanged(unsigned int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}

// SIGNAL 32
void QtAV::AVPlayer::subtitleStreamChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 32, _a);
}

// SIGNAL 33
void QtAV::AVPlayer::internalAudioTracksChanged(const QVariantList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 33, _a);
}

// SIGNAL 34
void QtAV::AVPlayer::internalVideoTracksChanged(const QVariantList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 34, _a);
}

// SIGNAL 35
void QtAV::AVPlayer::externalAudioTracksChanged(const QVariantList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 35, _a);
}

// SIGNAL 36
void QtAV::AVPlayer::internalSubtitleTracksChanged(const QVariantList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 36, _a);
}

// SIGNAL 37
void QtAV::AVPlayer::internalSubtitleHeaderRead(const QByteArray & _t1, const QByteArray & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 37, _a);
}

// SIGNAL 38
void QtAV::AVPlayer::internalSubtitlePacketRead(int _t1, const QtAV::Packet & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 38, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
