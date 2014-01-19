import bb.cascades 1.0
import bb.system 1.0
import com.canadainc.data 1.0

Page
{
    titleBar: TitleBar {
        title: qsTr("Settings") + Retranslate.onLanguageChanged
    }
    
    actions: [
        ActionItem {
            title: qsTr("Add Contact") + Retranslate.onLanguageChanged
            imageSource: "images/ic_add_contact.png"
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
                        if (result == SystemUiResult.ConfirmButtonSelection) {
                            var request = prompt.inputFieldTextEntry();
                            request = request.toLowerCase();
                            
                            var firstAtSign = request.indexOf("@");
                            var lastAtSign = request.lastIndexOf("@");
                            var dotExists = request.indexOf(".") > 0;
                            
                            if ( firstAtSign >= 0 && lastAtSign >= 0 && firstAtSign == lastAtSign && dotExists ) {
                                var contacts = persist.getValueFor("whitelist");
                                
                                if (!contacts) {
                                    contacts = [];
                                }
                                
                                if ( contacts.indexOf(request) == -1 ) {
                                    contacts.push(request);
                                    adm.append(request);
                                    persist.saveValueFor("whitelist", contacts);
                                } else {
                                    persist.showToast("Duplicate entry!");
                                }
                            } else {
                                persist.showToast("Invalid email address entered...");
                            }
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
            text: qsTr("You can restrict who is able to send commands to your device. There are currently no emails in your whitelist. To only accept commands from specific email addresses, click on the Add Contact action below and add them.")
        }
        
        Divider {
            id: divider
            topMargin: 0; bottomMargin: 0
            visible: instruction.visible
        }
        
        ListView
        {
            id: listView
            
            dataModel: ArrayDataModel {
                id: adm
            }
            
            function performDelete(indexPath)
            {
                var whitelist = persist.getValueFor("whitelist");
                
                if (whitelist)
                {
                    whitelist.splice(indexPath[0], 1);
                    adm.removeAt( indexPath[0] );
                    
                    if (whitelist.length > 0) {
                        persist.saveValueFor("whitelist", whitelist);
                    } else {
                        persist.remove("whitelist");
                        instruction.visible = true;
                    }
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
            
            onCreationCompleted:
            {
                var whitelist = persist.getValueFor("whitelist");
                
                if (whitelist) {
                    instruction.visible = false;
                    adm.append(whitelist);
                } else {
                    instruction.visible = true;
                }
            }
        }
    }
}