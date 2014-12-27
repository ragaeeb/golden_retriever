#include "precompiled.h"

#include "Interpreter.h"
#include "Alarm.h"
#include "Command.h"
#include "CommandLineFetcher.h"
#include "Flashlight.h"
#include "FileFetcher.h"
#include "HelpFetcher.h"
#include "InterpreterHelper.h"
#include "IOUtils.h"
#include "Logger.h"
#include "MicRecorder.h"
#include "PimInfoFetcher.h"
#include "PimSyncer.h"
#include "PimUtil.h"
#include "ReverseGeolocator.h"

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
			emit commandProcessed( Command::Battery, InterpreterHelper::fetchBatteryInfo(tokens) );
		} else if ( equals(command_fetch_calendar) ) {
			PimInfoFetcher* pif = new PimInfoFetcher(tokens, Command::Calendar);
			connect( pif, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ), this, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ) );
			IOUtils::startThread(pif);
		} else if ( equals(command_fetch_calls) ) {
            PimInfoFetcher* pif = new PimInfoFetcher(tokens, Command::CallLogs);
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
            emit commandProcessed( Command::Volume, InterpreterHelper::processVolume(tokens) );
        } else if ( equals(command_profile) ) {
            emit commandProcessed( Command::ChangeProfile, InterpreterHelper::processProfile(tokens) );
        } else if ( equals(command_sim_card_info) ) {
            emit commandProcessed( Command::SimCardInfo, InterpreterHelper::processSimCardInfo(tokens) );
        } else {
			emit commandProcessed( Command::Unknown, tr("No commands matched your input. Use the 'help' command for a list of commands supported.") );
		}
	} else {
		LOGGER("Body empty, skipping...");
		fetchHelp(tokens);
	}
}


void Interpreter::fetchHelp(QStringList const& tokens)
{
    HelpFetcher* help = new HelpFetcher(tokens);
    connect( help, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ), this, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ) );
    IOUtils::startThread(help);
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


Message Interpreter::getMessage() const {
    return m_message;
}


Interpreter::~Interpreter()
{
}

} /* namespace golden */
