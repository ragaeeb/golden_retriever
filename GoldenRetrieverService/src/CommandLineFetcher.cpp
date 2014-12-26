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
	    QString inParam = m_tokens.join(" ");
	    QStringList strListReturn;
	    FILE *fp;
	    char path[2048];

	    printf("inParam: %s\n", inParam.toStdString().c_str());
	    fflush(stdout);

	    // Open the command for reading.
	    fp = popen(inParam.toStdString().c_str(), "r");

	    if (fp == NULL) {
	        printf("No data, something's super-wrong\n");
	        fflush(stdout);
	        pclose(fp);
	    }

	    // Read the output a line at a time - output it.
	    while ( fgets(path, sizeof(path) - 1, fp) != NULL )
	    {
	        //  printf("%s\n", path);fflush(stdout);
	        strListReturn.append(path);
	    }

	    // close
	    pclose(fp);

	    replyBody = strListReturn.join("\n");
	} else {
		replyBody = tr("Missing actual command\n\nYou entered only: %1").arg( m_tokens.join(" ") );
	}

	emit commandProcessed(Command::CommandLine, replyBody);
}


} /* namespace golden */
