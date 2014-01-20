import bb.cascades 1.0
import bb.device 1.0

Dialog
{
    id: root
    
    Container
    {
        preferredWidth: displayInfo.pixelSize.width
        preferredHeight: displayInfo.pixelSize.height
        background: Color.create(0.0, 0.0, 0.0, 0.5)
        layout: DockLayout {}
        leftPadding: 10;rightPadding: 10
        
        TextField {
            id: passwordField
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            inputMode: TextFieldInputMode.Password
            input.submitKey: SubmitKey.Submit
            
            input.onSubmitted: {
                passwordValidator.validate();
            }
            
            validator: Validator
            {
                id: passwordValidator
                mode: ValidationMode.Custom
                errorMessage: qsTr("Invalid login credentials provided.") + Retranslate.onLanguageChanged
                
                onValidate: {
                    valid = security.login(passwordField.text);
                    
                    if (valid) {
                        root.close();
                    }
                }
            }
        }
        
        attachedObjects: [
            DisplayInfo {
                id: displayInfo
            }
        ]
    }
    
    onOpened: {
        passwordField.requestFocus();
    }
}