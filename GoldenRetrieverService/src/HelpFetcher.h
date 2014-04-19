#ifndef HELPFETCHER_H_
#define HELPFETCHER_H_

#include <QObject>
#include <QRunnable>
#include <QStringList>

namespace golden {

class HelpFetcher : public QObject, public QRunnable
{
	Q_OBJECT

signals:
	void commandProcessed(int command, QString const& replyBody, QVariantList const& attachments);

public:
	HelpFetcher(QStringList const& tokens);
	void run();
};

} /* namespace golden */

#endif /* HELPFETCHER_H_ */
