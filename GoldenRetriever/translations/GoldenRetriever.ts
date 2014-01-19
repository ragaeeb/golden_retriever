<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0">
<context>
    <name>AccountsSheet</name>
    <message>
        <location filename="../assets/AccountsSheet.qml" line="8"/>
        <source>Choose the mailbox to monitor for the commands.

When you are away from your device, send the command messages to this mailbox so your device can process them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/AccountsSheet.qml" line="8"/>
        <source>OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/AccountsSheet.qml" line="15"/>
        <source>Active Account</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/AccountsSheet.qml" line="19"/>
        <source>Save</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/AccountsSheet.qml" line="33"/>
        <source>Cancel</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Cover</name>
    <message>
        <location filename="../assets/Cover.qml" line="18"/>
        <source>Monitoring...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HelpPage</name>
    <message>
        <location filename="../assets/HelpPage.qml" line="7"/>
        <source>Help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/HelpPage.qml" line="27"/>
        <source>Step by Step</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/HelpPage.qml" line="63"/>
        <source>(c) 2013 %1. All Rights Reserved.
%2 %3

Please report all bugs to:
support@canadainc.org

Golden Retriever is a must have app for all BlackBerry users.

This app allows users to remotely control their BlackBerry 10 device using commands sent via email messages. This is particularly useful when the user may not have the device in their possession yet they need to interact with it. There are several occasions when this might happen.

An example is when the user has to lend their device to a friend or family for a period of time. In the meantime, the user would still like to know if there are any messages that came for them, or would still like to access some files that are stored in the device file system but since they do not have their device, they are currently out of reach. With this app, the user can simply send commands to the device to fetch this information even while they do not physically have access to the device.

Another example of a use case is if the user loses their phone, or misplaces it somewhere but they are in urgent need of their messages and some files stored in the file system. They might also want to actually track down where their phone is by using additional means outside of BlackBerry Protect. For example, they might want to hear the background noise around where the phone is currently to help narrow down where the phone may have been placed.

All of this and more can be done using Golden Retriever. When active, the app simply monitors a user&apos;s specified email inbox for issued commands. When the user is away from the phone, they can simply log into their email address from their laptop or desktop, and send an email command to this email address that the app is monitoring to fetch the information from the device they are looking for.

To issue a command, the user needs to email the target email address with the subject line: &apos;golden&apos; (without quotes).

In the body of the message, the user needs to specify the command they want to issue. Here are some of the commands that are available (keep in mind future updates will continually bring more and more features):

help: Returns the available list of commands with examples.

battery: Returns the current battery level and the temperature in degrees Celsius.

calendar: Does a query for calendar events that match the command arguments. For example, to find all events in the device calendar that contain the word &apos;Doctor&apos;, use the following command: calendar Doctor

contact: Does a query for a contact&apos;s details (specified in the command arguments) from the device address book. For example, to find all contacts in the device address book that contain the name &apos;Yunis&apos;, use the following command: contact Yunis

flash: Turns on the flash light for a specified amount of time. Follow it with the total duration that you wish to turn on the flashlight for. For example send &apos;flash 3&apos; to turn on the light for 3 minutes. The maximum duration is 10 minutes.

get: Fetches one or more files from the device&apos;s file system or media card. Wildcards are supported. For example, to fetch all image files on the device that start with &apos;Apple&apos;, use the following command:
get Apple*.jpg

To fetch all text files that contain the word &apos;Yunis&apos;, use the following command:
get *Yunis*.txt

There may be cases where your query is too broad, and matches too many files. For example, a query like:
get A*.txt
can produce a really large result. In that case the app will return you a list of all the files that matched the query. You should then instead specify the exact file that you are looking for.

To fetch a very specific file, provide the full path to the file as an argument. For example, to fetch the file named &apos;abdul khaliq.txt&apos; located in the documents folder of the device&apos;s SD card, use the following command:
get /accounts/1000/removable/sdcard/documents/abdul khaliq.txt

