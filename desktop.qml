// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {

    property int currentTab: -1
    property int heightWorkflow: root.height-settings.panelMinimalHeight
    property int widthWorkflow: root.width-2*(widthTab)
    property int yWorkflow: settings.panelMinimalHeight
    property int widthTab: settings.tabWidth-settings.tabRadius
    property string workflowWallpaper: settings.workflowWallpaper
    Settings {
        id: settings
        objectName: "settings"
    }

    id: root
    x: 0
    y: 0
    color: "white"
    anchors.fill: parent

    Tab{
        id: tab
        visible: false
    }

    Connections {
       target: window
       onAddClient: {
           if (index%2 == 0) {
               leftTabs.append({"name" : str, "index" : index})

           }
           else {
               rightTabs.append({"name" : str, "index" : index})
           }
            currentTab = index
           window.chooseClient(currentTab) }
       onCloseClient: {
           for (var i = 0; i<leftTabs.count; i++) {
               if (leftTabs.get(i).index == index) {
                   leftTabs.remove(i)
                   currentTab = index-1
                    window.mapClient(currentTab)
                   return
               }
           }
           for (i = 0; i<rightTabs.count; i++) {
               if (rightTabs.get(i).index == index) {
                   rightTabs.remove(i)
                   currentTab = index-1
                    window.mapClient(currentTab)
                   return
               }
           }

       }
    }

    Rectangle {
        id: workflow
        z: 0
        objectName: "workflow"
        x: root.widthTab
        y: root.yWorkflow
        height: root.heightWorkflow
        width: root.widthWorkflow
        Image {
            anchors.fill: workflow
            smooth: true
            source: root.workflowWallpaper
        }
    }



    Rectangle {
        x: 0
        y: root.yWorkflow
        height: workflow.height
        color: "black"
        width: root.widthTab

    }
    Rectangle {
        x: parent.width-tab.width+tab.radius
        y: root.yWorkflow
        height: workflow.height
        color: "black"
        width: root.widthTab

    }

    ListModel {
         id: leftTabs
         objectName: "leftTabs"
     }
    ListModel {
        id: rightTabs
        objectName: "rightTabs"
    }
    Component {
        id: leftTabsDelegate
        Tab {
            x: -radius
            tabText: name
            tabIndex: index
            position: "left"
        }
    }
    Component {
        id: rightTabsDelegate
        Tab {
            position: "right"
            x: settings.tabRadius
            tabIndex: index
            tabText: name
        }
    }
    ListView {
        id: leftTabsList
        y: root.yWorkflow
        anchors.left: parent.left
        height: workflow.height
        width: settings.tabWidth
        model: leftTabs
        delegate: leftTabsDelegate
        snapMode: ListView.SnapToItem
        clip: true
    }
    ListView {
        id: rightTabsList
        y: root.yWorkflow
        height: workflow.height
        width: settings.tabWidth
        anchors.right: parent.right
        model: rightTabs
        delegate: rightTabsDelegate
        snapMode: ListView.SnapToItem
        clip: true
    }

    Rectangle {
        x: 40
        y: root.height-root.yWorkflow
        height: runner.height
        color: "yellow"
        width: workflow.width-20
        radius: 5
        TextInput {
            id: runner
            objectName: "runner"
            font.pointSize: 14
            anchors.fill: parent
            color: "black"
            text: ""
            selectByMouse : true
            onAccepted: {
                if(text == "exit") {

                }
                else {
                    window.runProcess(text)
                    text = ""
                }


            }
        }
    }

}
