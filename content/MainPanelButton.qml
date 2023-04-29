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
    MouseArea {
        id: mouseArea
        anchors.fill: button
        onClicked: button.clicked()

        propagateComposedEvents: true
        cursorShape: Qt.PointingHandCursor
    }
    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 4

        Rectangle {
            id: rect
            width: 56
            height: 32
            radius: 100
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            color: checked ? button.color : "transparent"

            T.Button {
                id: buttonInside
                anchors.fill: parent
                onClicked: button.clicked()

                background: Item {
                    Ripple {
                        anchors.fill: parent
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        clip: true
                        clipRadius: 100
                        width: parent.width
                        height: parent.height
                        pressed: buttonInside.pressed
                        anchor: buttonInside
                        color: ListView.isCurrentItem ? control.Material.highlightedRippleColor : buttonInside.Material.rippleColor
                    }
                    Rectangle {
                        radius: 100

                        opacity: buttonInside.hovered ? 1.0 : 0.0
                        color: ListView.isCurrentItem ? control.Material.highlightedRippleColor : buttonInside.Material.rippleColor
                        anchors.fill: parent
                        HoverHandler {
                            cursorShape: Qt.PointingHandCursor
                        }
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
            
        }
    }
}
