#include "precompiled.h"

#include "HelpFetcher.h"
#include "Command.h"
#include "Logger.h"

namespace golden {

using namespace bb::data;

HelpFetcher::HelpFetcher(QStringList const& tokens)
{
    Q_UNUSED(tokens);
}

void HelpFetcher::run()
{
    LOGGER("Running");
    QStringList result( QString("In the new version of Golden Retriever the commands must be entered after your keyword in the subject line. For example to fetch the battery you would type in '%1 battery' into the subject line. The body of the message is ignored. The list of commands available are:").arg( QSettings().value("subject").toString() ) );

    QVariantList qvl = XmlDataAccess().load("app/native/assets/xml/commands.xml", "command").toList();

    for (int i = qvl.size()-1; i >= 0; i--)
    {
        QVariantMap current = qvl[i].toMap();
        result << tr("%1: %2").arg( current.value("title").toString() ).arg( current.value("description").toString() );
    }

	emit commandProcessed(Command::Help, result.join("\n"), QVariantList());
}


} /* namespace golden */
