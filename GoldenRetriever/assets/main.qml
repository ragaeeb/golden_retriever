import bb.cascades 1.0
import com.canadainc.data 1.0

NavigationPane
{
    id: navigationPane
    
    attachedObjects: [
        ComponentDefinition {
            id: definition
        }
    ]
    
    Menu.definition: CanadaIncMenu {
        projectName: "golden-retriever"
        bbWorldID: "23334872"
    }
    
    onPopTransitionEnded: {
        page.destroy();
    }
    
    Page
    {
        titleBar: TitleBar {
            title: qsTr("Golden Retriever") + Retranslate.onLanguageChanged
        }
        
        actions: [
            ActionItem {
                title: qsTr("Account") + Retranslate.onLanguageChanged
                imageSource: "images/ic_account.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                
                onTriggered: {
                    definition.source = "AccountsSheet.qml";
                    var sheet = definition.createObject();
                    sheet.open();
                }
                
                onCreationCompleted: {
                    if ( !persist.contains("account") ) {
                        triggered();
                    }
                }
            }
        ]
        
        Container
        {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            topPadding: 10; bottomPadding: 10; rightPadding: 10; leftPadding: 10
            
            Label {
                text: qsTr("Welcome to Golden Retriever. This app can be used to remotely control your BlackBerry device using emails. To learn more about the commands available, swipe-down from the top-bezel and choose Help.") + Retranslate.onLanguageChanged
                textStyle.fontSize: FontSize.XSmall
                textStyle.textAlign: TextAlign.Center
                multiline: true
                horizontalAlignment: HorizontalAlignment.Fill
            }
            
            Divider {
                topMargin: 0; bottomMargin: 0
            }
            
            ListView
            {
                id: listView
                property alias localizer: localeUtil
                
                leadingVisual: Label {
                    text: qsTr("Warning: The app will respond to anyone who issues a command in the active mailbox. If you want to restrict only specific users (ie: yourself) to be able to issue commands, swipe-down from the top-bezel, go to Settings use the Add action to add the users who are allowed.") + Retranslate.onLanguageChanged
                    textStyle.fontSize: FontSize.XSmall
                    textStyle.textAlign: TextAlign.Center
                    textStyle.color: Color.Red
                    multiline: true
                    horizontalAlignment: HorizontalAlignment.Fill
                    topMargin: 20
                    visible: false
                    
                    function onSettingChanged(key)
                    {
                        if (key == "contacts") {
                            var whitelisted = persist.contains("contacts");
                            visible = !whitelisted || persist.getValueFor("contacts").length == 0;
                        }
                    }
                    
                    onCreationCompleted: {
                        persist.settingChanged.connect(onSettingChanged);
                        onSettingChanged("contacts");
                        
                        if (visible)
                        {
                            listView.scrollToPosition(0, ScrollAnimation.None);
                            listView.scroll(-200, ScrollAnimation.Smooth);
                        }
                    }
                }
                
                listItemComponents: [
                    ListItemComponent {
                        type: ""+Command.Unknown
                        
                        CommandListItem {
                            title: qsTr("Unknown Command") + Retranslate.onLanguageChanged
                            imageSource: "images/ic_unknown.png"
                        }
                    },                    

                    ListItemComponent {
                        type: ""+Command.Battery
                        
                        CommandListItem {
                            title: qsTr("Battery Status") + Retranslate.onLanguageChanged
                            imageSource: "images/ic_battery.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.Calendar
                        
                        CommandListItem {
                            title: qsTr("Calendar Query") + Retranslate.onLanguageChanged
                            imageSource: "images/ic_calendar.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.Contact
                        
                        CommandListItem {
                            title: qsTr("Contact Query") + Retranslate.onLanguageChanged
                            imageSource: "images/ic_contact_fetch.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.FileRequest
                        
                        CommandListItem {
                            title: qsTr("File Request") + Retranslate.onLanguageChanged
                            imageSource: "images/ic_file.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.Help
                        
                        CommandListItem {
                            title: qsTr("Help Query") + Retranslate.onLanguageChanged
                            imageSource: "images/ic_help.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.Location
                        
                        CommandListItem {
                            title: qsTr("Location Query") + Retranslate.onLanguageChanged
                            imageSource: "images/ic_location.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.Microphone
                        
                        CommandListItem {
                            title: qsTr("Audio Record") + Retranslate.onLanguageChanged
                            imageSource: "images/ic_microphone.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.UnreadSMS
                        
                        CommandListItem {
                            title: qsTr("Unread SMS Query") + Retranslate.onLanguageChanged
                            imageSource: "images/ic_unread.png"
                        }
                    }
                ]
                
                function itemType(data, indexPath) {
                    return ""+data.command;
                }
                
                dataModel: ArrayDataModel {
                    id: adm
                }
                
                function onDataLoaded(id, data)
                {
                    console.log("ON DATA LOADED", id, data.length);
                    if (id == QueryId.FetchLogs)
                    {
                        console.log("ADDING");
                        adm.clear();
                        adm.append(data);
                    } else if (id == QueryId.FetchLatestLogs) {
                        adm.append(data);
                    }
                }
                
                onCreationCompleted: {
                    sql.dataLoaded.connect(onDataLoaded);
                }
                
                attachedObjects: [
                    LocaleUtil {
                        id: localeUtil
                    }
                ]
            }
        }
    }
}