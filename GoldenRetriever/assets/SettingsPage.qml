import bb.cascades 1.0
import bb.system 1.0
import com.canadainc.data 1.0

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
            }
            
            TextField {
                text: persist.getValueFor("subject")
            }
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
        }
    }
}