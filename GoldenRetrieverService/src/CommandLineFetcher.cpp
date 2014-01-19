#include "precompiled.h"

#include "CommandLineFetcher.h"
#include "Command.h"
#include "Logger.h"

namespace golden {

CommandLineFetcher::CommandLineFetcher(QStringList const& tokens) : m_tokens(tokens)
{
}

void CommandLineFetcher::run()
{
	QString replyBody;

	if ( !m_tokens.isEmpty() )
	{
	    char buffer[1024];

	    QByteArray byteArray = m_tokens.join(" ").toUtf8();
	    const char* cString = byteArray.constData();
	    FILE* fd = popen(cString, "r");

	    if (fd == NULL) {
	    	replyBody = tr("Error occured when running command!");
	    }

	    while( NULL != fgets(buffer, sizeof(buffer), fd) )
	    {
	    	QString s(buffer);
	    	replyBody += s.trimmed();
	    }

	    if ( replyBody.isEmpty() ) {
	    	replyBody = tr("Output of command was empty...");
	    }

	    pclose(fd);
	} else {
		replyBody = tr("Missing actual command\n\nYou entered only: %1").arg( m_tokens.join(" ") );
	}

	emit commandProcessed(Command::CommandLine, replyBody);
}


} /* namespace golden */
