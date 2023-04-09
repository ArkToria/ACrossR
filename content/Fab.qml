

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Studio.Effects 1.0
import QtQuick.Controls.Material.impl 2.15
import Qt5Compat.GraphicalEffects

Rectangle {
    id: rectangle
    implicitWidth: 56
    implicitHeight: 56
    color: "#633b48"
    radius: 16
    Image {
        width: 24
        height: 24
        anchors.verticalCenter: parent.verticalCenter
        source: "../misc/icons/dark/add.svg"
        sourceSize.height: 24
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.PreserveAspectFit
        sourceSize.width: 24
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
    }
    Ripple {
        id: ripple
        anchors.fill: parent
        anchor: rectangle
        clipRadius: 4
        pressed: mouseArea.pressed
        active: mouseArea.containsPress
        layer.enabled: true
        color: "#20FFFFFF"
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: ripple.width
                height: ripple.height
                radius: rectangle.radius
            }
        }
    }
}
