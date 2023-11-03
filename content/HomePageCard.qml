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

        Item {
            id: column
            Layout.fillHeight: true
            Layout.fillWidth: true

            Column {
                id: column1
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: 16

                anchors.topMargin: 16
                anchors.leftMargin: 16

                Text {
                    id: homePageCardTitle
                    color: Colors.onSurface
                    width: parent.width
                    wrapMode: Text.WordWrap
                    elide: Text.ElideRight
                    text: qsTr("TextNodeTextNodeTextNodeTextNodeTextNodeTextNodeTextNodeTextNodeTextNodeTextNodeTextNodeTextNodeTextNodeTextNodeTextNodeTextNodeTextNode")
                    font.pixelSize: 22
                }

                Text {
                    width: parent.width
                    text: qsTr("Group: Default GroupGroupGroupGroupGroupGroupGroupGroup")
                    font.pixelSize: 14
                    font.styleName: "Regular"
                    elide: Text.ElideRight

                    color: Colors.onSurfaceVariant
                }
                Text {
                    width: parent.width
                    text: qsTr("Protocol: vmessvmessvmessvmessvmessvmessvmessvmess")
                    font.pixelSize: 14
                    font.styleName: "Regular"
                    elide: Text.ElideRight

                    color: Colors.onSurfaceVariant
                }
                Text {
                    width: parent.width
                    text: qsTr("Address: across.example.orgacross.example.orgacross.example.orgacross.example.org")
                    font.pixelSize: 14
                    font.styleName: "Regular"
                    elide: Text.ElideRight

                    color: Colors.onSurfaceVariant
                }
                Text {
                    width: parent.width
                    text: qsTr("Port: 443443443443443443443443443443443443443443443443")
                    font.pixelSize: 14
                    font.styleName: "Regular"
                    elide: Text.ElideRight

                    color: Colors.onSurfaceVariant
                }
            }

            StartButton {
                id: startButton
                width: 60
                height: 60
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 16
                anchors.rightMargin: 16
            }
        }
    }
}
