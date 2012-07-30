// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    property int oldHeight
    id: panel
//    x: 30
//    y: 0
    width: settings.rootWidth
    height: settings.panelMinimalHeight
    color: "blue"
    state: "minimal"
    clip: true
    MouseArea{
        id: mouseArea
        anchors.fill: panel
        hoverEnabled: true
        onPressed: {
            panel.oldHeight = panel.height
            panel.state = "resize";
        }
        onReleased: {
            if (mouseY > panel.oldHeight) {
                if (mouseY < settings.panelNormalHeight) {
                panel.state = "normal";
                }
                else {
                    panel.state = "maximal"
                }
            }
            if (mouseY < panel.oldHeight) {
                panel.state = "minimal"
            }
        }
    }

    Connections {
       target: toppanel
       onChangedFocus: {
           if(panel.state != "resize")
               panel.state = "minimal" }
    }


    Settings {
        id: settings
    }

    states: [
        State {
            name: "minimal"
            PropertyChanges{target: panel; height: settings.panelMinimalHeight}
        },
        State {
            name: "normal"
            PropertyChanges{target: panel; color:"red"; height: settings.panelNormalHeight}
            PropertyChanges{target: appsList; visible:true}
        },
        State {
            name: "maximal"
            PropertyChanges{target: panel; color:"red"; height: settings.panelMaximalHeight}
        },
        State {
            name: "resize"
            PropertyChanges{target: panel; color:"green"; height: mouseArea.mouseY}
        }

    ]
    transitions: [
        Transition {
            from: "resize"
            to: "normal"
            NumberAnimation {target: panel; properties: "height"; duration: 500; easing.type: Easing.InOutQuad }
            NumberAnimation {target: appsList; properties: "visible"; duration: 500 }
        },
        Transition {
            from: "resize"
            to: "minimal"
            NumberAnimation { properties: "height"; duration: 500; easing.type: Easing.InOutQuad }
        },
        Transition {
            from: "resize"
            to: "maximal"
            NumberAnimation { properties: "height"; duration: 500; easing.type: Easing.InOutQuad }
        },
        Transition {
            from: "maximal"
            to: "minimal"
            NumberAnimation { properties: "height"; duration: 500; easing.type: Easing.InOutQuad }
        },
        Transition {
            from: "normal"
            to: "minimal"
            NumberAnimation { properties: "height"; duration: 500; easing.type: Easing.InOutQuad }
        }
    ]

    ListModel {
         id: appsMenu
         objectName: "appsMenu"

         ListElement {
             type: "programm"
             icon: "images/icons/application_x_executable.png"
             name: "test"
         }
         ListElement {
             type: "programm"
             icon: "images/icons/application_x_executable.png"
             name: "test1"
         }
     }



    ListView {

        Component {
            id: appsMenuDelegate
            Rectangle {
                id: shortcut
//                color: ListView.isCurrentItem ? "yellow" : "blue"
                radius: 5
                Image {
                    id: srcIcon
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    sourceSize.width: 100
                    sourceSize.height: 100
                    source: icon
                }
                Text {
                    id: appName
                    text: name
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.bold: true
                    font.pixelSize: 16
                }
                height: srcIcon.height+appName.height
                width: srcIcon.width
                MouseArea{
                    id: mouseArea1
                    anchors.fill: shortcut
                    hoverEnabled: true
                    onDoubleClicked: {
                        panel.state = "minimal"
                    }
                }
                states: [
                    State {
                        name: "mouse"
                        when: mouseArea1.containsMouse == true
                        PropertyChanges{target: shortcut; color: settings.tabContainsMouseColor}
                    }
                ]
            }
        }

        id: appsList
        objectName: "appsList"
//        visible: false
        x: settings.tabWidth-settings.tabRadius
        y: 30
        height: 120
        width: panel.width-2*x
        model: appsMenu
        delegate: appsMenuDelegate
        clip: true
        orientation: ListView.Horizontal

    }

}
