

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

T.Button {
    id: button
    implicitWidth: 56
    implicitHeight: 56

    property alias color: rectangle.color

    background: Rectangle {
        id: rectangle
        anchors.fill: parent
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


    /*
Rectangle {
        id: rectangle
        anchors.fill: parent
        color: button.color
        visible: button.hovered
        opacity: 0.25
        radius: rect.radius
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        Ripple {
            clip: true
            clipRadius: parent.radius
            width: parent.width
            height: parent.height
            pressed: button.pressed
            anchor: button
            active: button.enabled && (button.down
                                             || button.visualFocus
                                             || button.hovered)
            color: "#20FFFFFF"
        }
    }
    */
}
