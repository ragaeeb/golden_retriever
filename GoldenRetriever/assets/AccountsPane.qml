import bb.cascades 1.0

NavigationPane
{
    id: navigationPane
    
    onPopTransitionEnded: {
        page.destroy();
    }
    
    Page
    {
        actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
        
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
                console.log("UserEvent: AccountSelected");
                
                clearSelection();
                toggleSelection(indexPath);
                
                var selectedValue = adm.data(indexPath).accountId;
                persist.saveValueFor("account", selectedValue);
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
                
                if (results.length == 0) {
                    tutorial.init( qsTr("No mailboxes detected. It seems like you may not have given the app the appropriate permissions..."), "images/ic_account.png" );
                } else {
                    tutorial.init( qsTr("Choose the mailbox to monitor for the commands.\n\nWhen you are away from your device, send the command messages to this mailbox so your device can process them."), "images/ic_account.png" );
                }
            }
            
            onCreationCompleted: {
                app.accountsImported.connect(onAccountsImported);
                app.loadAccounts();
            }
        }
    }
}