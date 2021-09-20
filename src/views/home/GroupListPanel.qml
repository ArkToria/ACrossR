import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import ACross 1.0
import ACross

Item {
    id: groupListPanel
    implicitWidth: 312
    implicitHeight: 720

    GroupForm {
        id: updateGroupForm
    }

    RemoveConfirmDialog {
        id: removeConfirmDialog
        headerText: qsTr("Warning")
        contentText: qsTr("Confirm to remove group from database")
    }

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        ToolsBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 48

            z: 1
        }

        ListView {
            id: listScrollView
            Layout.fillWidth: true
            Layout.fillHeight: true

            highlightMoveDuration: 0

            model: GroupModel {
                id: groupModel
                list: acrossGroups
            }

            delegate: GroupItemCard {
                id: groupItemCard
                implicitWidth: listScrollView.width
            }

            highlight: Rectangle {
                color: acrossConfig.highlightColor
            }
        }
    }

    ButtonBox {
        id: panelMenuBackground
        x: parent.width - 72
        y: parent.height - 72
        implicitWidth: 56
        implicitHeight: 56
        z: 1

        basicColor: Qt.lighter(acrossConfig.highlightColor, 1.4)
        backgroundRadius: width / 2
        layer.enabled: true
        layer.effect: DropShadow {
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8
            transparentBorder: true
            //            samples: 17
            color: acrossConfig.shadowColor
        }

        SVGBox {
            anchors.centerIn: parent

            source: "qrc:/misc/icons/" + acrossConfig.iconStyle + "/add.svg"
            sourceWidth: 36
            sourceHeight: sourceWidth
        }

        property Component appendGroupFormComponent: null

        onClicked: {
            darkBackground.show()
            if (appendGroupFormComponent == null) {
                appendGroupFormComponent = Qt.createComponent(
                            "GroupForm.qml")
            }
            if (appendGroupFormComponent.status === Component.Ready) {
                appendGroupFormComponent.createObject(groupListPanel).open()
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/

