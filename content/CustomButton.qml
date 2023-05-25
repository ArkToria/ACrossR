

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
import ACross
import QtQuick.Layouts 6.3

T.Button {
    id: control

    implicitWidth: Math.max(
                       buttonBackground ? buttonBackground.implicitWidth : 0,
                       textItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        buttonBackground ? buttonBackground.implicitHeight : 0,
                        textItem.implicitHeight + topPadding + bottomPadding)
    height: 40
    width: 100

    text: "Button"
    property bool outline: false
    property url imageSource: "../misc/icons/dark/add.svg"
    background: buttonBackground
    Rectangle {
        id: buttonBackground
        implicitWidth: control.width
        implicitHeight: control.height
        radius: 100
        border.color: outline ? Colors.outline : "transparent"
        HoverHandler {
            acceptedDevices: PointerDevice.Mouse
            cursorShape: Qt.PointingHandCursor
        }
        Ripple {
            anchors.fill: parent
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            clip: true
            clipRadius: parent.radius
            width: parent.width
            height: parent.height
            pressed: control.pressed
            anchor: control
            active: control.enabled && (control.down || control.visualFocus
                                        || control.hovered)
            color: ListView.isCurrentItem ? control.Material.highlightedRippleColor : control.Material.rippleColor
        }
    }
    contentItem: contentRow

    Item {
        id: contentRow
        anchors.fill: parent
        anchors.rightMargin: 16
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.leftMargin: 12
        Image {
            id: image
            width: imageSource.length == 0 ? 0 : 0.45 * Math.min(control.width,
                                                                 control.height)
            height: 0.45 * Math.min(control.width, control.height)
            visible: imageSource.length != 0
            anchors.verticalCenter: parent.verticalCenter
            source: imageSource

            fillMode: Image.PreserveAspectFit
        }

        Text {
            anchors.left: image.right
            anchors.leftMargin: 8
            id: textItem
            text: control.text
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 14
            color: Colors.primary
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    states: [
        State {
            name: "enabled"
            when: control.enabled
            PropertyChanges {
                target: buttonBackground
                color: "transparent"
            }
        },
        State {
            name: "disabled"
            when: !control.enabled
            PropertyChanges {
                target: buttonBackground
                color: "transparent"
            }
            PropertyChanges {
                target: textItem
                color: Colors.onSurface
            }
        }
    ]
}
