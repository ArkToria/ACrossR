import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ACross 1.0

Rectangle {
    id: card
    width: 300
    height: 192
    color: Colors.surface1
    radius: 12

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            id: rowLayout
            height: 72
            spacing: 0
            Layout.bottomMargin: 12
            Layout.topMargin: 12
            Layout.rightMargin: 16
            Layout.leftMargin: 16
            Layout.fillWidth: true

            ColumnLayout {
                id: columnLayout1
                spacing: 4

                Layout.fillWidth: true

                Label {
                    id: label1
                    text: name
                    font.pixelSize: 16
                    font.styleName: "Medium"
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                    color: Colors.onSurface
                }

                Label {
                    id: label2
                    text: address
                    Layout.fillWidth: true
                    font.pixelSize: 14
                    font.styleName: "Regular"
                    elide: Text.ElideRight
                    color: Colors.onSurface
                }
            }
            Item {
                Layout.fillWidth: true
            }

            ToolButton {
                id: toolButton
                icon.source: "../misc/icons/dark/more_vert.svg"
                icon.color: Colors.onSurfaceVariant
                display: AbstractButton.IconOnly
                HoverHandler {
                    acceptedDevices: PointerDevice.Mouse
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }

        ColumnLayout {
            id: columnLayout

            spacing: 0
            Layout.margins: 16

            Label {
                id: label3
                text: protocol
                font.pixelSize: 16
                Layout.fillWidth: true
                elide: Text.ElideRight
                color: Colors.onSurface
            }

            Label {
                id: label4
                text: routingName
                font.pixelSize: 14
                font.styleName: "Regular"
                Layout.fillWidth: true
                elide: Text.ElideRight
                color: Colors.onSurfaceVariant
            }
        }

        RowLayout {
            id: rowLayout1
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            Layout.margins: 12
            spacing: 10

            Label {
                id: label
                Layout.fillWidth: true
                text: latency === -1 ? "" : latency + qsTr("ms")
                color: Colors.onSurfaceVariant
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
        }
    }
}
