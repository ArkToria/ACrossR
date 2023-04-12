

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.4
import QtQuick.Controls 6.4
import QtQuick.Controls.Basic as Basic
import ACross
import QtQuick.Controls.Material.impl 2.15
import Qt5Compat.GraphicalEffects
import QtQuick.Shapes
import QtQuick.Layouts 6.3

Rectangle {
    id: rectangle
    width: Constants.width
    height: Constants.height

    color: Colors.background

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: homeDrawer
            width: 260
            color: Colors.surface1
            radius: 16
            border.width: 0
            Layout.leftMargin: 1
            Layout.fillHeight: true

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
                            MouseArea {
                                id: mouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                enabled: true
                                cursorShape: Qt.IBeamCursor
                            }
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
                            }
                        }
                        Item {
                            id: searchButton
                            Layout.rightMargin: 14
                            Layout.preferredHeight: 24
                            Layout.preferredWidth: 24
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            Layout.bottomMargin: 2
                            Layout.topMargin: 2
                            Layout.fillHeight: true
                            MouseArea {
                                id: searchButtonArea
                                anchors.fill: parent
                                hoverEnabled: true
                                enabled: true
                                cursorShape: Qt.PointingHandCursor
                            }
                            Image {
                                id: image
                                anchors.fill: parent
                                source: "../misc/icons/dark/search.svg"
                            }
                        }
                    }
                }
                Component {
                    id: sectionHeading
                    Item {
                        id: sectionHeadingItem

                        height: hasDivider ? 57 : 56
                        property bool hasDivider: groupListView.itemAtIndex(
                                                      0) ? (section !== groupListView.itemAtIndex(
                                                                0).group) : 0
                        required property string section

                        anchors.left: parent.left
                        anchors.right: parent.right
                        Rectangle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.leftMargin: 16
                            anchors.rightMargin: 16
                            height: 1
                            color: Colors.outlineVariant
                            visible: parent.hasDivider
                        }

                        Item {
                            id: textItem
                            height: 56
                            anchors.left: parent.left
                            anchors.right: parent.right

                            property alias section: sectionHeadingItem.section

                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 16
                                text: parent.section
                                color: Colors.onSurfaceVariant
                                font.pixelSize: 14
                                font.styleName: "Medium"
                                font.family: "Roboto"
                            }
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
                                    sides: sidesCount
                                }
                            }

                            Text {
                                text: name
                                color: Colors.onSurfaceVariant
                                Layout.alignment: Qt.AlignHCenter
                                font.pixelSize: 14
                                font.styleName: "Medium"
                                font.family: "Roboto"
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
                                font.family: "Roboto"
                            }
                        }
                        Rectangle {
                            id: hoveredRectangle
                            anchors.fill: parent
                            color: Colors.secondaryContainer
                            visible: groupListMouseArea.containsMouse
                            opacity: 0.25
                            radius: 100
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            Ripple {
                                id: ripple
                                anchors.fill: parent
                                anchor: hoveredRectangle
                                clipRadius: 4
                                pressed: groupListMouseArea.pressed
                                active: groupListMouseArea.containsPress
                                layer.enabled: true
                                color: "#20FFFFFF"
                                layer.effect: OpacityMask {
                                    maskSource: Rectangle {
                                        width: ripple.width
                                        height: ripple.height
                                        radius: hoveredRectangle.radius
                                    }
                                }
                            }
                        }
                        MouseArea {
                            id: groupListMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: groupListView.currentIndex = index
                        }
                    }
                    section.property: "group"
                    section.delegate: sectionHeading
                    highlight: Rectangle {
                        color: Colors.secondaryContainer
                        radius: 100
                    }
                    highlightMoveVelocity: -1
                    model: ListModel {
                        ListElement {
                            name: "Default Group"
                            group: "Section 1"
                            count: "100+"
                            sidesCount: 36
                        }

                        ListElement {
                            name: "Public"
                            group: "Section 1"
                            count: "100+"
                            sidesCount: 3
                        }
                        ListElement {
                            name: "vmess"
                            group: "Section 1"
                            count: "100+"
                            sidesCount: 4
                        }
                        ListElement {
                            name: "Hallo"
                            group: "Section 1"
                            count: "100+"
                            sidesCount: 5
                        }

                        ListElement {
                            name: "Eins"
                            group: "Section Header"
                            count: "100+"
                            sidesCount: 36
                        }

                        ListElement {
                            name: "Zwei"
                            group: "Section Header"
                            count: "100+"
                            sidesCount: 3
                        }
                        ListElement {
                            name: "Drei"
                            group: "Section Header"
                            count: "100+"
                            sidesCount: 4
                        }
                    }
                }
            }
        }

        ColumnLayout {
            spacing: 8
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillHeight: true

            Rectangle {
                id: stackedCard
                height: 220
                color: Colors.surface1
                radius: 16
                Layout.topMargin: 8
                Layout.rowSpan: 6
                Layout.rightMargin: 8
                Layout.leftMargin: 8
                Layout.fillWidth: true
            }

            RowLayout {
                id: rowLayout1
                width: 100
                height: 100
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                spacing: 5
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.fillWidth: false
                layoutDirection: Qt.RightToLeft

                Rectangle {
                    id: rectangle1
                    width: 100
                    height: 40
                    color: Colors.primary
                    radius: 100
                    opacity: 0.12
                }

                Rectangle {
                    id: rectangle2
                    width: 100
                    height: 40
                    opacity: 0.12
                    color: Colors.primary
                    radius: 100
                }

                Rectangle {
                    id: rectangle3
                    width: 100
                    height: 40
                    opacity: 0.12
                    color: Colors.primary
                    radius: 100
                }
            }

            GridView {
                id: cardList
                Layout.rightMargin: 0
                Layout.leftMargin: 8
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true

                cellWidth: width / Math.floor(width / 300)
                cellHeight: 200

                delegate: Column {
                    Item {
                        NodeCard {
                            width: cardList.cellWidth - 8
                            height: 192
                            radius: 12
                        }
                    }
                }

                model: ListModel {
                    ListElement {
                        nodeName: "Node 1"
                        address: "example.arktoria.org"
                        protocol: "vmess"
                        routings: "default_routings"
                        latency: "114ms"
                    }

                    ListElement {
                        nodeName: "Node 2"
                        address: "example.arktoria.org"
                        protocol: "vmess"
                        routings: "default_routings"
                        latency: "114ms"
                    }

                    ListElement {
                        nodeName: "Node 3"
                        address: "example.arktoria.org"
                        protocol: "vmess"
                        routings: "default_routings"
                        latency: "114ms"
                    }

                    ListElement {
                        nodeName: "Node 4"
                        address: "example.arktoria.org"
                        protocol: "vmess"
                        routings: "default_routings"
                        latency: "114ms"
                    }
                }
            }
        }
    }
}
