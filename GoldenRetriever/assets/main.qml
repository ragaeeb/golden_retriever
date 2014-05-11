import bb.cascades 1.2
import bb.system 1.0

TabbedPane
{
    id: root
    showTabsOnActionBar: true
    
    Menu.definition: CanadaIncMenu
    {
        projectName: "golden-retriever"
        bbWorldID: "23334872"
        showServiceLogging: true
        showSubmitLogs: true
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
        if ( !persist.tutorial("subjectMoveTutorial", qsTr("IMPORTANT: In the new version of Golden Retriever, the commands are specified in the SUBJECT line after the '%1' keyword instead of being specified in the message body. For example before if you used to specify the '%1' keyword in the subject line and 'battery' in the body, you would now just specify '%1 battery' in the subject line only (without quotes).\n\nThis was done to preserve battery life since downloading the message body every time drains your device's battery. If you have any questions, please watch the tutorial video or send us an email!").arg(app.subjectPrefix), "asset:///images/ic_help.png" ) ) {}
        else if ( persist.tutorialVideo("http://youtu.be/KA56n786BOo") ) {}
        
        logTab.delegateActivationPolicy = TabDelegateActivationPolicy.ActivateWhenSelected;
        
        if (app.accountSelected) {
            activeTab = logTab;
        } else {
            activeTab = accounts;
        }
    }
    
    function authenticate()
    {
        if ( !security.accountCreated() ) {
            definition.source = "SignupSheet.qml";
        } else {
            definition.source = "LoginDialog.qml";
        }
        
        var sheet = definition.createObject();
        sheet.closed.connect(onSheetClosed);
        sheet.open();
    }
    
    onCreationCompleted: {
        app.initialize.connect(authenticate);
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: definition
        }
    ]
}