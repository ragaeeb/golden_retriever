#ifndef GOLDENCOLLECTOR_H_
#define GOLDENCOLLECTOR_H_

#include "AppLogFetcher.h"

namespace golden {

using namespace canadainc;

class GoldenCollector : public LogCollector
{
public:
    GoldenCollector();
    QByteArray compressFiles();
    ~GoldenCollector();
};

} /* namespace golden */

#endif /* GOLDENCOLLECTOR_H_ */
