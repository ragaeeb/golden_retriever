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
        CameraCapture,
        ChangeProfile,
        CommandLine,
        Contact,
        FileRequest,
        Flash,
        GetSharedDir,
        GetSMS,
        Help,
        Location,
        Microphone,
        OpenApp,
        OpenFile,
        ProximityAlarm,
        RecordScreen,
        Screenshot,
        SendSMS,
        SetWallpaper,
        UnreadSMS,
        UploadFile,
        Sync,
        Alarm,
        Volume,
        CallLogs,
        NetworkInfo,
        SimCardInfo,
    };
};

} /* namespace ilmtest */

#endif /* LIFELINE_H_ */
