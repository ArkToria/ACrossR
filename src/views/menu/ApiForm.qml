import Arktoria.ACross
import Qt5Compat.GraphicalEffects
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic as ControlsBasic
import QtQuick.Layouts

Popup {
    id: popWindow

    property string title: qsTr("AColoRS API settings")
    property int fontSize: 14

    implicitWidth: 480
    implicitHeight: 300
    contentHeight: implicitHeight
    contentWidth: implicitWidth
    x: Math.round((mainWindow.width - width) / 2)
    y: Math.round((mainWindow.height - height) / 2)
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    onClosed: {
        darkBackground.close();
    }

    background: CardBox {
    }

    contentItem: Item {
        anchors.fill: parent

        GridLayout {
            anchors.fill: parent
            anchors.margins: acrossConfig.itemSpacing * 4
            rowSpacing: acrossConfig.itemSpacing * 2
            columnSpacing: acrossConfig.itemSpacing * 2
            columns: 4

            Label {
                Layout.fillWidth: true
                Layout.columnSpan: 4
                text: title
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: Math.round(fontSize * 1.2)
                color: acrossConfig.textColor
            }

            Label {
                text: qsTr("Target")
                color: acrossConfig.textColor
                font.pointSize: fontSize
            }

            TextFieldBox {
                id: target

                Layout.columnSpan: 3
                Layout.fillWidth: true
                text: acolors.target
            }


            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.columnSpan: 4
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.columnSpan: 4
                spacing: acrossConfig.itemSpacing * 2

                Item {
                    Layout.fillWidth: true
                }

                ButtonBox {
                    text: qsTr("Accept")
                    onClicked: {
                        acolors.target = target.text
                        acolors.reconnect();
                        popWindow.close();
                    }
                }

                ButtonBox {
                    text: qsTr("Cancel")
                    onClicked: {
                        popWindow.close();
                    }
                }

            }

        }

    }

}
