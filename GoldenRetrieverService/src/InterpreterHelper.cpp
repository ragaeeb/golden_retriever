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


QString InterpreterHelper::fetchHelp(QStringList const& tokens)
{
    Q_UNUSED(tokens);

    return QObject::tr("In the new version of Golden Retriever the commands must be entered after your password in the subject line. For example to fetch the battery you would type in 'golden battery' into the subject line assuming golden was the password you set. The body of the message is ignored. The list of commands available are:\n\nhelp: Returns the available list of commands with examples.\n\n"
            "battery: Returns the current battery level and the temperature in degrees Celsius.\n\n"
            "calendar: Does a query for calendar events that match the command arguments. For example, to find all events in the device calendar that contain the word 'Doctor', use the following command: calendar Doctor\n\n"
            "contact: Does a query for a contact's details (specified in the command arguments) from the device address book. For example, to find all contacts in the device address book that contain the name 'Yunis', use the following command: contact Yunis\n\n"
            "flash: Turns on the flash light for a specified amount of time. Follow it with the total duration that you wish to turn on the flashlight for. For example send 'flash 3' to turn on the light for 3 minutes. The maximum duration is 10 minutes.\n\n"
            "get: Fetches one or more files from the device's file system or media card. Wildcards are supported. For example, to fetch all image files on the device that start with 'Apple', use the following command:\nget Apple*.jpg\n\nTo fetch all text files that contain the word 'Yunis', use the following command:\nget *Yunis*.txt\n\nThere may be cases where your query is too broad, and matches too many files. For example, a query like:\nget A*.txt\ncan produce a really large result. In that case the app will return you a list of all the files that matched the query. You should then instead specify the exact file that you are looking for.\n\nTo fetch a very specific file, provide the full path to the file as an argument. For example, to fetch the file named 'abdul khaliq.txt' located in the documents folder of the device's SD card, use the following command:\nget /accounts/1000/removable/sdcard/documents/abdul khaliq.txt\n\nTo fetch the file 'abdul khaliq.txt' that is located in the documents folder of the device's local storage, use the following command instead:\nget /accounts/1000/shared/documents/abdul khaliq.txt\n\n"
            "location: Fetches the current location of the device reverse geocoded as well as in latitude/longitude coordinates.\n\n"
            "mic: Records audio from the device's microphone and sends it back. You can customize how long the device should record for by specifying an argument in terms of seconds. For example to record 15 seconds of audio, use the following command:\nmic 15\nIf no argument is specified after the *mic* command, the default value of 10 seconds is used.\n\n"
            "unread: Fetches all the unread SMS messages that are on the device.");
}

} /* namespace golden */
