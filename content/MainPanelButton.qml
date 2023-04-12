import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.3
import QtQuick.Templates as T
import QtQuick.Controls.Material.impl 2.15
import Qt5Compat.GraphicalEffects
import ACross

Item {
    id: button
    width: 70
    height: 56
    implicitWidth: 70
    implicitHeight: 56

    signal clicked
    property string text: "Button"
    property color color: "#4a4458"
    property url image: "../misc/icons/dark/across.svg"
    //property alias text: name ? name !== null : "Button"
    property bool checked: true
    //property alias source: image.source
    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 4

        Rectangle {
            width: 56
            height: 32
            radius: 100
            color: checked ? button.color : "transparent"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            T.Button {
                id: buttonInside
                width: 56
                height: 32

                onClicked: button.clicked()
                Component.onCompleted: buttonInside.__behavior.cursorShape = Qt.PointingHandCursor
                background: Rectangle {
                    id: rectangle
                    anchors.fill: parent
                    color: button.color
                    visible: buttonInside.hovered
                    opacity: 0.25
                    radius: 100
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                    Ripple {
                        clip: true
                        clipRadius: parent.radius
                        width: parent.width
                        height: parent.height
                        pressed: buttonInside.pressed
                        anchor: buttonInside
                        active: buttonInside.enabled
                                && (buttonInside.down
                                    || buttonInside.visualFocus
                                    || buttonInside.hovered)
                        color: "#20FFFFFF"
                    }
                }
            }
            ColumnLayout {
                id: imageLayout
                anchors.fill: buttonInside

                Image {
                    id: image
                    width: 24
                    height: 24
                    source: button.image
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    sourceSize.height: 24
                    sourceSize.width: 24
                    fillMode: Image.PreserveAspectFit
                }
            }
        }

        Label {
            id: label
            text: button.text
            font.styleName: "Medium"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            color: Colors.onSurface
            font.family: "Roboto"
        }
    }
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        propagateComposedEvents: true
        enabled: false
    }
}
