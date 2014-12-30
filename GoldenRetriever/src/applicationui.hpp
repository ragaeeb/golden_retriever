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
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
    Q_PROPERTY(QString subjectPrefix READ subjectPrefix NOTIFY subjectPrefixChanged)

    CustomSqlDataSource m_sql;
    LazySceneCover m_cover;
    Persistance m_persistance;
    bb::system::InvokeManager m_invokeManager;
    QFileSystemWatcher m_updateWatcher;
    qint64 m_lastUpdate;
    AccountManager m_account;
    QFutureWatcher<QVariantList> m_permissions;

    ApplicationUI(Application *app);
    void recheck(int &count, const char* slotName);

private slots:
    void accountsLoaded(QVariantList const& qvl);
	void databaseUpdated(QString const& path);
    void lazyInit();
    void onPermissionsValidated();
	void settingChanged(QString const& key);

Q_SIGNALS:
    void accountsImported(QVariantList const& qvl);
    void accountSelectedChanged();
	void initialize();
    void lazyInitComplete();
    void permissionsReady(QVariantList const& allMessages, QVariantList const& allIcons);
    void readyChanged();
    void subjectPrefixChanged();
	void whiteListCountChanged();

public:
	static void create(Application *app);
    virtual ~ApplicationUI();

    bool accountSelected();
    Q_INVOKABLE bool addToWhiteList(QString request);
    Q_SLOT bool checkDatabase(QString const& path=QString());
    Q_INVOKABLE void clearWhiteList();
    Q_INVOKABLE QStringList getWhiteList();
    Q_INVOKABLE void loadAccounts();
    Q_INVOKABLE void removeFromWhiteList(QString request);
    Q_INVOKABLE void invokeService(QString const& command=QString());
    Q_INVOKABLE void startValidation();
    bool ready() const;
    QString subjectPrefix();
    int whiteListCount();
};

}

#endif /* ApplicationUI_HPP_ */
