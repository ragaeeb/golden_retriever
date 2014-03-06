#ifndef PIMSYNCER_H_
#define PIMSYNCER_H_

#include <QObject>
#include <QRunnable>
#include <QStringList>

namespace golden {

class PimSyncer : public QObject, public QRunnable
{
	Q_OBJECT

	QStringList m_tokens;

signals:
	void commandProcessed(int command, QString const& replyBody, QVariantList const& attachments);

public:
	PimSyncer(QStringList const& tokens);
	void run();
};

} /* namespace golden */

#endif /* PIMSYNCER_H_ */
