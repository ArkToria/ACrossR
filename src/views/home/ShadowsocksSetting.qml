import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../components"

Item {
    implicitWidth: 680
    implicitHeight: 160

    property int textBoxWidth: 72

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing
        spacing: 0

        Label {
            Layout.margins: acrossConfig.itemSpacing

            text: qsTr("Shadowsocks Setting")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("Security")
                color: acrossConfig.textColor
            }

            DropDownBox {
                id: securitySelect
                Layout.fillWidth: true

                model: ["aes-256-gcm", "aes-128-gcm", "chacha20-ietf-poly1305", "chacha20-poly1305", "plain", "none"]
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            spacing: acrossConfig.itemSpacing

            Label {
                Layout.preferredWidth: textBoxWidth

                text: qsTr("IVCheck")
                color: acrossConfig.textColor
            }

            Item {
                Layout.fillWidth: true
            }

            SwitchBox {
                id: ivCheckSelect
            }
        }
    }

    Connections {
        target: streamSettingLoader

        function onAcceptAll() {
            shadowsocksFormModel.security = securitySelect.editText
            shadowsocksFormModel.ivCheck = ivCheckSelect.checked
        }
    }
}
