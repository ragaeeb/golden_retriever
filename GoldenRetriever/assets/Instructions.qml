import bb.cascades 1.0

Page
{
    titleBar: TitleBar {
        title: qsTr("Step by Step") + Retranslate.onLanguageChanged
    }
    
    ListView
    {
        id: listView

        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        leftPadding: 20
        rightPadding: 20

        dataModel: GroupDataModel {
            id: adm
            sortingKeys: ["group"]
            grouping: ItemGrouping.ByFullValue
        }

        listItemComponents:
        [
            ListItemComponent {
                type: "header"
                
                Header {
                    title: ListItemData
                }
            },
            
            ListItemComponent
            {
                type: "item"
                
                Container
                {
                    topPadding: 10; bottomPadding: 20
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill

                    ImageView {
                        imageSource: ListItemData.imageSource
                        bottomMargin: 0
                    }
                    
                    Label {
                        topMargin: 0
                        text: ListItemData.text
                        multiline: true
                        textStyle.fontSize: FontSize.XSmall
                    }
                }
            }
        ]

        onCreationCompleted: {
            adm.insert({
                'text': "As you can see, send an email with the subject line 'golden', and 'help' in the body text to the mailbox that you chose to be an Active Account from the main screen.",
                'imageSource': "images/screenshots/command_help_send.png",
                'group': "help"
            });
            adm.insert({
                'text': "The app will intercept this message, and send you back all the commands that are available.",
                'imageSource': "images/screenshots/command_help_response.png",
                'group': "help"
            });
            adm.insert({
                'text': "If you want to query for a specific event you have saved in your device's local calendar, send an email with the subject 'golden' and the message body as 'calendar' followed by the event you want to query for the time..",
                'imageSource': "images/screenshots/command_calendar_send.png",
                'group': "calendar"
            });
            adm.insert({
                'text': "The app will intercept this message, and send you back all the matches of events it found on the device as plain-text attachments.",
                'imageSource': "images/screenshots/command_calendar_response.png",
                'group': "calendar"
            });
            adm.insert({
                'text': "If you would like to fetch a file from the device, set the subject of the email as 'golden' and the body as 'get' followed by the file you are looking for. This can be the exact file name or wildcards may be used. For example, in this scenario we are looking for any audio file that starts with the word 'Question'.",
                'imageSource': "images/screenshots/command_file_request_send.png",
                'group': "get"
            });
            adm.insert({
                'text': "The app will intercept this message, query the file system and SD card storage and send you back the file(s) found.",
                'imageSource': "images/screenshots/command_file_request_response.png",
                'group': "get"
            });
	        adm.insert({
                'text': "Be careful about making the query too broad.",
                'imageSource': "images/screenshots/command_file_request_many_send.png",
                'group': "get"
	        });
		    adm.insert({
	            'text': "The app will intercept this message, query the file system and SD card storage and if it finds more than 10 matches, it will simply reply back with the files that matched the query. It is now up to you to resend another message to be more specific in the file that you want, or issue a get command followed by the full path to the file.",
	            'imageSource': "images/screenshots/command_file_request_many_response.png",
                'group': "get"
		    });
            adm.insert({
                'text': "If you want to record some seconds of audio on your device, set the email subject as 'golden' and the body as 'mic'. You can optionally follow it with the number of seconds you want to record for. If this extra parameter is omitted, the app will record for 10 seconds.",
                'imageSource': "images/screenshots/command_mic_send.png",
                'group': "mic"
            });
	        adm.insert({
                'text': "The app will intercept this message, and immediately turn on the mic to start recording for the specified duration. Once the recording is complete the app will reply to you with the recording as an attached file.",
                'imageSource': "images/screenshots/command_mic_response.png",
                'group': "mic"
	        });
	        adm.insert({
                'text': "If you would like to fetch all the unread SMS messages that exist on your device, set the subject of the email as 'golden' and the body as 'unread sms'.",
                'imageSource': "images/screenshots/command_unread_send.png",
                'group': "unread sms"
	        });
		    adm.insert({
	            'text': "The app will intercept this message, then fetch all the unread SMS messages that are on your device and send them to you as attachments. As you can see in the screenshot, the name of the file is the phone number associated with the unread text message.",
	            'imageSource': "images/screenshots/command_unread_response.png",
                'group': "unread sms"
		    });
        }
    }
}