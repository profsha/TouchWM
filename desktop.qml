// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {

    property int currentTab: -1
    property int i: 0
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
           currentTab = index-1
           window.chooseClient(currentTab)
           for (i = 0; i<leftTabs.count; i++) {
               if (leftTabs.get(i).index == index) {
                   leftTabs.remove(i)
                   break
               }
           }
           for (i = 0; i<rightTabs.count; i++) {
               if (rightTabs.get(i).index == index) {
                   rightTabs.remove(i)
                   break
               }
           }

       }
    }

    Rectangle {
        id: workflow
        z: 0
        objectName: "workflow"
//        anchors.horizontalCenter: parent.horizontalCenter
        x: tab.width-tab.radius
        y: settings.panelMinimalHeight
        height: root.height-settings.panelMinimalHeight
        width: root.width-2*settings.tabWidth+2*settings.tabRadius
        Image {
            anchors.fill: workflow
            smooth: true
            source: settings.workflowWallpaper
        }
    }



    Rectangle {
        x: 0
        y: 30
        height: workflow.height
        color: "black"
        width: tab.width-tab.radius

    }
    Rectangle {
        x: parent.width-tab.width+tab.radius
        y: 30
        height: workflow.height
        color: "black"
        width: tab.width-tab.radius

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
        }
    }
    Component {
        id: rightTabsDelegate
        Tab {
            x: parent.width-width+radius
            tabIndex: index
            tabText: name
        }
    }
    ListView {
        id: leftTabsList
        y: 30
        anchors.left: parent.left
        height: workflow.height
        width: tab.width
        model: leftTabs
        delegate: leftTabsDelegate
        snapMode: ListView.SnapToItem
        clip: true
    }
    ListView {
        id: rightTabsList
        y: 30
        height: workflow.height
        width: tab.width
        anchors.right: parent.right
        model: rightTabs
        delegate: rightTabsDelegate
        snapMode: ListView.SnapToItem
        clip: true
    }

    Rectangle {
        x: 40
        y: root.height-30
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
                window.runProcess(text)
                text = ""
            }
        }
    }

}
