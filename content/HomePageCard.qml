import QtQuick 2.15
import QtQuick.Controls 2.15
import ACross
import Qt5Compat.GraphicalEffects
import QtQuick.Layouts 6.3

Rectangle {
    id: stackedCard
    height: 220
    color: Colors.surface1
    radius: 16

    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: Rectangle {
            width: stackedCard.width
            height: stackedCard.height
            radius: stackedCard.radius
        }
    }

    RowLayout {
        id: row
        anchors.fill: parent
        spacing: 0

        Dashboard {}

        Column {
            id: column
            Layout.fillHeight: true
            Layout.fillWidth: true

            Column {
                id: column1
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: 16
                anchors.leftMargin: 16

                Text {
                    id: text2
                    color: Colors.onSurface
                    text: qsTr("TextNode")
                    font.pixelSize: 22
                    font.family: "Roboto"
                }

                Text {
                    id: text3
                    text: qsTr("Group: Default Group\nProtocol: vmess\nAddress: across.example.org\nPort: 443")
                    font.pixelSize: 14
                    font.styleName: "Regular"
                    font.family: "Roboto"
                    color: Colors.onSurfaceVariant
                }
            }
        }
    }
}
