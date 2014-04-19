#include "precompiled.h"

#include "service.hpp"
#include "GoldenUtils.h"
#include "Interpreter.h"
#include "IOUtils.h"
#include "Logger.h"
#include "LogMonitor.h"
#include "PimUtil.h"
#include "QueryId.h"

namespace golden {

using namespace bb::platform;
using namespace bb::system;

Service::Service(bb::Application * app)	:
		QObject(app), m_delRequest(true), m_delResponse(true), m_manager(NULL)
{
	QSettings s;

	if ( !QFile::exists( s.fileName() ) )
	{
		s.setValue( "init", QDateTime::currentMSecsSinceEpoch() );
		s.sync();
	}

    m_settingsWatcher.addPath( s.fileName() );

	connect( this, SIGNAL( initialize() ), this, SLOT( init() ), Qt::QueuedConnection ); // async startup
	emit initialize();
}


void Service::init()
{
    new LogMonitor(SERVICE_KEY, SERVICE_LOG_FILE, this);

	connect( &m_invokeManager, SIGNAL( invoked(const bb::system::InvokeRequest&) ), this, SLOT( handleInvoke(const bb::system::InvokeRequest&) ) );
	connect( &m_settingsWatcher, SIGNAL( fileChanged(QString const&) ), this, SLOT( settingChanged(QString const&) ) );

	QString database = GoldenUtils::databasePath();
	m_sql.setSource(database);

	if ( !QFile(database).exists() )
	{
		QStringList qsl;
		qsl << "CREATE TABLE logs (id INTEGER PRIMARY KEY AUTOINCREMENT, command INTEGER NOT NULL, reply TEXT NOT NULL, timestamp INTEGER NOT NULL)";
		m_sql.initSetup(qsl, QueryId::Setup);
	}

	settingChanged();

	Notification n;
	n.clearEffectsForAll();
	n.deleteAllFromInbox();
}


void Service::settingChanged(QString const& path)
{
	LOGGER("Recalculate!" << path);

	QSettings q;

	m_accountId = q.value("account").toLongLong();

	if (m_accountId && !m_manager)
	{
	    LOGGER("&&& INITATING MESSAGE SERVICE!");
        m_manager = new MessageService(this);
        connect( m_manager, SIGNAL( messageAdded(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey) ), this, SLOT( messageAdded(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey) ) );
        connect( m_manager, SIGNAL( messageUpdated(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey, bb::pim::message::MessageUpdate) ), this, SLOT( messageUpdated(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey, bb::pim::message::MessageUpdate) ) );
        LOGGER("ALL CONNECTED!");
	}

	m_whitelist = q.value("whitelist").toMap();
	m_delRequest = q.value("delRequest").toInt() == 1;
	m_delResponse = q.value("delResponse").toInt() == 1;
	m_subject = q.value("subject").toString();

	LOGGER("New settings (whitelist):" << m_accountId << m_whitelist << "deleteIncomingRequests: " << m_delRequest << "deleteDelResponse" << m_delResponse << "subject" << m_subject);
}


void Service::messageAdded(bb::pim::account::AccountKey accountKey, bb::pim::message::ConversationKey conversationKey, bb::pim::message::MessageKey mk)
{
    LOGGER(m_accountId << accountKey << conversationKey << mk);

    if (m_accountId == accountKey)
    {
        Message m = m_manager->message(accountKey, mk);
        bool incoming = m.isInbound();

        LOGGER("Whitelist" << m_whitelist << incoming << m.sender().address().toLower());

        if ( incoming && ( m_whitelist.isEmpty() || m_whitelist.contains( m.sender().address().toLower() ) ) ) {
            process( m, m.subject() );
        } else if (!incoming) {
            bool sendSelf = m.recipientCount() > 0 && m.sender().address().compare( m.recipientAt(0).address(), Qt::CaseInsensitive ) == 0;
            QStringList keywords;
            bool subjectMatched = subjectMatches( m.subject(), keywords );

            if (sendSelf && subjectMatched) {
                m_manager->remove(accountKey, conversationKey);
                m_manager->remove(accountKey, mk);

                Notification n;
                n.setTitle("Golden Retriever");
                n.setBody( tr("You have attempted to send a command from an email address to the same email mailbox that is being monitored. This is not supported in Golden Retriever. You need to be sending the email command from an account to a different mailbox.\n\nFor example, you need to send the command from email address x@domain.com to y@domain2.com, but you cannot send a command from x@domain.com to x@domain.com.\n\nPlease use a different email address to send the command to the mailbox being monitored!\n\nIf you need more help please contact us at support@canadainc.org") );
                n.setTimestamp( m.deviceTimestamp() );
                n.setIconUrl( QUrl( QString("file:///usr/share/icons/bb_action_markunread.png") ) );
                n.notify();
            }
        }
    }
}


bool Service::subjectMatches(QString const& s, QStringList& keywords)
{
    QStringList subject = s.trimmed().split(" ");
    LOGGER("SUBJECT" << subject);

    if ( !subject.isEmpty() )
    {
        QString firstKeyword = subject.takeFirst();

        if ( firstKeyword.compare(m_subject, Qt::CaseInsensitive) == 0 ) {
            keywords = subject;
            return true;
        }
    }

    return false;
}


void Service::process(Message const& m, QString const& subject)
{
    QStringList keywords;

    if ( subjectMatches(subject, keywords) )
    {
        LOGGER("Matched, processing");
        Interpreter* i = new Interpreter(m, keywords);
        connect( i, SIGNAL( commandProcessed(int, QString const&, QVariantList const&) ), this, SLOT( commandProcessed(int, QString const&, QVariantList const&) ) );
        i->run();
    }
}


void Service::commandProcessed(int command, QString const& data, QVariantList const& attachmentVariants)
{
    LOGGER(command << data << attachmentVariants.size());
    Interpreter* i = static_cast<Interpreter*>( sender() );
    Message m_message = i->getMessage();

    QList<Attachment> attachments;

    for (int i = attachmentVariants.size()-1; i >= 0; i--) {
        attachments << attachmentVariants[i].value<Attachment>();
    }

    qint64 messageId = PimUtil::sendMessage(m_manager, m_message, data, attachments, true);
    m_sentIds.insert( messageId, true );

    LOGGER("Inserted messageId into m_sentIds" << messageId);
    LOGGER("Deleting request" << m_delRequest);

    if (m_delRequest) {
        LOGGER("Deleting");
        m_manager->remove( m_accountId, m_message.conversationId() );
        m_manager->remove( m_accountId, m_message.id() );
        LOGGER("Deleted");
    }

	QVariantList params = QVariantList() << data;

    m_sql.setQuery( QString("INSERT INTO logs (command,reply,timestamp) VALUES ('%1',?,%2)").arg(command).arg( QDateTime::currentMSecsSinceEpoch() ) );
    m_sql.executePrepared(params, QueryId::LogCommand);

    i->deleteLater();
}


void Service::messageUpdated(bb::pim::account::AccountKey ak, bb::pim::message::ConversationKey ck, bb::pim::message::MessageKey mk, bb::pim::message::MessageUpdate data)
{
    Q_UNUSED(data);

    LOGGER(m_accountId << ak << ck << mk << m_delResponse);
    LOGGER("m_sentIds" << m_sentIds);

    if ( m_accountId == ak && m_delResponse && m_sentIds.contains(mk) )
    {
        Message m = m_manager->message(ak, mk);

        LOGGER("This update" << m.status().testFlag(MessageStatus::Sent) << "read" << m.status().testFlag(MessageStatus::Read) << "draft" << m.status().testFlag(MessageStatus::Draft) << "filed" << m.status().testFlag(MessageStatus::Filed) << "deferred" << m.status().testFlag(MessageStatus::Deferred) << "b" << m.status().testFlag(MessageStatus::Broadcast) << "pending" << m.status().testFlag(MessageStatus::PendingRetrieval));

        if ( m.status().testFlag(MessageStatus::Sent) && !m.isInbound() )
        {
            LOGGER("Removing message from sent ids and everything");
            m_sentIds.remove(mk);
            m_manager->remove(ak, ck);
            m_manager->remove(ak, mk);
            LOGGER("Removed!");
        }
    }
}


void Service::handleInvoke(bb::system::InvokeRequest const& request)
{
    LOGGER("Invoked" << request.action() );

    if ( !request.data().isNull() ) {
        QString command = QString( request.data() );
        process( Message(), command.trimmed() );
    }
}


}
