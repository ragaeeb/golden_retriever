import bb.cascades 1.0

FullScreenDialog
{
    id: root
    canClose: false
    
    dialogContent: Container
    {
        leftPadding: 80; rightPadding: 80; topPadding: 100; bottomPadding: 150
        layout: DockLayout {}
        
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            imageSource: "images/login/background.amd"
            opacity: 0
            
            animations: [
                FadeTransition {
                    id: fader
                    fromOpacity: 0
                    toOpacity: 1
                    duration: 500
                }
            ]
        }
        
        Container
        {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            leftPadding: 15; rightPadding: 15;
            translationX: 1000
            
            TextField {
                id: passwordField
                inputMode: TextFieldInputMode.Password
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                input.submitKey: SubmitKey.Submit
                inputRoute.primaryKeyTarget: true
                
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
            
            animations: [
                TranslateTransition {
                    id: translateAnim
                    fromX: 1000
                    toX: 0
                    duration: 250
                    easingCurve: StockCurve.SineIn
                }
            ]
        }
        
        ImageView {
            horizontalAlignment: HorizontalAlignment.Left
            verticalAlignment: VerticalAlignment.Top
            imageSource: "images/login/lock.png"
            translationX: -100
            
            animations: [
                SequentialAnimation
                {
                    id: rotator
                    
                    TranslateTransition {
                        fromX: -100
                        toX: 0
                        duration: 250
                        easingCurve: StockCurve.SineInOut
                    }
                    
                    RotateTransition
                    {
                        fromAngleZ: 0
                        toAngleZ: 360
                        duration: 1000
                        easingCurve: StockCurve.SineOut
                    }
                }
            ]
        }
    }
    
    onOpened: {
        passwordField.requestFocus();
        fader.play();
        rotator.play();
        translateAnim.play();
    }
}