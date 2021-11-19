import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross
import "../typescripts/home.js" as HomeJS

Item {
    implicitWidth: 680
    implicitHeight: 300

    property int fontSize: 14

    onVisibleChanged: {
        HomeJS.visibleChangeToggle(visible, {
                                       "alterIDText": alterIDText,
                                       "securitySelect": securitySelect,
                                       "networkSelect": networkSelect,
                                       "pathText": pathText,
                                       "hostText": hostText,
                                       "typeSelect": typeSelect,
                                       "quicSecuritySelect": quicSecuritySelect,
                                       "tlsEnableSelect": tlsEnableSelect
                                   })
    }

    GridLayout {
        anchors.fill: parent

        columns: 4
        rowSpacing: acrossConfig.itemSpacing * 2
        columnSpacing: acrossConfig.itemSpacing * 2

        Label {
            Layout.fillWidth: true
            Layout.columnSpan: 4

            text: qsTr("VMESS Setting")
            font.pointSize: fontSize
            color: acrossConfig.textColor
        }

        Label {
            text: qsTr("Enable TLS")
            color: acrossConfig.textColor
        }

        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        SwitchBox {
            id: tlsEnableSelect

            onCheckedChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Label {
            text: qsTr("AlterID")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: alterIDText
            Layout.fillWidth: true
            Layout.columnSpan: 3

            placeholderText: "[0-65535] 0: auto enable VMessAEAD"
            validator: IntValidator {
                bottom: 0
                top: 65535
            }

            onTextChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Label {
            text: qsTr("Security")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: securitySelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["auto", "aes-128-gcm", "chacha20-poly1305", "none", "zero"]

            onEditTextChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Label {
            text: qsTr("Network")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: networkSelect
            Layout.fillWidth: true
            Layout.columnSpan: 3
            model: ["tcp", "ws", "grpc", "quic"]

            onEditTextChanged: HomeJS.networkSelectToggle(editText, {
                                                              "typeLabel": typeLabel,
                                                              "typeSelect": typeSelect,
                                                              "quicSecurityLabel": quicSecurityLabel,
                                                              "quicSecuritySelect": quicSecuritySelect,
                                                              "hostLabel": hostLabel,
                                                              "hostText": hostText,
                                                              "pathLabel": pathLabel,
                                                              "pathText": pathText
                                                          },
                                                          nodeEditFormPopWindow)
        }

        Label {
            id: typeLabel
            text: qsTr("Type")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: typeSelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["none", "http", "srtp", "utp", "wechat-video", "wireguard"]

            onEditTextChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Label {
            id: quicSecurityLabel
            text: qsTr("Securty")
            color: acrossConfig.textColor
        }

        DropDownBox {
            id: quicSecuritySelect
            Layout.fillWidth: true
            Layout.columnSpan: 3

            model: ["none", "aes-128-gcm", "chacha20-poly1305"]

            onEditTextChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Label {
            id: hostLabel
            text: qsTr("Host")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: hostText
            Layout.fillWidth: true
            Layout.columnSpan: 3

            color: acrossConfig.textColor

            onTextChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Label {
            id: pathLabel
            text: qsTr("Path")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: pathText
            Layout.fillWidth: true
            Layout.columnSpan: 3

            color: acrossConfig.textColor

            onTextChanged: {
                nodeEditFormPopWindow.configChanged()
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Connections {
        target: protocolSettingsLoader

        function onAcceptAll() {
            vmessSetting = {
                "enableTLS": tlsEnableSelect.checked,
                "alterID": alterIDText.text,
                "security": securitySelect.currentText,
                "network": networkSelect.currentText,
                "type": typeSelect.currentText,
                "host": hostText.visible === true ? hostText.text : quicSecuritySelect.currentText,
                "path": pathText.text
            }
        }
    }
}
