import bb.cascades 1.0

Sheet
{
    id: root
    
    onOpened: {
        persist.showToast( qsTr("Choose the mailbox to monitor for the commands.\n\nWhen you are away from your device, send the command messages to this mailbox so your device can process them."), qsTr("OK") );
    }
    
    Page
    {
        titleBar: TitleBar
        {
            title: qsTr("Active Account") + Retranslate.onLanguageChanged
            
            acceptAction: ActionItem {
                id: saveAction
                title: qsTr("Save") + Retranslate.onLanguageChanged
                enabled: false
                
                onTriggered: {
                    var selectedIndex = listView.selected();
                    var selectedValue = adm.data(selectedIndex).accountId;
                    
                    persist.saveValueFor("account", selectedValue);
                    
                    root.close();
                }
            }
            
            dismissAction: ActionItem {
                title: qsTr("Cancel") + Retranslate.onLanguageChanged
                
                onTriggered: {
                    root.close();
                }
            }
        }
        
        ListView
        {
            id: listView
            
            dataModel: ArrayDataModel {
                id: adm
            }
            
            listItemComponents: [
                ListItemComponent
                {
                    StandardListItem {
                        imageSource: "images/ic_account.png"
                        title: ListItemData.name
                        description: ListItemData.address
                    }
                }
            ]
            
            onTriggered: {
                clearSelection();
                toggleSelection(indexPath);
                
                saveAction.enabled = true;
            }
            
            function onAccountsImported(results)
            {
                for (var i = results.length-1; i >= 0; i--)
                {
                    var current = results[i];

                    if (current.accountId != 23 && current.accountId != 199) {
                        adm.append(current);
                    }
                }
                
                var activeAccount = persist.getValueFor("account");
                
                for (var j = adm.size()-1; j >= 0; j--)
                {
                    if ( activeAccount == adm.value(j).accountId )
                    {
                        listView.select([j],true);
                        break;
                    }
                }
            }
            
            onCreationCompleted: {
                app.accountsImported.connect(onAccountsImported);
                app.loadAccounts();
            }
        }
    }
    
    onClosed: {
        destroy();
    }
}