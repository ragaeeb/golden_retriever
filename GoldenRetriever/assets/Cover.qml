import bb.cascades 1.0

Container
{
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    background: Color.Black

    layout: DockLayout {}
    
    ImageView {
        imageSource: "images/logo.png"
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
    }
    
    Label {
        text: qsTr("Monitoring...") + Retranslate.onLanguageChanged
        verticalAlignment: VerticalAlignment.Top
        horizontalAlignment: HorizontalAlignment.Fill
        textStyle.textAlign: TextAlign.Center
        textStyle.color: Color.White
        opacity: 0.5
    }
}