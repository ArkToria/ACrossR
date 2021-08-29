import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0

import "../components"

Menu {
    topPadding: 8
    bottomPadding: 8

    background: CardBox {
        id: popMenuBackground
        color: acrossConfig.backgroundColor
        border.width: 1
        border.color: acrossConfig.deepColor
    }

    delegate: MenuItem {
        id: menuItem

        contentItem: Text {
            text: menuItem.text
            color: menuItem.highlighted ? acrossConfig.highlightTextColor : acrossConfig.textColor
        }

        background: Rectangle {
            implicitWidth: 168
            color: menuItem.highlighted ? acrossConfig.highlightColor : "transparent"
        }
    }

    Action {
        text: qsTr("Edit")
        onTriggered: {

        }
    }

    Action {
        text: qsTr("Copy URL")
        onTriggered: {

        }
    }

    Action {
        text: qsTr("ICMP Ping")
    }

    Action {
        text: qsTr("TCP Ping")
    }

    MenuSeparator {
        background: Rectangle {
            height: 1
            color: acrossConfig.deepColor
        }
    }

    Action {
        text: qsTr("Set as default")
    }

    Action {
        text: qsTr("Delete")

        onTriggered: {
            acrossNodes.removeCurrentNode(id)
        }
    }
}
