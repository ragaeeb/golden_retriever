#ifndef QUERYID_H_
#define QUERYID_H_

#include <qobjectdefs.h>

namespace golden {

class QueryId
{
    Q_GADGET
    Q_ENUMS(Type)

public:
    enum Type {
    	ClearLogs,
    	FetchLogs,
    	FetchLatestLogs,
    	LogCommand,
    	Setup
    };
};

}

#endif /* QUERYID_H_ */
