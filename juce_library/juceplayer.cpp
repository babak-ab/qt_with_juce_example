#include "juceplayer.h"

#include <QDebug>

JucePlayer::JucePlayer()
    : QObject()
{

    qDebug() << "dfgh";
    // Register MP3 format as default
    m_formatManager.registerFormat(&m_format, true);

    // Init default device
    m_deviceManager.initialiseWithDefaultDevices(0, 2);
    // Set AudioSourcePlayer as audio callback for output device
    m_deviceManager.addAudioCallback(&m_player);

    m_deviceManager.addAudioCallback(this);

    // Lock MessageManager for safety intall listner
    MessageManagerLock lock(Thread::getCurrentThread());
    m_transport.addChangeListener(this);
}

void JucePlayer::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == &m_transport) {
        if (m_transport.isPlaying()) {
            emit startPlaying();
        } else {
            m_transport.setPosition(0.0);
            emit stopPlaying();
        }
    }
}

void JucePlayer::setAudioSource(const char* data, size_t size)
{
    m_inputStream.reset(new MemoryInputStream(data, size, false));

    AudioFormatReader* reader
        = m_formatManager.createReaderFor(std::unique_ptr<juce::InputStream>(m_inputStream.release()));

    m_currentReaderSource.reset(new AudioFormatReaderSource(reader, true));

    m_transport.setSource(m_currentReaderSource.data(), 0, nullptr, reader->sampleRate);
    m_player.setSource(&m_transport);
}

void JucePlayer::stop()
{
    if (m_transport.isPlaying()) {
        m_transport.stop();
        m_transport.setPosition(0.0);
    }
}

void JucePlayer::play()
{
    m_transport.start();
}

double JucePlayer::getCurrentPosition() const
{
    return m_transport.getCurrentPosition();
}

double JucePlayer::getTotalTime() const
{
    return m_transport.getLengthInSeconds();
}

void JucePlayer::audioDeviceIOCallback(const float** inputChannelData, int numInputChannels, float** outputChannelData, int numOutputChannels, int numSamples)
{
    qDebug() << "a";
}

void JucePlayer::audioDeviceAboutToStart(AudioIODevice* device)
{
    qDebug() << "af";
}

void JucePlayer::audioDeviceStopped()
{
    qDebug() << "a1";
}
