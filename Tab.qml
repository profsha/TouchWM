// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {

    property string tabText: "tab"
    property string position
    property int tabIndex: 0

    id: tab
    color: settings.tabColor
    width: settings.tabWidth
    height: settings.tabHeight
    radius: settings.tabRadius
    clip: true


    signal tabClick(int index)

    onTabClick: {
        if (index != root.currentTab) {
            window.chooseClient(index)
            root.currentTab = index
        }
        else {
            window.turnClient(index)
            root.currentTab = -1
        }

        console.debug(index.toString() +" clicked " + root.currentTab.toString())
    }


    MouseArea{
        id: mouseArea
        anchors.fill: tab
        onClicked: tabClick(tabIndex)
        hoverEnabled: true
    }

    Text{
        id: tabString
        rotation: 90
        anchors.horizontalCenter: tab.horizontalCenter
        text:tabText
        y: tab.y + width/2

    }

    onPositionChanged: {
        if (position == "right") {
            tab.rotation = 180
        }
        else {
            tab.rotation = 0
        }
    }

    states: [
        State {
            name: "pressed"
            when: mouseArea.pressed == true
            PropertyChanges{target: tab; color: settings.tabPressedColor}
        },
        State {
            name: "mouse"
            when: mouseArea.containsMouse == true
            PropertyChanges{target: tab; color: settings.tabContainsMouseColor}
        },
        State {
            name: "currentTab"
            when: root.currentTab == tabIndex
            PropertyChanges{target: tab; color: settings.tabCurrentColor}
        }
    ]
}
