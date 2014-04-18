import bb.cascades 1.0
import bb.system 1.0
import com.canadainc.data 1.0

NavigationPane
{
    id: navigationPane

    onPopTransitionEnded: {
        page.destroy();
    }
    
    Page
    {
        titleBar: TitleBar
        {
            title: qsTr("Golden Retriever") + Retranslate.onLanguageChanged
            
            acceptAction: ActionItem
            {
                title: qsTr("Test") + Retranslate.onLanguageChanged
                imageSource: "images/menu/ic_test.png"
                
                onTriggered: {
                    prompt.show();
                }
                
                attachedObjects: [
                    SystemPrompt {
                        id: prompt
                        title: qsTr("Enter Command") + Retranslate.onLanguageChanged
                        inputField.emptyText: qsTr("Enter the command (ie: battery)") + Retranslate.onLanguageChanged
                        confirmButton.label: qsTr("OK") + Retranslate.onLanguageChanged
                        cancelButton.label: qsTr("Cancel") + Retranslate.onLanguageChanged
                        
                        onFinished: {
                            if (result == SystemUiResult.ConfirmButtonSelection) {
                                app.invokeService( inputFieldTextEntry() );
                            }
                        }
                    }
                ]
            }
        }
        
        actions: [
            DeleteActionItem
            {
                title: qsTr("Clear Logs") + Retranslate.onLanguageChanged
                imageSource: "images/menu/ic_clear_logs.png"
                enabled: listView.visible
                
                onTriggered: {
                    clearPrompt.show();
                }
                
                attachedObjects: [
                    SystemDialog
                    {
                        id: clearPrompt
                        title: qsTr("Confirmation") + Retranslate.onLanguageChanged
                        body: qsTr("Are you sure you want to clear all the logs?") + Retranslate.onLanguageChanged
                        confirmButton.label: qsTr("Yes") + Retranslate.onLanguageChanged
                        cancelButton.label: qsTr("No") + Retranslate.onLanguageChanged
                        
                        onFinished: {
                            if (result == SystemUiResult.ConfirmButtonSelection)
                            {
                                sql.query = "DELETE FROM logs";
                                sql.load(QueryId.ClearLogs);
                                persist.showToast( qsTr("Cleared all logs!"), "", "asset:///images/menu/ic_clear_logs.png" );
                            }
                        }
                    }
                ]
            }
        ]
        
        Container
        {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            EmptyDelegate
            {
                id: emptyDelegate
                graphic: "images/placeholder/empty_logs.png"
                labelText: qsTr("Welcome to Golden Retriever. This app can be used to remotely control your BlackBerry device using emails. To learn more about the commands available, swipe-down from the top-bezel and choose Help.") + Retranslate.onLanguageChanged
            }
            
            ListView
            {
                id: listView
                property alias localizer: localeUtil
                
                listItemComponents: [
                    ListItemComponent {
                        type: ""+Command.Unknown
                        
                        CommandListItem {
                            title: qsTr("Unknown Command") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_unknown.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.Alarm
                        
                        CommandListItem {
                            title: qsTr("Alarm") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_alarm.png"
                        }
                    },

                    ListItemComponent {
                        type: ""+Command.Battery
                        
                        CommandListItem {
                            title: qsTr("Battery Status") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_battery.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.Calendar
                        
                        CommandListItem {
                            title: qsTr("Calendar Query") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_calendar.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.Contact
                        
                        CommandListItem {
                            title: qsTr("Contact Query") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_contact_fetch.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.FileRequest
                        
                        CommandListItem {
                            title: qsTr("File Request") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_file.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.Flashlight
                        
                        CommandListItem {
                            title: qsTr("Flashlight") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_flashlight.png"
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
                            imageSource: "images/commands/ic_location.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.Microphone
                        
                        CommandListItem {
                            title: qsTr("Audio Record") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_microphone.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.Sync
                        
                        CommandListItem {
                            title: qsTr("Sync") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_sync.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.UnreadSMS
                        
                        CommandListItem {
                            title: qsTr("Unread SMS Query") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_unread.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.Volume
                        
                        CommandListItem {
                            title: qsTr("Volume") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_volume.png"
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
                    if (id == QueryId.FetchLogs) {
                        adm.append(data);
                    } else if (id == QueryId.FetchLatestLogs) {
                        adm.insert(0, data);
                    } else if (id == QueryId.ClearLogs) {
                        adm.clear();
                    }
                    
                    listView.visible = !adm.isEmpty();
                    emptyDelegate.delegateActive = adm.isEmpty();
                    
                    listView.scrollToPosition(ScrollPosition.Beginning, ScrollAnimation.Smooth);
                }
                
                onCreationCompleted: {
                    sql.dataLoaded.connect(onDataLoaded);
                    app.checkDatabase();
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