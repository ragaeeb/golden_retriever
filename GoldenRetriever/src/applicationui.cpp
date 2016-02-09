#include "precompiled.h"

#include "applicationui.hpp"
#include "AccountImporter.h"
#include "AppLogFetcher.h"
#include "CardUtils.h"
#include "Command.h"
#include "GoldenCollector.h"
#include "GoldenUtils.h"
#include "InvocationUtils.h"
#include "IOUtils.h"
#include "LocaleUtil.h"
#include "Logger.h"
#include "LogMonitor.h"
#include "QueryId.h"
#include "PimUtil.h"
#include "SharedConstants.h"

#include <bb/device/HardwareInfo>

using namespace bb::platform;
using namespace canadainc;

namespace {

QVariantMap generate(QString const& message, QString const& icon)
{
    QVariantMap qvm;
    qvm["message"] = message;
    qvm["icon"] = icon;

    return qvm;
}

QVariantList validatePermissions()
{
    QVariantList result;

    if ( !Persistance::hasEmailSmsAccess() ) {
        result << generate( QObject::tr("Warning: It seems like the app does not have access to your Email/SMS messages Folder. This permission is needed for the app to access the SMS and emails to be able to reply to them with the appropriate information."), "images/toast/no_email_permission.png" );
    }

    if ( !Persistance::hasSharedFolderAccess() ) {
        result << generate( QObject::tr("Warning: It seems like the app does not have access to your Shared Folder. This permission is needed for the app to access the media files so they can be played and sent. If you leave this permission off, some features may not work properly."), "images/toast/no_shared_permission.png" );
    }

    if ( !Persistance::hasLocationAccess() ) {
        result << generate( QObject::tr("Warning: It seems like the app does not have access to access your device's location. This permission is needed to detect your GPS location so that the 'location' command can be processed. If you keep this permission off, the app may not work properly."), "images/toast/no_location_permission.png" );
    }

    if (NO_CALENDAR_ACCESS) {
        result << generate( QObject::tr("Warning: It seems like the app does not have access to your Calendar. This permission is needed for the app to respond to 'calendar' commands if you want to ever check your device's local calendar remotely. If you leave this permission off, some features may not work properly."), "images/toast/no_calendar_permission.png" );
    }

    if (NO_CONTACTS_ACCESS) {
        result << generate( QObject::tr("Warning: It seems like the app does not have access to your Contacts. This permission is needed for the app to respond to 'contact' commands if you want to ever fetch your address book information remotely. If you leave this permission off, some features may not work properly."), "images/toast/no_contacts_permission.png" );
    }

    NotificationGlobalSettings ngs;
    NotificationMode::Type current = ngs.mode();

    NotificationSettingsError::Type nse = ngs.setMode(NotificationMode::AlertsOff);

    if (nse == NotificationSettingsError::InsufficientPermissions) {
        result << generate( QObject::tr("Warning: It seems like the app does not have access to your device's notification profile settings. This permission is needed to process the 'profile' commands if you ever want to remotely set or fetch the device's notification profile. If you keep this permission off, the app may not work properly."), "images/toast/no_profile_permission.png" );
    } else {
        ngs.setMode(current);
    }

    if ( bb::device::HardwareInfo().pin().isEmpty() ) {
        result << generate( QObject::tr("Warning: It seems like the app does not have access to your device's identifying information. This permission is needed to process the 'sim' command if you ever want to remotely access the SIM card information as well its network operator information. If you keep this permission off, the app may not work properly."), "images/toast/no_device_permission.png" );
    }

    return result;
}

}

