

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Templates as T
import QtQuick.Controls.Material.impl 2.15
import Qt5Compat.GraphicalEffects
import ACross

T.Button {
    id: button
    implicitWidth: 56
    implicitHeight: 56
    property bool running: false
    onClicked: running = !running

    background: Rectangle {
        id: rectangle
        anchors.fill: parent
        color: Colors.primary
        radius: button.width / 2
        Image {
            width: button.width / 2
            height: button.height / 2
            anchors.verticalCenter: parent.verticalCenter
            source: running ? "../misc/icons/dark/stop.svg" : "../misc/icons/dark/play.svg"
            sourceSize.height: 24
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectFit
            sourceSize.width: 24
        }

        HoverHandler {
            acceptedDevices: PointerDevice.Mouse
            cursorShape: Qt.PointingHandCursor
        }
        Ripple {
            id: ripple
            clip: true
            clipRadius: parent.radius
            width: parent.width
            height: parent.height
            pressed: button.pressed
            anchor: button
            active: button.enabled && (button.down || button.visualFocus
                                       || button.hovered)
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: ripple.width
                    height: ripple.height
                    radius: rectangle.radius
                }
            }
        }
    }
    states: [
        State {
            name: "running"
            when: running
            PropertyChanges {
                target: ripple
                color: Material.highlightedRippleColor
            }
            PropertyChanges {
                target: rectangle
                color: Colors.fabColor
            }
        },
        State {
            name: "stopped"
            when: !running
            PropertyChanges {
                target: ripple
                color: Material.rippleColor
            }
            PropertyChanges {
                target: rectangle
                color: Colors.primary
            }
        }
    ]
}
