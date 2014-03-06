#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <QObject>
#include <QVariantList>

#include <bb/pim/message/Message>

namespace golden {

using namespace bb::pim::message;

class Interpreter : public QObject
{
	Q_OBJECT

	Message m_message;

	void fetchBatteryInfo(QStringList const& tokens);
	void fetchHelp(QStringList const& tokens);

private slots:
	void reverseLookupFinished(QString const& location, QString const& city, QPointF const& point, bool error);

signals:
    void commandProcessed(int command, QString const& replyBody, QVariantList const& attachments=QVariantList());

public:
	Interpreter(Message const& m);
	virtual ~Interpreter();

	void run();
	Message getMessage() const;
};

} /* namespace golden */
#endif /* INTERPRETER_H_ */
