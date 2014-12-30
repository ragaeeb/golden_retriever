import bb.cascades 1.0
import bb.system 1.2
import com.canadainc.data 1.0

NavigationPane
{
    id: navigationPane

    onPopTransitionEnded: {
        page.destroy();
    }
    
    Page
    {
        actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
        
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
                        inputField.emptyText: qsTr("Enter the command (ie: %1 battery)").arg(app.subjectPrefix) + Retranslate.onLanguageChanged
                        inputField.defaultText: app.subjectPrefix+" "
                        inputOptions: SystemUiInputOption.None
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
                    console.log("UserEvent: ClearLogs");
                    var ok = persist.showBlockingDialog( qsTr("Confirmation"), qsTr("Are you sure you want to clear all the logs?") );
                    console.log("UserEvent: ClearLogsConfirm", ok);
                    
                    if (ok) {
                        sql.query = "DELETE FROM logs";
                        sql.load(QueryId.ClearLogs);
                        tutorial.init( qsTr("Cleared all logs!"), "images/menu/ic_clear_logs.png" );
                    }
                }
            }
        ]
        
        Container
        {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            layout: DockLayout {}
            
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
                        type: ""+Command.CallLogs
                        
                        CommandListItem {
                            title: qsTr("Calls") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_call_log.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.ChangeProfile
                        
                        CommandListItem {
                            title: qsTr("Change Profile") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_change_profile.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.CommandLine
                        
                        CommandListItem {
                            title: qsTr("Command Line") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_terminal.png"
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
                        type: ""+Command.Flash
                        
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
                            title: qsTr("Record Audio") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_microphone.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.NetworkInfo
                        
                        CommandListItem {
                            title: qsTr("Network Info") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_network.png"
                        }
                    },
                    
                    ListItemComponent {
                        type: ""+Command.SimCardInfo
                        
                        CommandListItem {
                            title: qsTr("SIM Card Info") + Retranslate.onLanguageChanged
                            imageSource: "images/commands/ic_sim.png"
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

                    if ( !app.checkDatabase() )
                    {
                        definition.source = "SetupDialog.qml";
                        var setup = definition.createObject();
                        setup.open();
                    }
                }
                
                attachedObjects: [
                    LocaleUtil {
                        id: localeUtil
                    },
                    
                    ComponentDefinition {
                        id: definition
                    }
                ]
            }
            
            PermissionToast
            {
                id: tm
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                labelColor: Color.Black
                
                function onReady(allMessages, allIcons)
                {
                    if (allMessages.length > 0)
                    {
                        messages = allMessages;
                        icons = allIcons;
                        delegateActive = true;
                    } else {
                        if ( tutorialToast.tutorial( "tutorialSettings", qsTr("To customize settings in the app, swipe-down from the top-bezel and choose 'Settings'."), "images/menu/ic_settings.png" ) ) {}
                        else if ( tutorialToast.tutorial( "tutorialHelp", qsTr("To get additional help about the app and the developers, please swipe-down from the top-bezel and choose 'Help'."), "images/menu/ic_help.png" ) ) {}
                        else if ( tutorialToast.tutorial( "tutorialTest", qsTr("To test out a command, simply tap on the 'Test' button on the top-right and you can enter a subject line to simulate how the app would respond to your command and whether the app is functional or not."), "images/menu/ic_test.png" ) ) {}
                        else if ( tutorialToast.tutorial( "tutorialClearLogs", qsTr("If this logs pane gets too long and the app load time is slow, simply tap on the '...' in the overflow menu and choose 'Clear Logs' to start fresh."), "images/menu/ic_clear_logs.png" ) ) {}
                        else if ( persist.reviewed() ) {}
                        else if ( reporter.performCII() ) {}
                    }
                }
                
                onCreationCompleted: {
                    app.permissionsReady.connect(onReady);
                    app.startValidation();
                }
            }
        }
    }
}