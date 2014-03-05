import bb.cascades 1.0
import bb 1.0

Page
{
    titleBar: TitleBar {
        title: qsTr("Help") + Retranslate.onLanguageChanged
    }
    
    attachedObjects: [
        ApplicationInfo {
            id: appInfo
        },

        PackageInfo {
            id: packageInfo
        }
    ]
    
    paneProperties: NavigationPaneProperties {
        property variant navPane: navigationPane
        id: properties
    }
    
    actions: [
        InvokeActionItem {
            query {
                mimeType: "text/html"
                uri: "http://www.youtube.com/watch?v=8hDCBJosXGQ"
                invokeActionId: "bb.action.OPEN"
            }
            
            imageSource: "images/ic_steps.png"
            title: qsTr("Video Tutorial") + Retranslate.onLanguageChanged
            ActionBar.placement: ActionBarPlacement.OnBar
        }
    ]

    Container
    {
        leftPadding: 20; rightPadding: 20

        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Fill

        ScrollView {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Fill

            Label {
                multiline: true
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                textStyle.textAlign: TextAlign.Center
                textStyle.fontSize: FontSize.Small
                content.flags: TextContentFlag.ActiveText | TextContentFlag.EmoticonsOff
                text: qsTr("(c) 2013 %1. All Rights Reserved.\n%2 %3\n\nPlease report all bugs to:\nsupport@canadainc.org\n\nGolden Retriever is a must have app for all BlackBerry users.\n\nThis app allows users to remotely control their BlackBerry 10 device using commands sent via email messages. This is particularly useful when the user may not have the device in their possession yet they need to interact with it. There are several occasions when this might happen.\n\nAn example is when the user has to lend their device to a friend or family for a period of time. In the meantime, the user would still like to know if there are any messages that came for them, or would still like to access some files that are stored in the device file system but since they do not have their device, they are currently out of reach. With this app, the user can simply send commands to the device to fetch this information even while they do not physically have access to the device.\n\nAnother example of a use case is if the user loses their phone, or misplaces it somewhere but they are in urgent need of their messages and some files stored in the file system. They might also want to actually track down where their phone is by using additional means outside of BlackBerry Protect. For example, they might want to hear the background noise around where the phone is currently to help narrow down where the phone may have been placed.\n\nAll of this and more can be done using Golden Retriever. When active, the app simply monitors a user's specified email inbox for issued commands. When the user is away from the phone, they can simply log into their email address from their laptop or desktop, and send an email command to this email address that the app is monitoring to fetch the information from the device they are looking for.\n\nTo issue a command, the user needs to email the target email address with the subject line: 'golden' (without quotes).\n\nIn the body of the message, the user needs to specify the command they want to issue. Here are some of the commands that are available (keep in mind future updates will continually bring more and more features):\n\nhelp: Returns the available list of commands with examples.\n\n"
                +"battery: Returns the current battery level and the temperature in degrees Celsius.\n\n"
                +"calendar: Does a query for calendar events that match the command arguments. For example, to find all events in the device calendar that contain the word 'Doctor', use the following command: calendar Doctor\n\n"
                +"contact: Does a query for a contact's details (specified in the command arguments) from the device address book. For example, to find all contacts in the device address book that contain the name 'Yunis', use the following command: contact Yunis\n\n"
                +"flash: Turns on the flash light for a specified amount of time. Follow it with the total duration that you wish to turn on the flashlight for. For example send 'flash 3' to turn on the light for 3 minutes. The maximum duration is 10 minutes.\n\n"
                +"get: Fetches one or more files from the device's file system or media card. Wildcards are supported. For example, to fetch all image files on the device that start with 'Apple', use the following command:\nget Apple*.jpg\n\nTo fetch all text files that contain the word 'Yunis', use the following command:\nget *Yunis*.txt\n\nThere may be cases where your query is too broad, and matches too many files. For example, a query like:\nget A*.txt\ncan produce a really large result. In that case the app will return you a list of all the files that matched the query. You should then instead specify the exact file that you are looking for.\n\nTo fetch a very specific file, provide the full path to the file as an argument. For example, to fetch the file named 'abdul khaliq.txt' located in the documents folder of the device's SD card, use the following command:\nget /accounts/1000/removable/sdcard/documents/abdul khaliq.txt\n\nTo fetch the file 'abdul khaliq.txt' that is located in the documents folder of the device's local storage, use the following command instead:\nget /accounts/1000/shared/documents/abdul khaliq.txt\n\n"
                +"location: Fetches the current location of the device reverse geocoded as well as in latitude/longitude coordinates.\n\n"
                +"mic: Records audio from the device's microphone and sends it back. You can customize how long the device should record for by specifying an argument in terms of seconds. For example to record 15 seconds of audio, use the following command:\nmic 15\nIf no argument is specified after the *mic* command, the default value of 10 seconds is used.\n\n"
                +"unread: Fetches all the unread SMS messages that are on the device.\n\nOnce the user sends the email to the target address with the command in the body and 'golden' as the subject, the app will process the command and reply back to the email with the response.\n\nTo turn off monitoring your inbox, simply close the app. To activate it again, just reopen the app, it's that simple!\n\n").arg(packageInfo.author).arg(appInfo.title).arg(appInfo.version)
            }
        }
    }
}
