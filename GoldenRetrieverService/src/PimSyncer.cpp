#include "precompiled.h"

#include "PimSyncer.h"
#include "Command.h"
#include "Logger.h"

namespace golden {

using namespace bb::pim::account;
using namespace bb::pim::message;

PimSyncer::PimSyncer(QStringList const& tokens)
{
    Q_UNUSED(tokens);
}

void PimSyncer::run()
{
    LOGGER("Running");
    QList<qint64> list;
    QList<Account> accountList = AccountService().accounts(Service::Messages);

    LOGGER( "Account list" << accountList.size() );

    foreach (Account const& account, accountList) {
        list << account.id();
    }

    LOGGER("Syncing");

    MessageService().syncAccounts(list);

    LOGGER("Synced!");

	emit commandProcessed(Command::Sync, tr("Synced OK"), QVariantList());
}


} /* namespace golden */
