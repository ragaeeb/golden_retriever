#include "precompiled.h"

#include "NetworkInfoFetcher.h"
#include "Command.h"

namespace {

void fetchInfoForInterface(QString const& name, QString& ipv6, QStringList& result)
{
    QNetworkInterface const& i = QNetworkInterface::interfaceFromName(name);

    if ( i.isValid() )
    {
        QString ipv4;

        foreach ( QNetworkAddressEntry const& entry, i.addressEntries() )
        {
            QAbstractSocket::NetworkLayerProtocol p = entry.ip().protocol();

            if ( p == QAbstractSocket::IPv4Protocol || ( p == QAbstractSocket::IPv6Protocol && ipv4.isNull() && entry.ip().toString() != ipv6 ) ) {
                ipv4 = entry.ip().toString();
            } else if ( p == QAbstractSocket::IPv6Protocol && ipv6.isNull() ) {
                ipv6 = entry.ip().toString();
            }
        }

        if ( !ipv4.isNull() ) {
            result << QString("%1=%2").arg(name).arg(ipv4);
        }
    }
}

}

namespace golden {

NetworkInfoFetcher::NetworkInfoFetcher(QStringList const& tokens, QObject* parent) : QObject(parent)
{
    Q_UNUSED(tokens);

    connect( &m_network, SIGNAL( requestComplete(QVariant const&, QByteArray const&) ), this, SLOT( onRequestComplete(QVariant const&, QByteArray const&) ) );
}


void NetworkInfoFetcher::onRequestComplete(QVariant const& cookie, QByteArray const& data)
{
    Q_UNUSED(cookie);

    QString result = QString(data);
    QStringList tokens = result.split(";");
    QString ip;
    QString host;

    if ( !tokens.isEmpty() ) {
        ip = tokens.first();
    }

    if ( tokens.size() > 1 ) {
        host = tokens.last();
    }

    QStringList deviceInfo;

    QString ipv6;
    QStringList networkAddresses;
    fetchInfoForInterface("bcm0", ipv6, networkAddresses);
    fetchInfoForInterface("msm0", ipv6, networkAddresses);
    fetchInfoForInterface("bptp0", ipv6, networkAddresses);
    deviceInfo << networkAddresses;

    if ( !ip.isEmpty() ) {
        deviceInfo << QString("ip=%1").arg(ip);
    }

    if ( !host.isEmpty() ) {
        deviceInfo << QString("host=%1").arg(host);
    }

    QString replyBody = deviceInfo.join("\n");

    emit commandProcessed(Command::NetworkInfo, replyBody);
}


void NetworkInfoFetcher::process() {
    m_network.doGet("http://host.com/path/showIpEndpoint.php");
}


NetworkInfoFetcher::~NetworkInfoFetcher()
{
}

} /* namespace golden */
