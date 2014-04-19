import bb.cascades 1.0
import bb 1.0

Page
{
    titleBar: AboutTitleBar {
        id: atb
    }
    
    actions: [
        InvokeActionItem
        {
            title: atb.channelTitle
            ActionBar.placement: ActionBarPlacement.OnBar
            
            query {
                invokeTargetId: atb.invokeTargetId
                uri: atb.channelUri
            }
        },
        
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
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Fill

        ScrollView
        {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Fill

            Container
            {
                leftPadding: 10; rightPadding: 10
                
                Label {
                    topMargin: 0; bottomMargin: 0
                    multiline: true
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    textStyle.textAlign: TextAlign.Center
                    textStyle.fontSize: FontSize.Small
                    content.flags: TextContentFlag.ActiveText | TextContentFlag.EmoticonsOff
                    text: qsTr("Golden Retriever is a must have app for all BlackBerry users.\n\nThis app allows users to remotely control their BlackBerry 10 device using commands sent via email messages. This is particularly useful when the user may not have the device in their possession yet they need to interact with it. There are several occasions when this might happen.\n\nAn example is when the user has to lend their device to a friend or family for a period of time. In the meantime, the user would still like to know if there are any messages that came for them, or would still like to access some files that are stored in the device file system but since they do not have their device, they are currently out of reach. With this app, the user can simply send commands to the device to fetch this information even while they do not physically have access to the device.\n\nAnother example of a use case is if the user loses their phone, or misplaces it somewhere but they are in urgent need of their messages and some files stored in the file system. They might also want to actually track down where their phone is by using additional means outside of BlackBerry Protect. For example, they might want to hear the background noise around where the phone is currently to help narrow down where the phone may have been placed.\n\nAll of this and more can be done using Golden Retriever. When active, the app simply monitors a user's specified email inbox for issued commands. When the user is away from the phone, they can simply log into their email address from their laptop or desktop, and send an email command to this email address that the app is monitoring to fetch the information from the device they are looking for.\n\nTo issue a command, the user needs to email the target email address with the subject line: '%1' (without quotes) followed by the command they want to issue.\n\nOnce the user sends the email to the target address with the command after '%1' as the subject, the app will process the command and reply back to the email with the response.\n\nHere are some of the commands that are available (keep in mind future updates will continually bring more and more features):").arg(app.subjectPrefix);
                }
            }
        }
        
        ImageView {
            imageSource: "images/divider.png"
            horizontalAlignment: HorizontalAlignment.Fill
            topMargin: 0; bottomMargin: 0
        }
        
        ListView
        {
            dataModel: XmlDataModel {
                source: "xml/commands.xml"
            }
            
            listItemComponents: [
                ListItemComponent
                {
                    type: "command"
                    
                    Container
                    {
                        property bool bodyVisible: false
                        
                        Button
                        {
                            id: button
                            text: ListItemData.title
                            preferredWidth: Infinity
                            imageSource: bodyVisible ? "asset:///images/collapse.png" : "asset:///images/expand.png"

                            onClicked: {
                                bodyVisible = !bodyVisible;
                            }
                        }
                        
                        ControlDelegate
                        {
                            delegateActive: bodyVisible
                            
                            sourceComponent: ComponentDefinition
                            {
                                Container
                                {
                                    leftPadding: 10; rightPadding: 10; bottomPadding: 10
                                    
                                    Label
                                    {
                                        text: ListItemData.description
                                        textStyle.fontStyle: FontStyle.Italic
                                        multiline: true
                                    }
                                }
                            }
                        }
                    }
                }
            ]
        }
    }
}
