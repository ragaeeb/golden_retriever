#ifndef SERVICE_H_
#define SERVICE_H_

#include <QFileSystemWatcher>
#include <QTimer>

#include <bb/system/InvokeManager>

#include "customsqldatasource.h"
#include "MessageManager.h"

namespace bb {
	class Application;

	namespace pim {
		class Message;
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

    QList<qint64> m_pending;
	bool m_delRequest;
	bool m_delResponse;
    MessageManager m_manager;
	QFileSystemWatcher m_settingsWatcher;
	InvokeManager m_invokeManager;
	QTimer m_timer;
	CustomSqlDataSource m_sql;
	QVariantMap m_whitelist;

	void triggerNotification();

private slots:
	void commandProcessed(int command, QString const& data);
	void handleInvoke(const bb::system::InvokeRequest &);
	void init();
	void messageReceived(Message const& m, qint64 accountKey, QString const& conversationKey);
	void processPending();
	void settingChanged(QString const& key=QString());

Q_SIGNALS:
	void initialize();

public:
	Service(bb::Application* app);
};

}

#endif /* SERVICE_H_ */
