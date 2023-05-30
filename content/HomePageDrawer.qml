import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.3
import QtQuick.Controls.Basic as Basic
import QtQuick.Templates as T
import ACross
import QtQuick.Controls.Material.impl 2.15
import Qt5Compat.GraphicalEffects

Rectangle {
    id: homeDrawer
    width: 260
    color: Colors.surface1
    radius: 16
    border.width: 0

    Fab {
        id: fab
        color: Colors.fabColor
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        z: 1
        anchors.bottomMargin: 12
        anchors.rightMargin: 12
    }

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        anchors.bottomMargin: 12
        anchors.topMargin: 12

        Rectangle {
            id: searchBar
            clip: true
            height: 28
            color: Colors.surface3
            radius: 28
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true

            RowLayout {
                id: rowLayout2
                anchors.fill: parent
                spacing: 4

                Item {
                    id: searchText
                    Layout.bottomMargin: 4
                    Layout.topMargin: 4
                    Layout.fillHeight: true
                    anchors.bottomMargin: 4
                    anchors.topMargin: 4
                    Layout.rightMargin: 10
                    Layout.leftMargin: 10
                    Layout.fillWidth: true

                    TextInput {
                        id: text1
                        text: qsTr("Hinted search text")
                        anchors.fill: parent

                        font.pixelSize: 12
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.NoWrap
                        selectionColor: Colors.secondary
                        cursorVisible: false

                        color: Colors.onSurfaceVariant
                        selectByMouse: true

                        HoverHandler {
                            id: hoverHandler
                            cursorShape: Qt.IBeamCursor
                        }
                    }
                }
                ToolButton {
                    id: searchButton
                    Layout.rightMargin: 8
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 32
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.bottomMargin: -8
                    Layout.topMargin: -8
                    Layout.fillHeight: true
                    display: AbstractButton.IconOnly
                    icon.source: "../misc/icons/dark/search.svg"
                    icon.height: 32
                    icon.width: 32

                    HoverHandler {
                        acceptedDevices: PointerDevice.Mouse
                        cursorShape: Qt.PointingHandCursor
                    }
                }
            }
        }
        Item {
            id: headline

            height: 56

            Layout.fillWidth: true

            Item {
                id: textItem
                height: 56
                anchors.left: parent.left
                anchors.right: parent.right

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    text: qsTr("Groups")
                    color: Colors.onSurfaceVariant
                    font.pixelSize: 14
                    font.styleName: "Medium"
                }
            }
        }
        ListView {
            id: groupListView
            clip: true
            Layout.fillHeight: true
            Layout.fillWidth: true
            ScrollBar.vertical: Basic.ScrollBar {
                active: hovered || pressed
                hoverEnabled: true
            }
            delegate: Item {
                height: 56
                anchors.left: parent.left
                anchors.right: parent.right
                RowLayout {
                    z: 1
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 24
                    anchors.bottomMargin: 16
                    anchors.topMargin: 16
                    spacing: 12
                    id: row1
                    Item {
                        width: 24
                        height: 24
                        GroupIcon {
                            width: 14
                            height: 14
                            anchors.centerIn: parent
                            sides: 5
                        }
                    }

                    Text {
                        text: name
                        color: Colors.onSurfaceVariant
                        Layout.alignment: Qt.AlignHCenter
                        font.pixelSize: 14
                        font.styleName: "Medium"
                    }
                    Item {
                        Layout.fillWidth: true
                    }

                    Text {
                        z: 1
                        width: 32
                        text: count
                        Layout.alignment: Qt.AlignHCenter
                        color: Colors.onSurfaceVariant
                        font.pixelSize: 14
                        font.styleName: "Medium"
                    }
                }
                T.Button {
                    id: buttonInside
                    anchors.fill: parent

                    onClicked: groupListView.currentIndex = index
                    HoverHandler {
                        acceptedDevices: PointerDevice.Mouse
                        cursorShape: Qt.PointingHandCursor
                    }
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
                        }
                    }
                }
            }
            highlight: Rectangle {
                color: Colors.secondaryContainer
                radius: 100
            }
            highlightMoveVelocity: -1
            model: cxxObject.acrossGroupModel
        }
    }
}
