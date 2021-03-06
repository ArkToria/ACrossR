import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: groupControlBar

    property Component popMenuComponent: null
    property Component nodeShareFormComponent: null

    implicitHeight: 36
    implicitWidth: 648

    RowLayout {
        anchors.leftMargin: acrossConfig.itemSpacing / 2
        anchors.rightMargin: acrossConfig.itemSpacing
        anchors.fill: parent

        Item {
            Layout.fillWidth: true
            Layout.preferredWidth: 72
        }

        CardBox {
            id: appendNodeButton

            Layout.fillHeight: true
            Layout.fillWidth: true
            layer.enabled: false
            color: "transparent"

            RowLayout {
                id: groupContorlCreate

                anchors.centerIn: parent

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/add_circle.svg"
                    sourceWidth: 22
                    sourceHeight: 22
                }

                Label {
                    text: qsTr("Create")
                    color: acrossConfig.textColor
                }

            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton
                onEntered: {
                    parent.color = acrossConfig.backgroundColor;
                }
                onExited: {
                    parent.color = "transparent";
                }
                onClicked: {
                    if (popMenuComponent == null)
                        popMenuComponent = Qt.createComponent("qrc:/Arktoria/ACross/src/views/home/NodeEditForm.qml");

                    if (popMenuComponent.status === Component.Ready)
                        popMenuComponent.createObject(appendNodeButton).show();

                }
            }

        }

        CardBox {
            Layout.fillHeight: true
            Layout.fillWidth: true
            layer.enabled: false
            color: "transparent"

            RowLayout {
                id: groupContorlCopy

                anchors.centerIn: parent

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/copy.svg"
                    sourceWidth: 22
                    sourceHeight: 22
                }

                Label {
                    text: qsTr("Copy")
                    color: acrossConfig.textColor
                }

            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton
                onEntered: {
                    parent.color = acrossConfig.backgroundColor;
                }
                onExited: {
                    parent.color = "transparent";
                }
                onClicked: {
                    acrossNodes.copyCurrentNodeURLToClipboard();
                }
            }

        }

        CardBox {
            Layout.fillHeight: true
            Layout.fillWidth: true
            layer.enabled: false
            color: "transparent"

            RowLayout {
                id: groupContorlQRCode

                anchors.centerIn: parent

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/qr_code.svg"
                    sourceWidth: 22
                    sourceHeight: 22
                }

                Label {
                    text: qsTr("QRCode")
                    color: acrossConfig.textColor
                }

            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton
                onEntered: {
                    parent.color = acrossConfig.backgroundColor;
                }
                onExited: {
                    parent.color = "transparent";
                }
                onClicked: {
                    let nodeModel = acrossNodes.currentNodeInfo;
                    if (nodeModel === null)
                        return ;

                    if (nodeShareFormComponent == null)
                        nodeShareFormComponent = Qt.createComponent("qrc:/Arktoria/ACross/src/views/home/NodeShareForm.qml");

                    if (nodeShareFormComponent.status === Component.Ready) {
                        var window = nodeShareFormComponent.createObject(groupControlBar, {
                            "nodeModel": nodeModel
                        });
                        window.show();
                    }
                }
            }

        }

        CardBox {
            Layout.fillHeight: true
            Layout.fillWidth: true
            layer.enabled: false
            color: "transparent"

            RowLayout {
                id: groupContorlLocate

                anchors.centerIn: parent

                SVGBox {
                    source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/location.svg"
                    sourceWidth: 22
                    sourceHeight: 22
                }

                Label {
                    text: qsTr("Locate")
                    color: acrossConfig.textColor
                }

            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    parent.color = acrossConfig.backgroundColor;
                }
                onExited: {
                    parent.color = "transparent";
                }
                onClicked: {
                    groupListPanel.setIndexByID(acrossNodes.currentGroupID);
                    nodeListPanel.locateCurrent();
                }
            }

        }

    }

}
