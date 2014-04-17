#include "precompiled.h"

#include "MicRecorder.h"
#include "Command.h"
#include "IOUtils.h"
#include "Logger.h"

#define default_duration 10
#define max_duration 60
#define min_duration 1

namespace golden {

using namespace bb::pim::message;
using namespace bb::multimedia;
using namespace canadainc;

MicRecorder::MicRecorder(QStringList const& tokens, QObject* parent) : QObject(parent), m_duration(default_duration)
{
	if ( !tokens.isEmpty() ) {
		m_duration = tokens.first().toUInt();
	}

	m_duration = qMin(m_duration, (unsigned int)max_duration); // max record of 1 minute
	m_duration = qMax(m_duration, (unsigned int)min_duration); // record for at least 1 second
	m_duration *= 1000;

	connect( &m_recorder, SIGNAL( durationChanged(unsigned int) ), this, SLOT( onDurationChanged(unsigned int) ) );
}


void MicRecorder::onDurationChanged(unsigned int duration)
{
	if (duration > m_duration) {
		disconnect( &m_recorder, SIGNAL( durationChanged(unsigned int) ), this, SLOT( onDurationChanged(unsigned int) ) );
		m_recorder.reset();

		QString fileName = QString("%1.m4a").arg( QDateTime::currentMSecsSinceEpoch() );

		QVariantList attachments;
		attachments << QVariant::fromValue( Attachment( "audio/mp4", fileName, m_recorder.outputUrl() ) );
		emit commandProcessed( Command::Microphone, tr("Recorded %n seconds of audio.", "", duration/1000), attachments );
	}
}


void MicRecorder::record()
{
	QString fileName = QString("%1.m4a").arg( QDateTime::currentMSecsSinceEpoch() );
	QString path = QString("%1/%2").arg(IOUtils::directory_temp).arg(fileName);
	QString uri = QString("file:///%1").arg(path);

	m_recorder.setOutputUrl(uri);
	MediaError::Type error = m_recorder.record();

	if (error != MediaError::None) {
		emit commandProcessed( Command::Microphone, tr("Could not record audio due to an unknown error.") );
	}
}


MicRecorder::~MicRecorder()
{
}

} /* namespace golden */
