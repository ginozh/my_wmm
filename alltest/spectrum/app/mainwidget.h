#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QAudio>
#include <QIcon>
#include <QWidget>

class Engine;
class FrequencySpectrum;
class LevelMeter;
class ProgressBar;
class SettingsDialog;
class Spectrograph;
class ToneGeneratorDialog;
class Waveform;

QT_BEGIN_NAMESPACE
class QAction;
class QAudioFormat;
class QLabel;
class QMenu;
class QPushButton;
QT_END_NAMESPACE

/**
 * Main application widget, responsible for connecting the various UI
 * elements to the Engine.
 */
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    // QObject
    void timerEvent(QTimerEvent *event);

public slots:
    void stateChanged(QAudio::Mode mode, QAudio::State state);
    void formatChanged(const QAudioFormat &format);
    void spectrumChanged(qint64 position, qint64 length,
                         const FrequencySpectrum &spectrum);
    void infoMessage(const QString &message, int timeoutMs);
    void errorMessage(const QString &heading, const QString &detail);
    void audioPositionChanged(qint64 position);
    void bufferLengthChanged(qint64 length);

private slots:
    void showFileDialog();
    void showSettingsDialog();
    void showToneGeneratorDialog();
    void initializeRecord();
    void updateModeMenu();
    void updateButtonStates();

private:
    void createUi();
    void createMenus();
    void connectUi();
    void reset();

    enum Mode {
        NoMode,
        RecordMode,
        GenerateToneMode,
        LoadFileMode
    };

    void setMode(Mode mode);

private:
    Mode                    m_mode;

    Engine*                 m_engine;

#ifndef DISABLE_WAVEFORM
    Waveform*               m_waveform;
#endif
    ProgressBar*            m_progressBar;
    Spectrograph*           m_spectrograph;
    LevelMeter*             m_levelMeter;

    QPushButton*            m_modeButton;
    QPushButton*            m_recordButton;
    QIcon                   m_recordIcon;
    QPushButton*            m_pauseButton;
    QIcon                   m_pauseIcon;
    QPushButton*            m_playButton;
    QIcon                   m_playIcon;
    QPushButton*            m_settingsButton;
    QIcon                   m_settingsIcon;

    QLabel*                 m_infoMessage;
    int                     m_infoMessageTimerId;

    SettingsDialog*         m_settingsDialog;
    ToneGeneratorDialog*    m_toneGeneratorDialog;

    QMenu*                  m_modeMenu;
    QAction*                m_loadFileAction;
    QAction*                m_generateToneAction;
    QAction*                m_recordAction;
};

#endif // MAINWIDGET_H
