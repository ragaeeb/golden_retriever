#include "precompiled.h"

#include "applicationui.hpp"
#include "AccountImporter.h"
#include "AppLogFetcher.h"
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

#define UI_KEY "logUI"

namespace golden {

using namespace bb::cascades;
using namespace bb::pim::message;
using namespace bb::platform;
using namespace canadainc;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
		QObject(app), m_cover("Cover.qml"),
		m_lastUpdate(0), m_account(&m_persistance)
{
    INIT_SETTING(UI_KEY, true);
    INIT_SETTING(SERVICE_KEY, false);

    new AppLogFetcher( new GoldenCollector(), this );
    new LogMonitor(UI_KEY, UI_LOG_FILE, this);

	qmlRegisterType<canadainc::LocaleUtil>("com.canadainc.data", 1, 0, "LocaleUtil");
	qmlRegisterUncreatableType<Command>("com.canadainc.data", 1, 0, "Command", "Can't instantiate");
	qmlRegisterUncreatableType<QueryId>("com.canadainc.data", 1, 0, "QueryId", "Can't instantiate");
	qmlRegisterType<bb::device::DisplayInfo>("bb.device", 1, 0, "DisplayInfo");

    QmlDocument* qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("app", this);
    qml->setContextProperty("persist", &m_persistance);
    qml->setContextProperty("sql", &m_sql);
    qml->setContextProperty("security", &m_account);

    AbstractPane* root = qml->createRootObject<AbstractPane>();
    app->setScene(root);

	connect( &m_persistance, SIGNAL( settingChanged(QString const&) ), this, SLOT( settingChanged(QString const&) ) );
	connect( this, SIGNAL( initialize() ), this, SLOT( init() ), Qt::QueuedConnection ); // async startup

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


void ApplicationUI::init()
{
	m_cover.setContext("app", this);

	invokeService();

	bool ok = PimUtil::validateEmailSMSAccess( tr("Warning: It seems like the app does not have access to your Email/SMS messages Folder. This permission is needed for the app to access the SMS and email services it needs to validate messages and reply to them with the content you desire. If you leave this permission off, some features may not work properly. Select OK to launch the Application Permissions screen where you can turn these settings on.") );

	if (ok)
	{
		ok = InvocationUtils::validateSharedFolderAccess( tr("Warning: It seems like the app does not have access to your Shared Folder. This permission is needed for the app to access the media files so they can be played and sent. If you leave this permission off, some features may not work properly.") );

		if (ok)
		{
			ok = PimUtil::validateCalendarAccess( tr("Warning: It seems like the app does not have access to your Calendar. This permission is needed for the app to respond to 'calendar' commands if you want to ever check your device's local calendar remotely. If you leave this permission off, some features may not work properly.") );

			if (ok)
			{
				ok = InvocationUtils::validateLocationAccess( tr("Warning: It seems like the app does not have access to access your device's location. This permission is needed to detect your GPS location so that the 'location' command can be processed. If you keep this permission off, the app may not work properly.\n\nPress OK to launch the application permissions, then go to Golden Retriever and please enable the Location permission.") );

				if (ok)
				{
				    NotificationGlobalSettings ngs;
				    NotificationMode::Type current = ngs.mode();

				    NotificationSettingsError::Type nse = ngs.setMode(NotificationMode::AlertsOff);
                    LOGGER("NSE Permissions" << nse);

				    if ( nse == NotificationSettingsError::InsufficientPermissions )
				    {
				        LOGGER("No permissions");
				        m_persistance.showToast( tr("Warning: It seems like the app does not have access to your device's notification profile settings. This permission is needed to process the 'profile' commands if you ever want to remotely set or fetch the device's notification profile. If you keep this permission off, the app may not work properly.\n\nPress OK to launch the application permissions, then go to Golden Retriever and please enable the Location permission."), tr("OK"), "asset:///images/commands/ic_change_profile.png" );
				        InvocationUtils::launchAppPermissionSettings();
				    } else {
				        ngs.setMode(current);
				    }
				}
			}
		}
	}

	INIT_SETTING("subject", "golden");
	INIT_SETTING("delRequest", 1);
	INIT_SETTING("delResponse", 1);
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

	LOGGER("Database updated!");

	m_sql.setQuery( QString("SELECT command,reply,timestamp FROM logs WHERE timestamp > %1 ORDER BY timestamp").arg(m_lastUpdate) );
	m_sql.load(QueryId::FetchLatestLogs);

	m_lastUpdate = QDateTime::currentMSecsSinceEpoch();
}


void ApplicationUI::checkDatabase()
{
	QString database = GoldenUtils::databasePath();

	if ( QFile::exists(database) )
	{
		m_lastUpdate = QDateTime::currentMSecsSinceEpoch();

		m_sql.setSource(database);
		m_sql.setQuery("SELECT command,reply,timestamp FROM logs ORDER BY timestamp DESC");
		m_sql.load(QueryId::FetchLogs);

		connect( &m_updateWatcher, SIGNAL( fileChanged(QString const&) ), this, SLOT( databaseUpdated(QString const&) ) );
		m_updateWatcher.addPath(database);
	} else {
		LOGGER("Database does not exist");
		static int count = 0;
		recheck( count, SLOT( checkDatabase() ) );
	}
}


void ApplicationUI::recheck(int &count, const char* slotName)
{
	LOGGER("Database does not exist");
	++count;

	if (count < 5) {
		LOGGER("Retrying" << count);
		QTimer::singleShot(2000*count, this, slotName);
	} else {
		LOGGER("Can't connect...");
		m_persistance.showToast( tr("Error initializing link with service. Please restart your device..."), "", "asset:///images/commands/ic_unknown.png" );
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

    if ( !m_persistance.contains("whitelist") )
    {
        QVariantMap contacts = m_persistance.getValueFor("whitelist").toMap();

        for (int i = emailAccounts.size()-1; i >= 0; i--) {
            QString current = emailAccounts[i].toMap().value("address").toString().toLower();
            contacts[current] = true;
        }

        m_persistance.saveValueFor("whitelist", contacts);
    }

    emit accountsImported(emailAccounts);
}


bool ApplicationUI::addToWhiteList(QString request)
{
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
	QVariantMap result = m_persistance.getValueFor("whitelist").toMap();
	result.remove(request);

	m_persistance.saveValueFor("whitelist", result);
}


bool ApplicationUI::accountSelected() {
	return m_persistance.contains("account");
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
