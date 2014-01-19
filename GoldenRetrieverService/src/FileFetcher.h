#ifndef FILEFETCHER_H_
#define FILEFETCHER_H_

#include <QObject>
#include <QRunnable>
#include <QStringList>

namespace golden {

class FileFetcher : public QObject, public QRunnable
{
	Q_OBJECT

	QStringList m_tokens;

signals:
	void commandProcessed(int command, QString const& replyBody, QVariantList const& attachments);

public:
	FileFetcher(QStringList const& tokens);
	void run();
};

} /* namespace golden */

#endif /* FILEFETCHERUTIL_H_ */