To fetch the file &apos;abdul khaliq.txt&apos; that is located in the documents folder of the device&apos;s local storage, use the following command instead:
get /accounts/1000/shared/documents/abdul khaliq.txt

location: Fetches the current location of the device reverse geocoded as well as in latitude/longitude coordinates.

mic: Records audio from the device&apos;s microphone and sends it back. You can customize how long the device should record for by specifying an argument in terms of seconds. For example to record 15 seconds of audio, use the following command:
mic 15
If no argument is specified after the *mic* command, the default value of 10 seconds is used.

unread: Fetches all the unread SMS messages that are on the device.

Once the user sends the email to the target address with the command in the body and &apos;golden&apos; as the subject, the app will process the command and reply back to the email with the response.

To turn off monitoring your inbox, simply close the app. To activate it again, just reopen the app, it&apos;s that simple!

</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Instructions</name>
    <message>
        <location filename="../assets/Instructions.qml" line="6"/>
        <source>Step by Step</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SettingsPage</name>
    <message>
        <location filename="../assets/SettingsPage.qml" line="8"/>
        <source>Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/SettingsPage.qml" line="13"/>
        <source>Add Contact</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/SettingsPage.qml" line="24"/>
        <source>Email Address</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/SettingsPage.qml" line="25"/>
        <source>Enter the email address to whitelist:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/SettingsPage.qml" line="26"/>
        <source>OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/SettingsPage.qml" line="27"/>
        <source>Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/SettingsPage.qml" line="74"/>
        <source>You can restrict who is able to send commands to your device. There are currently no emails in your whitelist. To only accept commands from specific email addresses, click on the Add Contact action below and add them.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>golden::ApplicationUI</name>
    <message>
        <location filename="../src/applicationui.cpp" line="44"/>
        <source>Warning: It seems like the app does not have access to your Email/SMS messages Folder. This permission is needed for the app to access the SMS and email services it needs to validate messages and reply to them with the content you desire. If you leave this permission off, some features may not work properly. Select OK to launch the Application Permissions screen where you can turn these settings on.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/applicationui.cpp" line="48"/>
        <source>Warning: It seems like the app does not have access to your Shared Folder. This permission is needed for the app to access the media files so they can be played. If you leave this permission off, some features may not work properly.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/applicationui.cpp" line="52"/>
        <source>Warning: It seems like the app does not have access to your Calendar. This permission is needed for the app to respond to &apos;calendar&apos; commands if you want to ever check your device&apos;s local calendar remotely. If you leave this permission off, some features may not work properly.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/applicationui.cpp" line="55"/>
        <source>Warning: It seems like the app does not have access to access your device&apos;s location. This permission is needed to detect your GPS location so that the &apos;location&apos; command can be processed. If you keep this permission off, the app may not work properly.

Press OK to launch the application permissions, then go to Golden Retriever and please enable the Location permission.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/applicationui.cpp" line="116"/>
        <source>Error initializing link with service. Please restart your device...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>main</name>
    <message>
        <location filename="../assets/main.qml" line="26"/>
        <source>Golden Retriever</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/main.qml" line="31"/>
        <source>Account</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/main.qml" line="56"/>
        <source>Welcome to Golden Retriever. This app can be used to remotely control your BlackBerry device using emails. To learn more about the commands available, swipe-down from the top-bezel and choose Help.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/main.qml" line="73"/>
        <source>Warning: The app will respond to anyone who issues a command in the active mailbox. If you want to restrict only specific users (ie: yourself) to be able to issue commands, swipe-down from the top-bezel, go to Settings use the Add action to add the users who are allowed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/main.qml" line="107"/>
        <source>Unknown Command</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/main.qml" line="116"/>
        <source>Battery Status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/main.qml" line="125"/>
        <source>Calendar Query</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/main.qml" line="134"/>
        <source>Contact Query</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/main.qml" line="143"/>
        <source>File Request</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/main.qml" line="152"/>
        <source>Help Query</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/main.qml" line="161"/>
        <source>Location Query</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/main.qml" line="170"/>
        <source>Audio Record</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../assets/main.qml" line="179"/>
        <source>Unread SMS Query</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
