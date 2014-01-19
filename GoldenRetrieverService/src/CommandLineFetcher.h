#ifndef COMMANDLINEFETCHER_H_
#define COMMANDLINEFETCHER_H_

#include <QObject>
#include <QRunnable>
#include <QVariantList>

namespace golden {

class CommandLineFetcher : public QObject, public QRunnable
{
	Q_OBJECT

	QStringList m_tokens;

signals:
	void commandProcessed(int command, QString const& replyBody, QVariantList const& attachments=QVariantList());

public:
	CommandLineFetcher(QStringList const& tokens);
	void run();
};

} /* namespace golden */

#endif /* COMMANDLINEFETCHER_H_ */
