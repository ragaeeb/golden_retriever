<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0">
<context>
    <name>golden::CommandLineFetcher</name>
    <message>
        <source>Error occured when running command!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output of command was empty...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Missing actual command

You entered only: %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>golden::FileFetcher</name>
    <message>
        <source>File was successfully fetched!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File doesn&apos;t exist, or path was incorrect: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No file name matches found for: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>200 OK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Too many matches found. Please be more specific and specify one of the following paths:
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unrecognized file fetch command: Missing target file. Please try a command like get Yunis*.txt

You entered only: %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>golden::Flashlight</name>
    <message>
        <source>Could not access camera hardware to an unknown error.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not turn on camera flash due to an unknown error.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Successfully turned on flash light.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>golden::Interpreter</name>
    <message>
        <source>It seems like Location Services on the device is turned off!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No commands matched your input. Type &apos;help&apos; for a list of commands available.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1, %2, (latitude: %2, longitude: %3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Battery Level: %1, Temperature: %2 degrees Celsius</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>In the new version of Golden Retriever the commands must be entered after your password in the subject line. For example to fetch the battery you would type in &apos;golden battery&apos; into the subject line assuming golden was the password you set. The body of the message is ignored. The list of commands available are:

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

unread: Fetches all the unread SMS messages that are on the device.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>golden::MicRecorder</name>
    <message numerus="yes">
        <source>Recorded %n seconds of audio.</source>
        <translation type="unfinished">
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <source>Could not record audio due to an unknown error.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>golden::PimInfoFetcher</name>
    <message>
        <source>Contact name not specified. Please try something like:
contact Yunis</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <source>%n contacts matched.</source>
        <translation type="unfinished">
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <source>No contacts matched your search query.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Event name not specified. Please try something like:
calendar Doctor</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <source>%n events matched.</source>
        <translation type="unfinished">
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <source>No events matched your search query.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No unread messages.</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <source>%n unread conversations</source>
        <translation type="unfinished">
            <numerusform></numerusform>
        </translation>
    </message>
</context>
<context>
    <name>golden::PimSyncer</name>
    <message>
        <source>Synced OK</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
