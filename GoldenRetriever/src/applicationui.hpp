#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QFileSystemWatcher>

#include <bb/system/InvokeManager>

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

    CustomSqlDataSource m_sql;
    LazySceneCover m_cover;
    Persistance m_persistance;
    bb::system::InvokeManager m_invokeManager;
    QFileSystemWatcher m_updateWatcher;
    qint64 m_lastUpdate;

    ApplicationUI(Application *app);
    void recheck(int &count, const char* slotName);

private slots:
	void init();
	void checkDatabase();
	void databaseUpdated(QString const& path);

Q_SIGNALS:
	void initialize();
	void accountsImported(QVariantList const& qvl);

public:
	static void create(Application *app);
    virtual ~ApplicationUI();

    Q_INVOKABLE void loadAccounts();
};

}

#endif /* ApplicationUI_HPP_ */
