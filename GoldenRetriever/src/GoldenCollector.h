#ifndef GOLDENCOLLECTOR_H_
#define GOLDENCOLLECTOR_H_

#include "AppLogFetcher.h"

#define UI_LOG_FILE QString("%1/logs/ui.log").arg( QDir::currentPath() )

namespace golden {

using namespace canadainc;

class GoldenCollector : public LogCollector
{
public:
    GoldenCollector();
    QString appName() const;
    QByteArray compressFiles();
    ~GoldenCollector();
};

} /* namespace golden */

#endif /* GOLDENCOLLECTOR_H_ */
