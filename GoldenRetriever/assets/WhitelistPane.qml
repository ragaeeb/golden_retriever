import bb.cascades 1.0
import bb.system 1.0

NavigationPane
{
    id: navigationPane
    
    onPopTransitionEnded: {
        page.destroy();
    }
    
    Page
    {
        titleBar: TitleBar {
            title: qsTr("Whitelist") + Retranslate.onLanguageChanged
        }
        
        actions: [
            ActionItem {
                title: qsTr("Add Email") + Retranslate.onLanguageChanged
                imageSource: "images/ic_add_email.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                
                onTriggered: {
                    prompt.show();
                }
                
                attachedObjects: [
                    SystemPrompt {
                        id: prompt
                        title: qsTr("Email Address") + Retranslate.onLanguageChanged
                        body: qsTr("Enter the email address to whitelist:") + Retranslate.onLanguageChanged
                        confirmButton.label: qsTr("OK") + Retranslate.onLanguageChanged
                        cancelButton.label: qsTr("Cancel") + Retranslate.onLanguageChanged
                        
                        onFinished: {
                            if (result == SystemUiResult.ConfirmButtonSelection)
                            {
                                var value = prompt.inputFieldTextEntry();
                                app.addToWhiteList(value);
                                adm.append(value);
                            }
                        }
                    }
                ]
            },
            
            DeleteActionItem {
                id: clearAllAction
                title: qsTr("Clear List") + Retranslate.onLanguageChanged
                imageSource: "images/ic_clear_whitelist.png"
                
                onTriggered: {
                    delPrompt.show()
                }
                
                attachedObjects: [
                    SystemDialog {
                        id: delPrompt
                        title: qsTr("Confirm") + Retranslate.onLanguageChanged
                        body: qsTr("Are you sure you want to clear the whitelist?") + Retranslate.onLanguageChanged
                        confirmButton.label: qsTr("Yes") + Retranslate.onLanguageChanged
                        cancelButton.label: qsTr("No") + Retranslate.onLanguageChanged
                        
                        onFinished: {
                            if (result == SystemUiResult.ConfirmButtonSelection) {
                                app.clearWhiteList();
                                adm.clear();
                            }
                        }
                    }
                ]
            }
        ]
        
        Container
        {
            leftPadding: 10; rightPadding: 10; topPadding: 10
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            Label {
                id: instruction
                multiline: true
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.textAlign: TextAlign.Center
                textStyle.fontSize: FontSize.XSmall
                text: app.whiteListCount > 0 ? qsTr("Warning: The app will respond to anyone who issues a command in the active mailbox. In order to restrict this, choose 'Add Email'.") : qsTr("You can restrict who is able to send commands to your device. There are currently no emails in your whitelist. To only accept commands from specific email addresses, click on the Add Contact action below and add them.")
            }
            
            Divider {
                id: divider
                topMargin: 0; bottomMargin: 0
            }
            
            ListView
            {
                id: listView
                
                dataModel: ArrayDataModel {
                    id: adm
                }
                
                function performDelete(indexPath)
                {
                    var email = adm.data(indexPath);
                    adm.removeAt( indexPath[0] );
                    var undoClicked = persist.showBlockingToast( qsTr("Removed %1 from white list!").arg(email), qsTr("Undo") );
                    
                    if (undoClicked) {
                        dataModel.insert(indexPath[0], email);
                    } else {
                        app.removeFromWhiteList(email);
                    }
                }
                
                listItemComponents: [
                    ListItemComponent {
                        StandardListItem {
                            id: rootItem
                            title: ListItemData
                            imageSource: "images/ic_users.png";
                            
                            contextActions: [
                                ActionSet {
                                    title: rootItem.title
                                    subtitle: rootItem.description
                                    
                                    DeleteActionItem
                                    {
                                        imageSource: "images/ic_delete_contact.png"
                                        
                                        onTriggered: {
                                            rootItem.ListItem.view.performDelete(rootItem.ListItem.indexPath);
                                        }
                                    }
                                }
                            ]
                        }
                    }
                ]
                
                onCreationCompleted: {
                    adm.append( app.getWhiteList() );
                }
            }
        }
    }

}