import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    implicitWidth: 720
    implicitHeight: 260

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing
        spacing: 0

        Label {
            Layout.margins: acrossConfig.itemSpacing

            text: qsTr("Trojan Setting")
            font.pixelSize: 24
            color: acrossConfig.textColor
        }

        GridLayout {
            Layout.fillWidth: true
            Layout.margins: acrossConfig.itemSpacing

            columns: 2
            rowSpacing: acrossConfig.itemSpacing

            Label {
                text: qsTr("Host")
                color: acrossConfig.textColor
            }

            TextFieldBox {
                id: serverNameText
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Network")
                color: acrossConfig.textColor
            }

            DropDownBox {
                id: networkSelect
                Layout.fillWidth: true

                model: ["tcp"]
            }

            Label {
                text: qsTr("Security")
                color: acrossConfig.textColor
            }

            DropDownBox {
                id: securitySelect
                Layout.fillWidth: true

                model: ["tls", "none"]
            }

            Label {
                text: qsTr("ALPN")
                color: acrossConfig.textColor
            }

            DropDownBox {
                id: alpnSelect
                Layout.fillWidth: true

                model: ["h2+http/1.1", "h2", "http/1.1"]
            }
        }
    }

    Connections {
        target: streamSettingLoader

        function onAcceptAll() {
            trojanFormModel.serverName = serverNameText.text
            trojanFormModel.network = networkSelect.editText
            trojanFormModel.security = securitySelect.editText
            trojanFormModel.alpn = alpnSelect.editText
        }
    }
}
