import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    implicitHeight: apiStateText.contentHeight + coreVersionText.contentHeight + guiVersionText.contentHeight
    property Component apiFormComponent: null
    function openApiForm() {
        darkBackground.show();
        if (apiFormComponent == null)
            apiFormComponent = Qt.createComponent("qrc:/Arktoria/ACross/src/views/menu/ApiForm.qml");

        if (apiFormComponent.status === Component.Ready)
            apiFormComponent.createObject(parent).open();

    }

    ColumnLayout {
        anchors.fill: parent
        anchors.centerIn: parent

        Row {
            Layout.alignment: Qt.AlignHCenter
            Label {
                id: apiStateText
                text: "AColoRS"
                color: acrossConfig.textColor;
            }
            SVGBox {
                id: powerIcon
                // visible: true
                source: "qrc:/misc/icons/" + acrossConfig.iconStyle + ( acolorsNotifications.isRunning ? "/power.svg" : "/power_off.svg" )
                sourceWidth: 20
                sourceHeight: 20
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onClicked: {
                        openApiForm();
                    }
                    hoverEnabled: true
                    cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
                }
            }
        }
        

        Label {
            id: coreVersionText

            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
            text: acrossConfig.coreName + "\n" + acrossConfig.coreVersion
            color: acrossConfig.textColor
        }

        Label {
            id: guiVersionText

            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("Client") + " " + acrossConfig.guiVersion
            color: acrossConfig.textColor
        }

    }

}
