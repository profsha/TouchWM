// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    property int oldHeight

    property int panelMinimalHeight: settings.panelMinimalHeight
    property int panelNormalHeight: settings.panelNormalHeight
    property int panelMaximalHeight: 0
    id: panel
    height: panel.panelMinimalHeight
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
                if (mouseY < panel.panelNormalHeight) {
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

    onStateChanged: {
        if(panel.state == "minimal" && appsList.model != categoriesMenu) {
            appsList.x = settings.tabWidth-settings.tabRadius
            appsList.model = categoriesMenu
        }
    }

    Connections {
       target: toppanel
       onChangedFocus: {
           if(panel.state != "resize")
               panel.state = "minimal" }

       onClearCategories: {
           for(var i =0; i<categoriesMenu.count; i++) {
               categoriesMenu.get(i).myModel.destroy();
           }
           categoriesMenu.clear()
       }

       onCountCategories: {
           var newObject = Qt.createQmlObject('import QtQuick 1.0; ListModel { id: appsMenu' + count +' ; objectName: "appsMenu'+ count +'";}',
                panel, "dynamicSnippet"+ count);
           categoriesMenu.append({"myModel" : newObject, "name" : text, "icon" : picture, "modelIndex" : count, "exec" : "", "terminal": false});
       }

       onAddApp: {
           categoriesMenu.get(categorie).myModel.append({"icon" : icon, "exec" : exec, "name" : name, "terminal": terminal})
       }

    }


    Settings {
        id: settings
    }

    states: [
        State {
            name: "minimal"
            PropertyChanges{target: panel; height: panel.panelMinimalHeight}
        },
        State {
            name: "normal"
            PropertyChanges{target: panel; color:"red"; height: panel.panelNormalHeight}

        },
        State {
            name: "maximal"
            PropertyChanges{target: panel; color:"red"; height: panel.panelMaximalHeight}
            PropertyChanges{target: appsList; model: categoriesMenu;
                delegate: appsMaxMenuDelegate; height: panelMaximalHeight - 60;
                orientation: ListView.Vertical; x: settings.tabWidth-settings.tabRadius + appsMenuDelegate.width}
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
        id: categoriesMenu
        objectName: "categoriesMenu"

    }

    ListModel {
        id: appsMaxMenu
        objectName: "appsMaxMenu"
    }

    Rectangle {
        property string picture
        property string text
        id: backButton
        visible: false
        radius: 5
        Image {
            id: srcIcon
            fillMode: Image.PreserveAspectFit
            smooth: true
            height: 96
            width: 96
            sourceSize.width: 96
            sourceSize.height: 96
            source: "images/icons/application_x_executable.png"
        }
        Text {
            id: appName
            text: "back"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            font.bold: true
            font.pixelSize: 12
        }
        height: srcIcon.height+appName.height
        width: srcIcon.width
        MouseArea{
            id: mouseArea2
            anchors.fill: backButton
            hoverEnabled: true
            onClicked: {
                if (panel.state == "normal" ) {
                    if(appsList.model != categoriesMenu) {
                        appsList.x = settings.tabWidth-settings.tabRadius
                        appsList.model = categoriesMenu
                        return
                    }
                }

            }
        }
        states: [
            State {
                name: "mouse"
                when: mouseArea2.containsMouse == true
                PropertyChanges{target: backButton; color: settings.tabContainsMouseColor}
            }
        ]
    }

    Component {
        id: appsMenuDelegate
        Shortcut {
            picture: icon
            text: name
            isterminal: terminal
            path: exec
        }
    }

    Component {
        id: appsMaxMenuDelegate
        Row {
            Shortcut{
                picture: icon
                text: name

            }

            ListView {
                id: appsMList
                objectName: "appsList"
                height: 120
                width: panel.width-2*x
                model: myModel
                delegate: appsMenuDelegate
                clip: true
                orientation: ListView.Horizontal

            }
        }


    }

    ListView {



        id: appsList
        objectName: "appsList"
        x: settings.tabWidth-settings.tabRadius
        y: 30
        height: 120
        width: panel.width-2*x
        model: categoriesMenu
        delegate: appsMenuDelegate
        clip: true
        orientation: ListView.Horizontal

    }

}
