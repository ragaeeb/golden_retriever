#include "precompiled.h"

#include "InterpreterHelper.h"

#define PROFILE_NORMAL "normal"
#define PROFILE_SILENT "silent"
#define PROFILE_VIBRATE "vibrate"

namespace golden {

using namespace bb::platform;

QString InterpreterHelper::processVolume(QStringList const& tokens)
{
    QString message;

    if ( !tokens.isEmpty() )
    {
        unsigned int vol = tokens.first().toUInt();
        vol = qMin((unsigned int)100, vol);
        audiomixer_set_output_level(AUDIOMIXER_OUTPUT_SPEAKER, vol);

        message = QObject::tr("The master volume is current set to %1/100").arg(vol);
    } else {
        float currentVol;
        audiomixer_get_output_level(AUDIOMIXER_OUTPUT_SPEAKER, &currentVol);
        message = QObject::tr("The master volume was successfully set to %1/100").arg( QString::number(currentVol) );
    }

    return message;
}


QString InterpreterHelper::processProfile(QStringList const& tokens)
{
    QString message;
    NotificationGlobalSettings ngs;

    if ( !tokens.isEmpty() )
    {
        QString command = tokens.first();
        NotificationSettingsError::Type nse = NotificationSettingsError::None;

        if ( equals(PROFILE_VIBRATE) ) {
            nse = ngs.setMode(NotificationMode::Vibrate);
            message = QObject::tr("The device notification profile was successfully set to Vibrate");
        } else if ( equals(PROFILE_NORMAL) ) {
            nse = ngs.setMode(NotificationMode::Normal);
            message = QObject::tr("The device notification profile was successfully set to Normal");
        } else if ( equals(PROFILE_SILENT) ) {
            nse = ngs.setMode(NotificationMode::Silent);
            message = QObject::tr("The device notification profile was successfully set to Silent");
        } else {
            message = QObject::tr("The profile you specified could not match any known notification modes.");
        }

        if (nse == NotificationSettingsError::Internal) {
            message = QObject::tr("The profile you specified could not be set due to an internal error.");
        } else if (nse == NotificationSettingsError::InsufficientPermissions) {
            message = QObject::tr("The profile you specified could not be set since the app does not have enough permissions to do it. You will need to go into the BB10 Settings, Security & Privacy, Application Permissions, Golden Retriever and allow the appropriate permissions.");
        } else if (nse == NotificationSettingsError::ServiceUnavailable) {
            message = QObject::tr("The profile you specified could not be set because the Notification service is not available. The device may need to be restarted.");
        } else if (nse == NotificationSettingsError::Unknown) {
            message = QObject::tr("The profile you specified could not be set due to an unknown error.");
        }

    } else {
        QString currentProfile = QObject::tr("Unknown");

        switch ( ngs.mode() )
        {
            case NotificationMode::Silent:
                currentProfile = QObject::tr("Silent");
                break;
            case NotificationMode::Vibrate:
                currentProfile = QObject::tr("Vibrate");
                break;
            case NotificationMode::Normal:
                currentProfile = QObject::tr("Normal");
                break;
            case NotificationMode::PhoneOnly:
                currentProfile = QObject::tr("Phone Only");
                break;
            case NotificationMode::AlertsOff:
                currentProfile = QObject::tr("Alerts Off");
                break;
            default:
                break;
        }

        message = QObject::tr("The device profile is currently set to %1").arg(currentProfile);
    }

    return message;
}


QString InterpreterHelper::fetchBatteryInfo(QStringList const& tokens)
{
    Q_UNUSED(tokens);

    bb::device::BatteryInfo b;
    return QObject::tr("Battery Level: %1, Temperature: %2 degrees Celsius").arg( b.level() ).arg( b.temperature() );
}

} /* namespace golden */
