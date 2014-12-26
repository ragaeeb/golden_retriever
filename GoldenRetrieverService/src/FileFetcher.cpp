#include "precompiled.h"

#include "FileFetcher.h"
#include "Command.h"
#include "IOUtils.h"
#include "Logger.h"

namespace {

QFileInfoList fetchFiles(QString const& dirPath, QString const& targetFile)
{
	QDir selectedDir(dirPath);
	QMap<QString, QFileInfo> result;

	if ( selectedDir.exists() )
	{
		selectedDir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
		selectedDir.setNameFilters( QStringList() << targetFile );

	    QDirIterator it(selectedDir, QDirIterator::Subdirectories);

	    while ( it.hasNext() ) {
	    	QFileInfo qfi = QFileInfo( it.next() );
	        result[ qfi.filePath() ] = qfi;
	    }
	}

    return result.values();
}

}

namespace golden {

using namespace canadainc;
using namespace bb::pim::message;

FileFetcher::FileFetcher(QStringList const& tokens) : m_tokens(tokens)
{
}

void FileFetcher::run()
{
	QVariantList attachments;
	QString replyBody;

	if ( !m_tokens.isEmpty() )
	{
		QString target = m_tokens.first();

		if ( target.contains("/") )
		{
			target = m_tokens.join(" "); // since the spaces were split, if the filename has spaces then it will mess it up
			QFileInfo f(target);

			if ( f.exists() ) {
				replyBody = tr("File was successfully fetched!");
				attachments << QVariant::fromValue( Attachment( "", f.fileName(), QUrl::fromLocalFile(target) ) );
			} else {
				replyBody = tr("File doesn't exist, or path was incorrect: %1").arg(target);
			}
		} else {
			QFileInfoList result = fetchFiles(directory_sdcard, target);
			result.append( fetchFiles(directory_local_shared, target) );

			if ( result.isEmpty() ) {
				replyBody = tr("No file name matches found for: %1").arg(target);
			} else if ( result.size() < 10 ) {
				for (int i = result.size()-1; i >= 0; i--) {
					attachments << QVariant::fromValue( Attachment( "", result[i].fileName(), QUrl::fromLocalFile( result[i].filePath() ) ) );
				}

				replyBody = tr("200 OK");
			} else {
				replyBody = tr("Too many matches found. Please be more specific and specify one of the following paths:\n");

				for (int i = result.size()-1; i > 0; i--) {
					replyBody += result[i].filePath()+"\n";
				}

				replyBody += result[0].filePath();
			}
		}
	} else {
		replyBody = tr("Unrecognized file fetch command: Missing target file. Please try a command like get Yunis*.txt\n\nYou entered only: %1").arg( m_tokens.join(" ") );
	}

	emit commandProcessed(Command::FileRequest, replyBody, attachments);
}


} /* namespace golden */
