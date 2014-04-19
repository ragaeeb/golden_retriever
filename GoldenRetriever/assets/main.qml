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
        promoteChannel: true
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
        if ( !persist.contains("subjectMoveTutorial") ) {
            persist.showBlockingToast( qsTr("IMPORTANT: In the new version of Golden Retriever, the commands are specified in the SUBJECT line after the 'golden' keyword instead of being specified in the message body. For example before if you used to specify the 'golden' keyword in the subject line and 'battery' in the body, you would now just specify 'golden battery' in the subject line only (without quotes).\n\nThis was done to preserve battery life since downloading the message body every time drains your device's battery. If you have any questions, please watch the tutorial video or send us an email!"), qsTr("OK"), "asset:///images/ic_help.png" );
            persist.saveValueFor("subjectMoveTutorial", 1);
        }
        
        if ( !persist.contains("tutorialVideo") ) {
            var yesClicked = persist.showBlockingDialog( qsTr("Tutorial"), qsTr("Would you like to see a video tutorial on how to use the app?"), qsTr("Yes"), qsTr("No") );
            
            if (yesClicked) {
                vidTutorial.trigger("bb.action.OPEN");
            }
            
            persist.saveValueFor("tutorialVideo", 1);
        }
        
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
        //sheet.open();
        onSheetClosed();
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: definition
        },
        
        Invocation {
            id: vidTutorial
            
            query: InvokeQuery {
                mimeType: "text/html"
                uri: "https://www.youtube.com/watch?v=8hDCBJosXGQ"
            }
        }
    ]
}