// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Rectangle {
    property string picture
    property string text
    property string path
    property bool isterminal
    id: shortcut
    radius: 5
    clip: true
    Image {
        id: srcIcon
        fillMode: Image.PreserveAspectFit
        smooth: true
        height: 96
        width: 96
        sourceSize.width: 96
        sourceSize.height: 96
        source: shortcut.picture
    }
    Text {
        id: appName
        text: shortcut.text
        anchors.bottom: parent.bottom

        font.pixelSize: 12
    }
    height: srcIcon.height+appName.height
    width: srcIcon.width
    MouseArea{
        id: mouseArea1
        anchors.fill: shortcut
        hoverEnabled: true
        onEntered: {
            if (shortcut.width < appName.width) {
                playstr.start()
            }
        }

        onExited: {
            if (shortcut.width < appName.width) {
                playstr.stop()
            }
        }

        onClicked: {
            if (panel.state == "normal" ) {
                if(appsList.model != categoriesMenu) {
                    backButton.visible = false
                    if (shortcut.isterminal){
                        toppanel.runProcess("xterm -e '"+shortcut.path+"'")

                    }
                    else {
                        toppanel.runProcess(shortcut.path)
                    }
                    panel.state = "minimal"
                    return
                }
                appsList.x = settings.tabWidth-settings.tabRadius + shortcut.width
                appsList.model = categoriesMenu.get(modelIndex).myModel
                backButton.x = settings.tabWidth-settings.tabRadius
                backButton.y = appsList.y
                backButton.visible = true
            }
            if (panel.state == "maximal") {
                if (shortcut.terminal){
                    toppanel.runProcess("xterm -e '"+shortcut.path+"'")

                }
                else {
                    toppanel.runProcess(shortcut.path)
                }
                panel.state = "minimal"
            }
        }
    }
    states: [
        State {
            name: "mouse"
            when: mouseArea1.containsMouse == true
            PropertyChanges{target: shortcut; color: settings.tabContainsMouseColor}
        }
    ]
    SequentialAnimation {
             id: playstr
             running: false

             NumberAnimation { target: appName
                                 properties: "x";from: 0; to: shortcut.width-appName.width ; duration: 2000; }
             NumberAnimation { target: appName
                                 properties: "x";from: shortcut.width-appName.width; to: 0 ; duration: 2000;  }
             loops: Animation.Infinite
             alwaysRunToEnd: true
         }
}
