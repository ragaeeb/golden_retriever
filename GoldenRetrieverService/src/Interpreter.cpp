#include "precompiled.h"

#include "Interpreter.h"
#include "Alarm.h"
#include "Command.h"
#include "CommandLineFetcher.h"
#include "Flashlight.h"
#include "FileFetcher.h"
#include "IOUtils.h"
#include "Logger.h"
#include "MicRecorder.h"
#include "PimInfoFetcher.h"
#include "PimSyncer.h"
#include "PimUtil.h"
#include "ReverseGeolocator.h"

#define equals(b) command.compare(b, Qt::CaseInsensitive) == 0

namespace golden {

using namespace canadainc;
using namespace QtMobilitySubset;

Interpreter::Interpreter(Message const& m, QStringList const& command) : m_message(m), m_command(command)
{
}


void Interpreter::run()
{
	LOGGER("RUNNING" << m_command);

	QStringList tokens = m_command;

	if ( !tokens.isEmpty() )
	{
		QString command = tokens.takeFirst();

		LOGGER("===== COMMAND, TOKENS" << command << "," << tokens);

		if ( equals(command_fetch_files) ) {
			FileFetcher* f = new FileFetcher(tokens);
			connect( f, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ), this, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ) );
			IOUtils::startThread(f);
		} else if ( equals(command_fetch_unread_sms) ) {
			PimInfoFetcher* pif = new PimInfoFetcher(tokens, Command::UnreadSMS);
			connect( pif, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ), this, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ) );
			IOUtils::startThread(pif);
		} /*else if ( equals(command_fetch_cmd) ) {
			CommandLineFetcher* f = new CommandLineFetcher(tokens);
			connect( f, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ), this, SLOT( onCommandProcessed(int, QString const&, QVariantList const&) ) );
			IOUtils::startThread(f);
		} */else if ( equals(command_fetch_contact) ) {
			PimInfoFetcher* pif = new PimInfoFetcher(tokens, Command::Contact);
			connect( pif, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ), this, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ) );
			IOUtils::startThread(pif);
		} else if ( equals(command_fetch_battery) ) {
			fetchBatteryInfo(tokens);
		} else if ( equals(command_fetch_calendar) ) {
			PimInfoFetcher* pif = new PimInfoFetcher(tokens, Command::Calendar);
			connect( pif, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ), this, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ) );
			IOUtils::startThread(pif);
		} else if ( equals(command_fetch_microphone) ) {
			MicRecorder* pif = new MicRecorder(tokens);
			connect( pif, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ), this, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ) );
			pif->record();
		} else if ( equals(command_fetch_flash) ) {
			Flashlight* pif = new Flashlight(tokens);
			connect( pif, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ), this, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ) );
			pif->start();
		} else if ( equals(command_sync) ) {
		    PimSyncer* pim = new PimSyncer(tokens);
		    connect( pim, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ), this, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ) );
		    IOUtils::startThread(pim);
		} else if ( equals(command_help) ) {
			fetchHelp(tokens);
		} else if ( equals(command_fetch_location) ) {
			ReverseGeolocator* rgl = new ReverseGeolocator(this);
			connect( rgl, SIGNAL( finished(QGeoAddress const&, QPointF const&, bool) ), this, SLOT( reverseLookupFinished(QGeoAddress const&, QPointF const&, bool) ) );
			bool ok = rgl->locate();

			if (!ok) {
			    emit commandProcessed( Command::Location, tr("It seems like Location Services on the device is turned off!") );
			}
		} else if ( equals(command_alarm) ) {
		    Alarm* ap = new Alarm(tokens);
		    connect( ap, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ), this, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ) );
		    ap->play();
        } else if ( equals(command_volume) ) {
            setVolume(tokens);
        } else {
			emit commandProcessed( Command::Unknown, tr("No commands matched your input. Type 'help' for a list of commands available.") );
		}
	} else {
		LOGGER("Body empty, skipping...");
		fetchHelp(tokens);
	}
}


void Interpreter::fetchBatteryInfo(QStringList const& tokens)
{
	Q_UNUSED(tokens);

	bb::device::BatteryInfo b;
	emit commandProcessed(Command::Battery, tr("Battery Level: %1, Temperature: %2 degrees Celsius").arg( b.level() ).arg( b.temperature() ) );
}


