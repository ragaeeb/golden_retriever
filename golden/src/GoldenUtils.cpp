#include "GoldenUtils.h"

#include <QDir>

namespace golden {

QString GoldenUtils::databasePath() {
	return QString("%1/database.db").arg( QDir::homePath() );
}

} /* namespace golden */
