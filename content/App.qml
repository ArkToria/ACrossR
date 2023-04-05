import QtQuick 6.4
import QtQuick.Controls 6.4
import ACross
import QtQuick.Layouts 6.3
import QtQuick.Controls.Material

Window {
    id: window
    width: Constants.width
    height: Constants.height

    color: Colors.background
    Material.theme: Material.Dark
    Material.accent: Material.Purple

    visible: true

    SwipeView {
        id: swipeView
        anchors.left: mainPanel.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        orientation: Qt.Vertical
        anchors.bottomMargin: 0
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        currentIndex: mainPanelList.currentIndex
        onCurrentIndexChanged: {
            if (mainPanelList.currentIndex != currentIndex) {
                mainPanelList.currentIndex = currentIndex
            }
        }

        HomePage {
            id: homePage
        }

        LogsPage {
            id: logsPage
        }

        SettingPage {
            id: settingPage
        }

        HelpPage {
            id: helpPage
        }
    }

    Rectangle {
        id: mainPanel
        width: 88
        color: Colors.surface1
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.bottomMargin: 0

        ColumnLayout {
            id: mainPanelLayout
            anchors.fill: parent

            ColumnLayout {
                id: navigation
                width: 80
                height: 100
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                anchors.topMargin: 0
                layoutDirection: Qt.LeftToRight
                spacing: 40

                ColumnLayout {
                    id: menuAndFab
                    width: 80
                    height: 108
                    Layout.topMargin: 32
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

                    ToolButton {
                        id: toolButton
                        text: qsTr("Tool Button")
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        icon.source: "../misc/icons/dark/menu.svg"
                        icon.color: Colors.onSurfaceVariant
                        display: AbstractButton.IconOnly
                        HoverHandler {
                            acceptedDevices: PointerDevice.Mouse
                            cursorShape: Qt.PointingHandCursor
                        }
                    }

                    Fab {
                        id: fab1
                        color: Colors.fabColor
                    }
                }

                ListView {
                    id: mainPanelList
                    boundsBehavior: Flickable.OvershootBounds
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    clip: true
                    model: ListModel {
                        id: listModel
                        property bool completed: false
                        Component.onCompleted: {
                            append({
                                       "name": qsTr("Home"),
                                       "buttonImage": "../misc/icons/dark/across.svg",
                                       "buttonColor": Colors.panelButtonColor.toString()
                                   })
                            append({
                                       "name": qsTr("Logs"),
                                       "buttonImage": "../misc/icons/dark/log.svg",
                                       "buttonColor": Colors.panelButtonColor.toString()
                                   })
                            append({
                                       "name": qsTr("Setting"),
                                       "buttonImage": "../misc/icons/dark/setting.svg",
                                       "buttonColor": Colors.panelButtonColor.toString()
                                   })
                            append({
                                       "name": qsTr("Help"),
                                       "buttonImage": "../misc/icons/dark/help.svg",
                                       "buttonColor": Colors.panelButtonColor.toString()
                                   })
                            completed = true
                        }
                    }

                    delegate: Item {
                        height: 56
                        width: 88
                        MainPanelButton {
                            anchors.centerIn: parent
                            checked: parent.ListView.isCurrentItem
                            onClicked: {
                                if (mainPanelList.currentIndex != index) {
                                    mainPanelList.currentIndex = index
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
