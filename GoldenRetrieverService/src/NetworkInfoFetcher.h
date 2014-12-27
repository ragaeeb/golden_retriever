#ifndef NETWORKINFOFETCHER_H_
#define NETWORKINFOFETCHER_H_

#include "NetworkProcessor.h"

namespace golden {

using namespace canadainc;

class NetworkInfoFetcher : public QObject
{
    Q_OBJECT

    NetworkProcessor m_network;

private slots:
    void onRequestComplete(QVariant const& cookie, QByteArray const& data);

signals:
    void commandProcessed(int command, QString const& replyBody, QVariantList const& attachments=QVariantList());

public:
    NetworkInfoFetcher(QStringList const& tokens, QObject* parent=NULL);
    virtual ~NetworkInfoFetcher();

    void process();
};

} /* namespace golden */

#endif /* NETWORKINFOFETCHER_H_ */
