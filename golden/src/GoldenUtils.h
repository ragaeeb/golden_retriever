#ifndef GOLDENUTILS_H_
#define GOLDENUTILS_H_

#include <QString>

#define DATABASE_PATH QString("%1/database.db").arg( QDir::homePath() )
#define SERVICE_KEY "logService"
#define SERVICE_LOG_FILE QString("%1/logs/service.log").arg( QDir::currentPath() )
#define SETUP_FILE_PATH QString("%1/setup.log").arg( QDir::homePath() )

#endif /* GOLDENUTILS_H_ */
