#include "precompiled.h"

#include "service.hpp"
#include "GoldenUtils.h"
#include "Interpreter.h"
#include "IOUtils.h"
#include "Logger.h"
#include "QueryId.h"

namespace {
	const char* key_reply_prefix = "re";
}

namespace golden {

using namespace bb::platform;
using namespace bb::system;

Service::Service(bb::Application * app)	: QObject(app)
{
	QSettings s;

	if ( !QFile::exists( s.fileName() ) )
	{
		s.setValue( "init", QDateTime::currentMSecsSinceEpoch() );
		s.sync();
	}

	LOGGER("Constructed");

	connect( this, SIGNAL( initialize() ), this, SLOT( init() ), Qt::QueuedConnection ); // async startup

	emit initialize();
}


void Service::init()
{
    m_timer.setSingleShot(true);
	m_settingsWatcher.addPath( QSettings().fileName() );

	connect( &m_invokeManager, SIGNAL( invoked(const bb::system::InvokeRequest&) ), this, SLOT( handleInvoke(const bb::system::InvokeRequest&) ) );
	connect( &m_timer, SIGNAL( timeout() ), this, SLOT( processPending() ) );
	connect( &m_manager, SIGNAL( messageReceived(Message const&, qint64, QString const&) ), this, SLOT( messageReceived(Message const&, qint64, QString const&) ) );
	connect( &m_settingsWatcher, SIGNAL( fileChanged(QString const&) ), this, SLOT( settingChanged(QString const&) ) );

	QString database = GoldenUtils::databasePath();
	m_sql.setSource(database);

	if ( !QFile(database).exists() )
	{
		QStringList qsl;
		qsl << "CREATE TABLE logs (id INTEGER PRIMARY KEY AUTOINCREMENT, command INTEGER NOT NULL, reply TEXT NOT NULL, timestamp INTEGER NOT NULL)";
		m_sql.initSetup(qsl, QueryId::Setup);
	}

	settingChanged("account");
	settingChanged("whitelist");
}


void Service::settingChanged(QString const& path)
{
	LOGGER("Recalculate!" << path);

	QSettings q;

	QVariant account = q.value("account");

	if ( account.isValid() ) {
		m_manager.setAccountKey( account.toLongLong() );
		m_manager.setMonitoring(true);
	} else {
		m_manager.setMonitoring(false);
	}

	m_whitelist = q.value("whitelist").toMap();
}


void Service::handleInvoke(const bb::system::InvokeRequest & request)
{
	LOGGER("Invoekd" << request.action() );

	if (request.action().compare("com.example.GoldenRetrieverService.RESET") == 0)
	{
	}
}


void Service::processPending()
{
	LOGGER("PROCESSING");
	for (int i = 0; i < m_pending.size(); i++)
	{
		qint64 current = m_pending.first();
		Message m = m_manager.getMessage(current);

		if ( m.body(MessageBody::PlainText).isPartial() || m.body(MessageBody::Html).isPartial() ) {
			m_timer.start(1000);
		} else {
			m_pending.takeFirst();

			Interpreter* i = new Interpreter(&m_manager, m);
			connect( i, SIGNAL( commandProcessed(int, QString const&) ), this, SLOT( commandProcessed(int, QString const&) ) );
			i->run();
		}
	}

    LOGGER("All pending processed...");
}


void Service::commandProcessed(int command, QString const& data)
{
	Q_UNUSED(data);

	QVariantList params = QVariantList() << data;

    m_sql.setQuery( QString("INSERT INTO logs (command,reply,timestamp) VALUES ('%1',?,%2)").arg(command).arg( QDateTime::currentMSecsSinceEpoch() ) );
    m_sql.executePrepared(params, QueryId::LogCommand);
}


void Service::messageReceived(Message const& m, qint64 accountKey, QString const& conversationKey)
{
	Q_UNUSED(conversationKey);

	LOGGER("======== NEW MESSAGE" << accountKey << "whitelist" << m_whitelist << "message id" << m.subject() << m.id() );

	if ( ( m_whitelist.isEmpty() || m_whitelist.contains( m.sender().address() ) ) )
	{
		QString subject = m.subject();
		LOGGER("======== SUBJECT" << subject);

		QString goldenSubject = "golden";

		QString replyWithoutSpace = QString("%1:%2").arg(key_reply_prefix).arg(goldenSubject);
		QString replyWithSpace = QString("%1: %2").arg(key_reply_prefix).arg(goldenSubject);
		bool partialMatch = subject.contains(replyWithoutSpace, Qt::CaseInsensitive) || subject.contains(replyWithSpace, Qt::CaseInsensitive);
		bool perfectMatch = subject.compare(goldenSubject, Qt::CaseInsensitive) == 0;

		if (perfectMatch || partialMatch)
		{
			m.body(MessageBody::PlainText).data();
			m.body(MessageBody::Html).data();

			m_pending << m.id();
			m_timer.start(1000);
		}
	}
}

}
