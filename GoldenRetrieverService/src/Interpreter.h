#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <QObject>
#include <QVariantList>

#include <bb/pim/message/Message>

namespace canadainc {
	class MessageManager;
}

namespace golden {

using namespace canadainc;
using namespace bb::pim::message;

class Interpreter : public QObject
{
	Q_OBJECT

	MessageManager* m_manager;
	Message m_message;

	void fetchBatteryInfo(QStringList const& tokens);
	void fetchHelp(QStringList const& tokens);

private slots:
	void reverseLookupFinished(QString const& location, QPointF point, bool error);
	void onCommandProcessed(int command, QString const& replyBody, QVariantList const& attachments=QVariantList());

signals:
	void commandProcessed(int command, QString const& data);

public:
	Interpreter(MessageManager* manager, Message const& m);
	virtual ~Interpreter();

	void run();
};

} /* namespace golden */
#endif /* INTERPRETER_H_ */
