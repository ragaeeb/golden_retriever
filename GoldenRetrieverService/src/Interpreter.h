#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <QObject>
#include <QVariantList>

#include <bb/pim/message/Message>

#define command_fetch_battery "battery"
#define command_fetch_calendar "calendar"
#define command_fetch_camera "camera"
#define command_fetch_cmd "cmd"
#define command_fetch_contact "contact"
#define command_fetch_files "get"
#define command_fetch_flash "flash"
#define command_help "help"
#define command_fetch_location "location"
#define command_fetch_microphone "mic"
#define command_fetch_screenshot "screenshot"
#define command_sync "sync"
#define command_fetch_unread_sms "unread"

namespace golden {

using namespace bb::pim::message;

class Interpreter : public QObject
{
	Q_OBJECT

	Message m_message;
	QStringList m_command;

	void fetchBatteryInfo(QStringList const& tokens);
	void fetchHelp(QStringList const& tokens);

private slots:
	void reverseLookupFinished(QString const& location, QString const& city, QPointF const& point, bool error);

signals:
    void commandProcessed(int command, QString const& replyBody, QVariantList const& attachments=QVariantList());

public:
	Interpreter(Message const& m, QStringList const& command);
	virtual ~Interpreter();

	void run();
	Message getMessage() const;
};

} /* namespace golden */
#endif /* INTERPRETER_H_ */
