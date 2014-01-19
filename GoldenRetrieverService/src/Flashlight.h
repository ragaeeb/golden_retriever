#ifndef FLASHLIGHT_H_
#define FLASHLIGHT_H_

#include <QObject>
#include <QVariantList>
#include <QTimer>

#include <camera/camera_api.h>

namespace golden {

class Flashlight : public QObject
{
	Q_OBJECT

	camera_handle_t m_camera;
	unsigned int m_duration;
	QTimer m_timer;

private slots:
	void timeout();

Q_SIGNALS:
	void commandProcessed(int command, QString const& replyBody, QVariantList const& attachments=QVariantList());

public:
	Flashlight(QStringList const& tokens, QObject* parent=NULL);
	virtual ~Flashlight();

	void start();
};

} /* namespace golden */
#endif /* FLASHLIGHT_H_ */
