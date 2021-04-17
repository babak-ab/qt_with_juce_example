#ifndef JUCEPLAYER_H
#define JUCEPLAYER_H

#include "JuceLibraryCode/JuceHeader.h"

#include <QDebug>
#include <QObject>
#include <QScopedPointer>

/*!
 *    \class JucePlayer
 *    \brief very simple example how to play media data from JUCE in Qt project
 */
class Q_DECL_EXPORT JucePlayer : public QObject,
                                 public ChangeListener,
                                 public AudioIODeviceCallback

{
    Q_OBJECT

public:
    JucePlayer();

    //! \note Callback for AudioTransportSource
    void changeListenerCallback(ChangeBroadcaster* source) override;

    //! \note Set raw audio file data or another audio data in MP3 format
    void setAudioSource(const char* data, size_t size);

    void stop();
    void play();

    double getCurrentPosition() const;
    double getTotalTime() const;

signals:
    void startPlaying();
    void stopPlaying();

private:
    AudioSourcePlayer m_player;
    AudioTransportSource m_transport;
    AudioFormatManager m_formatManager;
    AudioDeviceManager m_deviceManager;
    MP3AudioFormat m_format;

    QScopedPointer<AudioFormatReaderSource> m_currentReaderSource;
    std::unique_ptr<MemoryInputStream> m_inputStream;

    // AudioIODeviceCallback interface
public:
    void audioDeviceIOCallback(const float **inputChannelData, int numInputChannels, float **outputChannelData, int numOutputChannels, int numSamples);
    void audioDeviceAboutToStart(AudioIODevice *device);
    void audioDeviceStopped();
};

#endif // JUCEPLAYER_H