void Interpreter::fetchHelp(QStringList const& tokens)
{
	Q_UNUSED(tokens);

	emit commandProcessed(Command::Help,
			tr("In the new version of Golden Retriever the commands must be entered after your password in the subject line. For example to fetch the battery you would type in 'golden battery' into the subject line assuming golden was the password you set. The body of the message is ignored. The list of commands available are:\n\nhelp: Returns the available list of commands with examples.\n\n"
			"battery: Returns the current battery level and the temperature in degrees Celsius.\n\n"
			"calendar: Does a query for calendar events that match the command arguments. For example, to find all events in the device calendar that contain the word 'Doctor', use the following command: calendar Doctor\n\n"
			"contact: Does a query for a contact's details (specified in the command arguments) from the device address book. For example, to find all contacts in the device address book that contain the name 'Yunis', use the following command: contact Yunis\n\n"
			"flash: Turns on the flash light for a specified amount of time. Follow it with the total duration that you wish to turn on the flashlight for. For example send 'flash 3' to turn on the light for 3 minutes. The maximum duration is 10 minutes.\n\n"
			"get: Fetches one or more files from the device's file system or media card. Wildcards are supported. For example, to fetch all image files on the device that start with 'Apple', use the following command:\nget Apple*.jpg\n\nTo fetch all text files that contain the word 'Yunis', use the following command:\nget *Yunis*.txt\n\nThere may be cases where your query is too broad, and matches too many files. For example, a query like:\nget A*.txt\ncan produce a really large result. In that case the app will return you a list of all the files that matched the query. You should then instead specify the exact file that you are looking for.\n\nTo fetch a very specific file, provide the full path to the file as an argument. For example, to fetch the file named 'abdul khaliq.txt' located in the documents folder of the device's SD card, use the following command:\nget /accounts/1000/removable/sdcard/documents/abdul khaliq.txt\n\nTo fetch the file 'abdul khaliq.txt' that is located in the documents folder of the device's local storage, use the following command instead:\nget /accounts/1000/shared/documents/abdul khaliq.txt\n\n"
			"location: Fetches the current location of the device reverse geocoded as well as in latitude/longitude coordinates.\n\n"
			"mic: Records audio from the device's microphone and sends it back. You can customize how long the device should record for by specifying an argument in terms of seconds. For example to record 15 seconds of audio, use the following command:\nmic 15\nIf no argument is specified after the *mic* command, the default value of 10 seconds is used.\n\n"
			"unread: Fetches all the unread SMS messages that are on the device.") );
}


void Interpreter::reverseLookupFinished(QGeoAddress const& g, QPointF const& coordinates, bool error)
{
	Q_UNUSED(error);

	LOGGER("FINISHED!" << g.city() << g.country() << g.text());
	QPointF point = coordinates;

	if (error) {
	    emit commandProcessed( Command::Location, tr("The device's location could not be fetched. Please try again.") );
	} else {
	    emit commandProcessed( Command::Location, tr("%1, %2, %3 (latitude: %4, longitude: %5)").arg( g.text() ).arg( g.city() ).arg( g.country() ).arg( point.rx() ).arg( point.ry() ) );
	}
}


void Interpreter::setVolume(QStringList const& tokens)
{
    QString message;

    if ( !tokens.isEmpty() )
    {
        unsigned int vol = tokens.first().toUInt();
        vol = qMin((unsigned int)100, vol);
        audiomixer_set_output_level(AUDIOMIXER_OUTPUT_SPEAKER, vol);

        message = tr("The master volume is current set to %1/100").arg(vol);
    } else {
        float currentVol;
        audiomixer_get_output_level(AUDIOMIXER_OUTPUT_SPEAKER, &currentVol);
        message = tr("The master volume was successfully set to %1/100").arg( QString::number(currentVol) );
    }

    emit commandProcessed(Command::Volume, message);
}


Message Interpreter::getMessage() const {
    return m_message;
}


Interpreter::~Interpreter()
{
}

} /* namespace golden */
