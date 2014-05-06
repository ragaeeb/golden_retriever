#include "precompiled.h"

#include "Flashlight.h"
#include "Command.h"
#include "IOUtils.h"
#include "Logger.h"

namespace {

const unsigned int default_duration = 2;
const unsigned int max_duration = 10;
const unsigned int min_duration = 1;

} // namespace

namespace golden {

using namespace canadainc;
using namespace bb::device;

Flashlight::Flashlight(QStringList const& tokens, QObject* parent) :
		QObject(parent), m_camera(0), m_led(LedColor::Blue)
{
	m_duration = min_duration;

	if ( !tokens.isEmpty() ) {
		m_duration = tokens.first().toUInt();
	}

	m_duration = qMin(m_duration, max_duration); // max flash of 10 minute
	m_duration = qMax(m_duration, min_duration); // flash for at least 1 minute
	m_duration *= 60000;

	connect( &m_timer, SIGNAL( timeout() ), this, SLOT( timeout() ) );
}


void Flashlight::start()
{
    const camera_error_t error = camera_open(CAMERA_UNIT_REAR, CAMERA_MODE_PREAD | CAMERA_MODE_PWRITE, &m_camera);

    if (CAMERA_EOK != error) {
    	emit commandProcessed( Command::Flash, tr("Could not access camera hardware to an unknown error.") );
    } else {
    	const camera_videolightmode_t mode = (CAMERA_VIDEOLIGHT_ON);
    	const camera_error_t error = camera_config_videolight(m_camera, mode);

    	if (CAMERA_EOK != error) {
    		emit commandProcessed( Command::Flash, tr("Could not turn on camera flash due to an unknown error.") );
    	} else {
    		m_led.flash(-1);

    		emit commandProcessed( Command::Flash, tr("Successfully turned on flash light.") );
        	m_timer.start(m_duration);
    	}
    }
}


void Flashlight::timeout()
{
	const camera_videolightmode_t mode = (CAMERA_VIDEOLIGHT_OFF);
	camera_config_videolight(m_camera, mode);
	camera_close(m_camera);

	m_led.cancel();

	deleteLater();
}


Flashlight::~Flashlight()
{
}

} /* namespace golden */
