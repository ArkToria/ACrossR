import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.3
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

    //property alias text: name ? name !== null : "Button"
    property bool checked: true
    //property color buttonColor: buttonColor !== null ? buttonColor : "#e8def8"
    //property alias source: image.source
    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 4

        Rectangle {
            width: 56
            height: 32
            radius: 100
            color: checked ? buttonColor : "transparent"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
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
            Rectangle {
                id: rectangle
                width: 56
                height: 32
                color: mouseArea.containsMouse ? buttonColor : "transparent"
                opacity: 0.25
                radius: 100
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
            ColumnLayout {
                id: imageLayout
                anchors.fill: rectangle

                Image {
                    id: image
                    width: 24
                    height: 24
                    source: buttonImage
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    sourceSize.height: 24
                    sourceSize.width: 24
                    fillMode: Image.PreserveAspectFit
                }
            }
        }

        Label {
            id: label
            text: name
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            color: Colors.onSurface
            font.family: "Roboto Medium"
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: button.clicked()
    }
}
