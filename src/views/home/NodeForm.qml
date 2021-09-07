import QtQuick 2.12
import QtQuick.Window 2.15
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import ACross 1.0

import "../components/"

Window {
    id: nodeFormPopWindow
    width: 680
    height: 300
    minimumWidth: 680
    minimumHeight: 300
    title: qsTr("Create New Configuration")

    flags: Qt.WindowStaysOnTopHint
    modality: Qt.ApplicationModal

    ShadowsocksFormModel {
        id: shadowsocksFormModel
    }

    TrojanFormModel {
        id: trojanFormModel
    }

    VMessFormModel {
        id: vmessFormModel
    }

    RawOutboundFormModel {
        id: rawOutboundFormModel
    }

    URLSchemeFormModel {
        id: urlSchemeFormModel
    }

    NodeFormModel {
        id: nodeFormModel
        list: acrossNodes
        trojan: trojanFormModel
        shadowsocks: shadowsocksFormModel
        vmess: vmessFormModel
        raw: rawOutboundFormModel
        urlScheme: urlSchemeFormModel
    }

    Rectangle {
        id: background
        anchors.fill: parent

        color: acrossConfig.deepColor

        ScrollView {
            id: scrollView
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing
            contentWidth: column.width
            contentHeight: column.height
            clip: true

            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AlwaysOff

            Column {
                id: column
                width: scrollView.availableWidth
                spacing: acrossConfig.itemSpacing

                CardBox {
                    id: outboundSetting
                    implicitWidth: column.width
                    implicitHeight: 64

                    layer.enabled: false

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.margins: acrossConfig.itemSpacing

                            spacing: acrossConfig.itemSpacing

                            Label {
                                Layout.margins: acrossConfig.itemSpacing

                                text: qsTr("Outbound")
                                font.pixelSize: 24
                                color: acrossConfig.textColor
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            DropDownBox {
                                model: ["url", "manual", "outbound"]

                                onEditTextChanged: {
                                    var height = 64
                                    manualSetting.visible = false

                                    switch (editText) {
                                    case "manual":
                                        nodeFormModel.protocol = "vmess"
                                        height += manualSetting.implicitHeight
                                        manualSetting.visible = true
                                        streamSettingLoader.source
                                                = "qrc:/src/views/home/VMESSSetting.qml"
                                        break
                                    case "outbound":
                                        nodeFormModel.protocol = "raw"
                                        streamSettingLoader.source
                                                = "qrc:/src/views/home/RawOutboundSetting.qml"
                                        break
                                    case "url":
                                        nodeFormModel.protocol = "scheme"
                                        streamSettingLoader.source
                                                = "qrc:/src/views/home/URLSchemeSetting.qml"
                                        break
                                    default:
                                        console.log("unknown setting")
                                    }

                                    outboundSetting.implicitHeight = height
                                }
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                        }

                        ManualSetting {
                            id: manualSetting
                            Layout.fillWidth: true

                            visible: false
                        }
                    }
                }

                CardBox {
                    implicitWidth: column.width
                    implicitHeight: streamSettingLoader.implicitHeight

                    layer.enabled: false

                    Loader {
                        id: streamSettingLoader
                        anchors.fill: parent

                        signal acceptAll
                    }
                }

                RowLayout {
                    spacing: acrossConfig.itemSpacing
                    implicitWidth: column.width

                    Item {
                        Layout.fillWidth: true
                    }

                    ButtonBox {
                        id: acceptFormButton
                        text: qsTr("Accept")
                        onClicked: {
                            streamSettingLoader.acceptAll()
                            nodeFormModel.accept()
                            nodeFormPopWindow.close()
                        }
                    }

                    ButtonBox {
                        text: qsTr("Cancel")

                        onClicked: {
                            nodeFormModel.cancel()
                            nodeFormPopWindow.close()
                        }
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/

