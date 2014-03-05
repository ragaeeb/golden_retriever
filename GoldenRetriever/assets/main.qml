import bb.cascades 1.2
import bb.system 1.0

TabbedPane
{
    id: root
    showTabsOnActionBar: true
    
    Menu.definition: CanadaIncMenu {
        projectName: "golden-retriever"
        bbWorldID: "23334872"
        promoteChannel: true
    }
    
    Tab
    {
        id: logTab
        title: qsTr("Logs") + Retranslate.onLanguageChanged
        description: qsTr("Processed commands") + Retranslate.onLanguageChanged
        imageSource: "images/ic_logs.png"
        delegateActivationPolicy: TabDelegateActivationPolicy.None
        
        delegate: Delegate {
            source: "CommandLogPane.qml"
        }
        
        function onDataLoaded(id, data)
        {
            if (id == QueryId.FetchLogs) {
                unreadContentCount = data.length;
            } else if (id == QueryId.FetchLatestLogs) {
                unreadContentCount = unreadContentCount + data.length;
            }
        }
        
        onCreationCompleted: {
            sql.dataLoaded.connect(onDataLoaded);
        }
    }
    
    Tab {
        id: accounts
        title: qsTr("Account") + Retranslate.onLanguageChanged
        description: qsTr("Monitored Mailbox") + Retranslate.onLanguageChanged
        imageSource: "images/ic_account.png"
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateWhenSelected
        newContentAvailable: !app.accountSelected
        
        delegate: Delegate {
            source: "AccountsPane.qml"
        }
    }
    
    Tab {
        id: whitelist
        title: qsTr("Whitelist") + Retranslate.onLanguageChanged
        description: qsTr("Allowed senders") + Retranslate.onLanguageChanged
        imageSource: "images/ic_whitelist.png"
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateWhenSelected
        unreadContentCount: app.whiteListCount
        
        delegate: Delegate {
            source: "WhitelistPane.qml"
        }
    }
    
    function onSheetClosed()
    {
        logTab.delegateActivationPolicy = TabDelegateActivationPolicy.ActivateWhenSelected;
        
        if (app.accountSelected) {
            activeTab = logTab;
        } else {
            activeTab = accounts;
        }
    }
    
    onCreationCompleted: {
        if ( !security.accountCreated() ) {
            definition.source = "SignupSheet.qml";
        } else {
            definition.source = "LoginDialog.qml";
        }
        
        var sheet = definition.createObject();
        sheet.closed.connect(onSheetClosed);
        sheet.open();
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: definition
        }
    ]
}