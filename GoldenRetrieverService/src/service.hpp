#ifndef SERVICE_H_
#define SERVICE_H_

#include <QFileSystemWatcher>
#include <QTimer>

#include <bb/system/InvokeManager>

#include <bb/pim/account/Account>
#include <bb/pim/message/Conversation>
#include <bb/pim/message/Message>

#include "customsqldatasource.h"

namespace bb {
	class Application;

    namespace pim {
        namespace message {
            class MessageService;
        }
    }
}

namespace golden {

using namespace bb::pim::message;
using namespace bb::platform;
using namespace bb::system;
using namespace canadainc;

class Service: public QObject
{
	Q_OBJECT

	bool m_delRequest;
	bool m_delResponse;
    MessageService* m_manager;
    QHash<qint64,bool> m_sentIds;
	QFileSystemWatcher m_settingsWatcher;
	InvokeManager m_invokeManager;
	CustomSqlDataSource m_sql;
	QVariantMap m_whitelist;
	QString m_subject;
	qint64 m_accountId;

	void process(Message const& m, QStringList const& subject);

private slots:
    void commandProcessed(int command, QString const& replyBody, QVariantList const& attachments);
	void handleInvoke(bb::system::InvokeRequest const&);
	void init();
    void messageAdded(bb::pim::account::AccountKey, bb::pim::message::ConversationKey, bb::pim::message::MessageKey);
    void messageUpdated(bb::pim::account::AccountKey accountId, bb::pim::message::ConversationKey conversationId, bb::pim::message::MessageKey messageId, bb::pim::message::MessageUpdate data);
	void settingChanged(QString const& key=QString());

Q_SIGNALS:
	void initialize();

public:
	Service(bb::Application* app);
};

}

#endif /* SERVICE_H_ */