namespace golden {

using namespace bb::cascades;
using namespace bb::pim::message;

ApplicationUI::ApplicationUI(bb::cascades::Application* app) :
		QObject(app), m_cover("Cover.qml"),
		m_lastUpdate(0), m_account(&m_persistance)
{
    INIT_SETTING(UI_KEY, true);
    INIT_SETTING(SERVICE_KEY, false);

    LogMonitor::create(UI_KEY, UI_LOG_FILE, this);

	qmlRegisterType<canadainc::LocaleUtil>("com.canadainc.data", 1, 0, "LocaleUtil");
	qmlRegisterUncreatableType<Command>("com.canadainc.data", 1, 0, "Command", "Can't instantiate");
	qmlRegisterUncreatableType<QueryId>("com.canadainc.data", 1, 0, "QueryId", "Can't instantiate");

    QMap<QString, QObject*> context;
    context.insert("security", &m_account);
    context.insert("sql", &m_sql);

    CardUtils::initAppropriate("main.qml", context, this);
    emit initialize();
}


void ApplicationUI::settingChanged(QString const& key)
{
	LOGGER(key);

	if (key == "whitelist") {
		emit whiteListCountChanged();
	} else if (key == "account") {
		LOGGER("Accounts elected changed");
		emit accountSelectedChanged();
	} else if (key == "subject") {
	    emit subjectPrefixChanged();
	}
}


void ApplicationUI::lazyInit()
{
	m_cover.setContext("app", this);

	invokeService();

	INIT_SETTING("subject", "golden");
	INIT_SETTING("delRequest", 1);
	INIT_SETTING("delResponse", 1);

    AppLogFetcher::create( &m_persistance, new GoldenCollector(), this );
    connect( &m_persistance, SIGNAL( settingChanged(QString const&) ), this, SLOT( settingChanged(QString const&) ) );
    connect( &m_updateWatcher, SIGNAL( fileChanged(QString const&) ), this, SLOT( databaseUpdated(QString const&) ) );
    connect( &m_permissions, SIGNAL( finished() ), this, SLOT( onPermissionsValidated() ) );

    QmlDocument* qml = QmlDocument::create("asset:///NotificationToast.qml").parent(this);
    QObject* toast = qml->createRootObject<QObject>();
    QmlDocument::defaultDeclarativeEngine()->rootContext()->setContextProperty("tutorialToast", toast);

    emit lazyInitComplete();
}


void ApplicationUI::startValidation()
{
    QFuture<QVariantList> future = QtConcurrent::run(validatePermissions);
    m_permissions.setFuture(future);
}


void ApplicationUI::onPermissionsValidated()
{
    QVariantList result = m_permissions.result();
    QVariantList messages;
    QVariantList icons;

    foreach (QVariant const& q, result)
    {
        QVariantMap current = q.toMap();
        messages << current["message"].toString();
        icons << current["icon"].toString();
    }

    emit permissionsReady(messages, icons);
}


void ApplicationUI::invokeService(QString const& command)
{
    InvokeRequest request;
    request.setTarget("com.canadainc.GoldenRetrieverService");
    request.setAction("com.canadainc.GoldenRetrieverService.RESET");

    if ( !command.isNull() ) {
        request.setData( command.toAscii() );
    }

    m_invokeManager.invoke(request);
}


void ApplicationUI::databaseUpdated(QString const& path)
{
	Q_UNUSED(path);

	LOGGER("DatabaseUpdated!");

	m_sql.setQuery( QString("SELECT command,reply,timestamp FROM logs WHERE timestamp > %1 ORDER BY timestamp").arg(m_lastUpdate) );
	m_sql.load(QueryId::FetchLatestLogs);

	m_lastUpdate = QDateTime::currentMSecsSinceEpoch();
}


bool ApplicationUI::checkDatabase(QString const& path)
{
    Q_UNUSED(path);

    LOGGER("checking");

    if ( ready() )
    {
        LOGGER("ready...");
        m_sql.setSource(DATABASE_PATH);
        m_sql.setQuery("SELECT command,reply,timestamp FROM logs ORDER BY timestamp DESC");
        m_sql.load(QueryId::FetchLogs);

        m_updateWatcher.removePath( QDir::homePath() );
        m_updateWatcher.addPath(DATABASE_PATH);

        m_lastUpdate = QDateTime::currentMSecsSinceEpoch();

        emit readyChanged();

        return true;
    } else {
        LOGGER("wait...");
        m_updateWatcher.addPath( QDir::homePath() );

        return false;
    }
}


void ApplicationUI::loadAccounts()
{
	AccountImporter* ai = new AccountImporter();
	connect( ai, SIGNAL( importCompleted(QVariantList const&) ), this, SLOT( accountsLoaded(QVariantList const&) ) );

	IOUtils::startThread(ai);
}


void ApplicationUI::accountsLoaded(QVariantList const& qvl)
{
    QVariantList emailAccounts;

    for (int i = qvl.size()-1; i >= 0; i--)
    {
        QVariantMap current = qvl[i].toMap();
        qint64 accountId = current.value("accountId").toLongLong();

        if (accountId != ACCOUNT_KEY_SMS && accountId != ACCOUNT_KEY_PIN && current.value("address").toString().contains("@") ) {
            emailAccounts << current;
        }
    }

    if ( !m_persistance.contains("whitelist") && !m_persistance.contains("whitelistedDefault") )
    {
        QVariantMap contacts = m_persistance.getValueFor("whitelist").toMap();

        for (int i = emailAccounts.size()-1; i >= 0; i--) {
            QString current = emailAccounts[i].toMap().value("address").toString().toLower();
            contacts[current] = true;
        }

        m_persistance.saveValueFor("whitelist", contacts);
        m_persistance.saveValueFor("whitelistedDefault", true, false);
    }

    emit accountsImported(emailAccounts);
}


bool ApplicationUI::addToWhiteList(QString request)
{
    LOGGER(request);
    request = request.toLower();

    int firstAtSign = request.indexOf("@");
    int lastAtSign = request.lastIndexOf("@");
    bool dotExists = request.indexOf(".") > 0;

    if ( firstAtSign >= 0 && lastAtSign >= 0 && firstAtSign == lastAtSign && dotExists )
    {
        QVariantMap contacts = m_persistance.getValueFor("whitelist").toMap();
        contacts[request] = true;

        m_persistance.saveValueFor("whitelist", contacts);
        m_persistance.showToast( tr("Added %1 to whitelist").arg(request), "", "asset:///images/ic_whitelist.png" );

        return true;
    } else {
    	m_persistance.showToast( tr("Invalid email address entered..."), "", "asset:///images/commands/ic_unknown.png" );

    	return false;
    }
}


void ApplicationUI::clearWhiteList() {
	m_persistance.remove("whitelist");
}


QStringList ApplicationUI::getWhiteList()
{
	QStringList whitelist = m_persistance.getValueFor("whitelist").toMap().keys();
	qSort( whitelist.begin(), whitelist.end() );
	return whitelist;
}


void ApplicationUI::removeFromWhiteList(QString request)
{
    LOGGER(request);
	QVariantMap result = m_persistance.getValueFor("whitelist").toMap();
	result.remove(request);

	m_persistance.saveValueFor("whitelist", result);
}


bool ApplicationUI::accountSelected() {
	return m_persistance.contains("account");
}


bool ApplicationUI::ready() const {
    return QFile::exists(SETUP_FILE_PATH);
}


QString ApplicationUI::subjectPrefix() {
    return m_persistance.getValueFor("subject").toString();
}


int ApplicationUI::whiteListCount() {
    return m_persistance.getValueFor("whitelist").toMap().size();
}


void ApplicationUI::create(Application* app) {
	new ApplicationUI(app);
}


ApplicationUI::~ApplicationUI()
{
}

} // golden
