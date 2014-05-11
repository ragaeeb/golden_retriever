import bb.cascades 1.0
import bb.system 1.0

Page
{
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
                
                onCreationCompleted: {
                    if ( !persist.tutorial("subjectTutorial", qsTr("The subject password is the keyword what the subject of all your emails must start with. So for example if your subject password is '%1', then in order to issue a battery command, the subject of the email must be '%1 battery'.").arg(app.subjectPrefix), "asset:///images/ic_help.png" ) ) {}
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
}