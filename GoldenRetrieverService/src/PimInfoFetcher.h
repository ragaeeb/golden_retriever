#ifndef PIMINFOFETCHER_H_
#define PIMINFOFETCHER_H_

#include <QObject>
#include <QRunnable>
#include <QStringList>

namespace golden {

class PimInfoFetcher : public QObject, public QRunnable
{
	Q_OBJECT

	QStringList m_tokens;
	int m_command;

	void processContactSearch();
	void processCalendarSearch();
	void fetchUnreadMessages();
	void fetchAllCalls();

signals:
	void commandProcessed(int command, QString const& replyBody, QVariantList const& attachments);

public:
	PimInfoFetcher(QStringList const& tokens, int command);
	void run();
};

} /* namespace golden */
#endif /* PIMINFOFETCHER_H_ */
