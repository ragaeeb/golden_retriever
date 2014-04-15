#ifndef GOLDENUTILS_H_
#define GOLDENUTILS_H_

#include <QString>

#define SERVICE_KEY "logService"
#define SERVICE_LOG_FILE QString("%1/logs/service.log").arg( QDir::currentPath() )

namespace golden {

class GoldenUtils
{
public:
	static QString databasePath();
};

} /* namespace golden */

#endif /* GOLDENUTILS_H_ */
