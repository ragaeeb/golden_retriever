import bb.cascades 1.0
import bb.system 1.0

Page
{
    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
    
    titleBar: TitleBar {
        title: qsTr("Settings") + Retranslate.onLanguageChanged
    }
    
    actions: [
        ActionItem
        {
            title: qsTr("Change Password") + Retranslate.onLanguageChanged
            imageSource: "images/ic_password.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            
            onTriggered: {
                var passwordSheet = definition.createObject();
                passwordSheet.open();
            }
            
            attachedObjects: [
                ComponentDefinition {
                    id: definition
                    source: "SignupSheet.qml"
                }
            ]
        }
    ]
    
    Container
    {
        leftPadding: 10; rightPadding: 10; topPadding: 10
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        Container
        {
            horizontalAlignment: HorizontalAlignment.Fill
            
            Label {
                text: qsTr("Subject") + Retranslate.onLanguageChanged
                verticalAlignment: VerticalAlignment.Center
                textStyle.fontSize: FontSize.Medium
                
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
            }
            
            TextField
            {
                id: subjectField
                text: persist.getValueFor("subject");
                input.flags: TextInputFlag.AutoCapitalizationOff | TextInputFlag.WordSubstitutionOff | TextInputFlag.PredictionOff
                hintText: qsTr("Subject Password (ie: golden)") + Retranslate.onLanguageChanged
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                
                validator: Validator
                {
                    mode: ValidationMode.Immediate
                    errorMessage: qsTr("Subject password cannot be empty and cannot contain a space") + Retranslate.onLanguageChanged
                    
                    onValidate: { 
                        valid = subjectField.text.length > 0 && subjectField.text.indexOf(" ") == -1;
                    }
                }
                
                onTextChanged: {
                    subjectField.text = subjectField.text.trim().toLowerCase();
                    validator.validate();
                    
                    if (validator.valid) {
                        persist.saveValueFor("subject", subjectField.text);
                    }
                }
            }
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
        }
        
        PersistCheckBox
        {
            topMargin: 20
            text: qsTr("Delete Incoming Request") + Retranslate.onLanguageChanged
            key: "delRequest"
        }
        
        PersistCheckBox
        {
            topMargin: 20
            text: qsTr("Delete Outgoing Response") + Retranslate.onLanguageChanged
            key: "delResponse"
        }
    }
    
    function onPushed(page)
    {
        navigationPane.pushTransitionEnded.disconnect(onPushed);
        
        if ( tutorialToast.tutorial("tutorialSubject", qsTr("The subject password is the keyword what the subject of all your emails must start with. So for example if your subject password is '%1', then in order to issue a battery command, the subject of the email must be '%1 battery'.").arg(app.subjectPrefix), "images/ic_help.png" ) ) {}
        else if ( tutorialToast.tutorial("tutorialDelResponse", qsTr("Enable the 'Delete Outgoing Response' option to remove the app's reply from your sent box."), "images/ic_help.png" ) ) {}
        else if ( tutorialToast.tutorial("tutorialDelRequest", qsTr("Enable the 'Delete Incoming Request' option to remove your incoming command from your inbox."), "images/ic_help.png" ) ) {}
        else if ( tutorialToast.tutorial("tutorialChangePassword", qsTr("To change the password that you are prompted when you open the app, simply tap on the 'Change Password' option in the menu."), "images/ic_password.png" ) ) {}
        
        reporter.initPage(rootPage);
    }
    
    onCreationCompleted: {
        navigationPane.pushTransitionEnded.connect(onPushed);
    }
}