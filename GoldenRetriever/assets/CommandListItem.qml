import bb.cascades 1.0

StandardListItem
{
    id: sli
    description: ListItemData.reply
    status: ListItem.view.localizer.renderStandardTime( new Date(ListItemData.timestamp) )
    
    onCreationCompleted: {
        inflateAnim.play();
    }
    
    animations: [
        ParallelAnimation
        {
            id: inflateAnim
            
            ScaleTransition
            {
                fromX: 0.8
                toX: 1
                fromY: 0.8
                toY: 1
                duration: 800
                easingCurve: StockCurve.ElasticOut
            }
            
            FadeTransition {
                fromOpacity: 0
                toOpacity: 1
                duration: 200
            }
            
            delay: sli.ListItem.indexInSection * 100
            
            onCreationCompleted: {
                play();
            }
        }
    ]
}