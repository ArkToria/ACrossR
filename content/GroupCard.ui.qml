import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ACross 1.0

Rectangle {
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
            Layout.rightMargin: 4
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
                    text: modifiedAt
                    font.pixelSize: 14
                    font.styleName: "Regular"

                    Layout.fillWidth: true
                    elide: Text.ElideRight

                    color: Colors.onSurface
                }
            }
            Item {
                Layout.fillWidth: true
            }
            ToolButton {
                id: updateToolButton
                icon.source: "../misc/icons/dark/update.svg"
                icon.color: Colors.onSurfaceVariant
                display: AbstractButton.IconOnly
                HoverHandler {
                    acceptedDevices: PointerDevice.Mouse
                    cursorShape: Qt.PointingHandCursor
                }
            }
            ToolButton {
                id: moreToolButton
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
                text: type
                font.pixelSize: 16

                Layout.fillWidth: true
                elide: Text.ElideRight

                color: Colors.onSurface
            }

            Label {
                id: label4
                text: subType
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
                text: count + qsTr(" Proxies")
                color: Colors.onSurfaceVariant

                Layout.fillWidth: true
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignRight

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
        }
    }
}
