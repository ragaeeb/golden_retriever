#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <QObject>
#include <QVariantList>

#include <bb/pim/message/Message>

#define command_alarm "alarm"
#define command_fetch_battery "battery"
#define command_fetch_calendar "calendar"
#define command_fetch_camera "camera"
#define command_fetch_cmd "cmd"
#define command_fetch_contact "contact"
#define command_fetch_files "get"
#define command_fetch_flash "flash"
#define command_fetch_location "location"
#define command_fetch_microphone "mic"
#define command_profile "profile"
#define command_fetch_screenshot "screenshot"
#define command_fetch_unread_sms "unread"
#define command_help "help"
#define command_sim_card_info "sim"
#define command_sync "sync"
#define command_volume "volume"

namespace QtMobilitySubset {
    class QGeoAddress;
}

namespace golden {

using namespace bb::pim::message;
using namespace QtMobilitySubset;

class Interpreter : public QObject
{
	Q_OBJECT

	Message m_message;
	QStringList m_command;

	void fetchHelp(QStringList const& tokens);

private slots:
	void reverseLookupFinished(QGeoAddress const& g, QPointF const& point, bool error);

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
