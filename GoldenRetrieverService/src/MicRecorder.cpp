#include "precompiled.h"

#include "MicRecorder.h"
#include "Command.h"
#include "IOUtils.h"
#include "Logger.h"

#define default_duration 10
#define max_duration 60
#define min_duration 1
#define extension_amr "amr"
#define extension_m4a "m4a"
#define quality_hi "hi"

namespace golden {

using namespace bb::pim::message;
using namespace bb::multimedia;
using namespace canadainc;

MicRecorder::MicRecorder(QStringList const& tokens, QObject* parent) :
        QObject(parent), m_duration(default_duration), m_extension(extension_amr)
{
    int n = tokens.size();

	if (n > 0)
	{
		m_duration = tokens.first().toUInt();

		if (n > 1)
		{
		    QString quality = tokens[1];

		    if ( quality.compare(quality_hi, Qt::CaseInsensitive) == 0 ) {
		        m_extension = extension_m4a;
		    }
		}
	}

	m_duration = qMin(m_duration, (unsigned int)max_duration); // max record of 1 minute
	m_duration = qMax(m_duration, (unsigned int)min_duration); // record for at least 1 second
	m_duration *= 1000;

	connect( &m_recorder, SIGNAL( durationChanged(unsigned int) ), this, SLOT( onDurationChanged(unsigned int) ) );
}


void MicRecorder::onDurationChanged(unsigned int duration)
{
	if (duration > m_duration)
	{
		disconnect( &m_recorder, SIGNAL( durationChanged(unsigned int) ), this, SLOT( onDurationChanged(unsigned int) ) );
		m_recorder.reset();

		QString fileName = QString("%1.%2").arg( QDateTime::currentMSecsSinceEpoch() ).arg(m_extension);

		QVariantList attachments;
		attachments << QVariant::fromValue( Attachment( QString("audio/%1").arg(m_extension), fileName, m_recorder.outputUrl() ) );

		if (m_extension == extension_amr) {
	        emit commandProcessed( Command::Microphone, tr("Recorded %n seconds of LQ audio.", "", duration/1000), attachments );
		} else {
		    emit commandProcessed( Command::Microphone, tr("Recorded %n seconds of HQ audio.", "", duration/1000), attachments );
		}
	}
}


void MicRecorder::record()
{
	QString fileName = QString("%1.%2").arg( QDateTime::currentMSecsSinceEpoch() ).arg(m_extension);
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
