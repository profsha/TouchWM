// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

QtObject {
    property int tabHeight: 100
    property int tabWidth: 40
    property int tabRadius: 10
    property color tabColor: "grey"
    property color tabPressedColor: "orange"
    property color tabContainsMouseColor: "yellow"
    property color tabCurrentColor: "red"
    property int rootHeight: 0
    property int rootWidth: 0
    property string workflowWallpaper: "images/redplanet.png"
    property int panelMinimalHeight: 30
    property int panelNormalHeight: 188
    property int panelMaximalHeight: 800
}
