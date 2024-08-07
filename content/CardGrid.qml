import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Basic as ControlsBasic

GridView {
    id: cardList

    clip: true

    cellWidth: width / Math.floor(width / 275)
    cellHeight: 200

    delegate: Column {
        Item {
            NodeCard {
                width: cardList.cellWidth - 8
                height: 192
                radius: 12
            }
        }
    }

    model: ListModel {
        ListElement {
            nodeName: "Node 1"
            address: "example.arktoria.org"
            protocol: "vmess"
            routings: "default_routings"
            latency: "114ms"
        }

        ListElement {
            nodeName: "Node 2"
            address: "example.arktoria.org"
            protocol: "vmess"
            routings: "default_routings"
            latency: "114ms"
        }

        ListElement {
            nodeName: "Node 3"
            address: "example.arktoria.org"
            protocol: "vmess"
            routings: "default_routings"
            latency: "114ms"
        }

        ListElement {
            nodeName: "Node 4"
            address: "example.arktoria.org"
            protocol: "vmess"
            routings: "default_routings"
            latency: "114ms"
        }
    }
    ScrollBar.vertical: ControlsBasic.ScrollBar {
        policy: ScrollBar.AsNeeded
        smooth: true
    }
}
