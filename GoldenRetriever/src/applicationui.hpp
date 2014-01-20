#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QFileSystemWatcher>

#include <bb/system/InvokeManager>

#include "AccountManager.h"
#include "customsqldatasource.h"
#include "LazySceneCover.h"
#include "Persistance.h"

namespace bb {
	namespace cascades {
		class Application;
	}
}

namespace golden {

using namespace bb::cascades;
using namespace canadainc;

class ApplicationUI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int whiteListCount READ whiteListCount NOTIFY whiteListCountChanged)
    Q_PROPERTY(bool accountSelected READ accountSelected NOTIFY accountSelectedChanged)

    CustomSqlDataSource m_sql;
    LazySceneCover m_cover;
    Persistance m_persistance;
    bb::system::InvokeManager m_invokeManager;
    QFileSystemWatcher m_updateWatcher;
    qint64 m_lastUpdate;
    AccountManager m_account;

    ApplicationUI(Application *app);
    void recheck(int &count, const char* slotName);

private slots:
	void init();
	void databaseUpdated(QString const& path);
	void settingChanged(QString const& key);

Q_SIGNALS:
	void initialize();
	void accountsImported(QVariantList const& qvl);
	void whiteListCountChanged();
	void accountSelectedChanged();

public:
	static void create(Application *app);
    virtual ~ApplicationUI();

    bool accountSelected();
    Q_INVOKABLE void addToWhiteList(QString request);
	Q_SLOT void checkDatabase();
    Q_INVOKABLE void clearWhiteList();
    Q_INVOKABLE QStringList getWhiteList();
    Q_INVOKABLE void loadAccounts();
    Q_INVOKABLE void removeFromWhiteList(QString request);
    int whiteListCount();
};

}

#endif /* ApplicationUI_HPP_ */
