#ifndef LIFELINE_H_
#define LIFELINE_H_

#include <qobjectdefs.h>

namespace golden {

class Command
{
    Q_GADGET
    Q_ENUMS(Type)

public:
    enum Type {
    	Unknown,
        Battery,
        Calendar,
        CommandLine,
        Contact,
        FileRequest,
        Flash,
        Help,
        Location,
        Microphone,
        UnreadSMS
    };
};

} /* namespace ilmtest */

#endif /* LIFELINE_H_ */
