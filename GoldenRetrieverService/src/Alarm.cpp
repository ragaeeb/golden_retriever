#include "precompiled.h"

#include "Alarm.h"
#include "Command.h"
#include "Logger.h"

#define max_duration 60*5 // 5 minutes of alarm

namespace golden {

Alarm::Alarm(QStringList const& tokens, QObject* parent) :
        QObject(parent), m_duration(max_duration), m_currentVol(100)
{
    if ( !tokens.isEmpty() ) {
        m_duration = tokens.first().toUInt();
    }

    if (m_duration > max_duration) {
        m_duration = max_duration;
    }

    m_duration *= 1000;

    LOGGER("duration" << m_duration);
}


void Alarm::timeout()
{
    LOGGER("timeout");
    m_vibrator.stop();
    m_player.stop();
    audiomixer_set_output_level(AUDIOMIXER_OUTPUT_SPEAKER, m_currentVol);
    emit commandProcessed( Command::Alarm, tr("Played alarm for %n seconds.", "", m_duration/1000) );
}


void Alarm::play()
{
    audiomixer_get_output_level(AUDIOMIXER_OUTPUT_SPEAKER, &m_currentVol);
    audiomixer_set_output_level(AUDIOMIXER_OUTPUT_SPEAKER, 100);
    LOGGER("Play" << m_currentVol);

    m_player.setSourceUrl( QUrl("asset:///audio/alarm.mp3") );
    m_player.setRepeatMode(RepeatMode::Track);
    m_player.setVolume(1);
    MediaError::Type error = m_player.play();

    if (error != MediaError::None) {
        emit commandProcessed( Command::Alarm, tr("Could not play alarm due to an unknown error.") );
        LOGGER("Alarm Error!" << error);
        return;
    }

    if ( m_vibrator.isSupported() ) {
        m_vibrator.start(100, m_duration);
    }

    QTimer::singleShot( m_duration, this, SLOT( timeout() ) );
    LOGGER("Successfully started");
}


Alarm::~Alarm()
{
}

} /* namespace golden */
