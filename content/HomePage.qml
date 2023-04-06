

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.4
import QtQuick.Controls 6.4
import ACross
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
                        Rectangle {
                            width: cardList.cellWidth - 8
                            height: 192
                            color: colorCode
                            radius: 12
                        }
                    }
                }

                model: ListModel {
                    ListElement {
                        name: "Grey"
                        colorCode: "grey"
                    }

                    ListElement {
                        name: "Red"
                        colorCode: "red"
                    }

                    ListElement {
                        name: "Blue"
                        colorCode: "blue"
                    }

                    ListElement {
                        name: "Green"
                        colorCode: "green"
                    }
                }
            }
        }
    }
}
