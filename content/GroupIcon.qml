import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes

Item {
    implicitHeight: 14
    implicitWidth: 14
    width: 14
    height: 14
    antialiasing: true
    layer.enabled: true
    layer.samples: 2
    id: rect

    property int sides: 6
    property real pRotate: (sides % 2 ? 1 : (sides == 4 ? 1 / 2 : 0)) * Math.PI / 2

    Shape {
        anchors.fill: parent

        id: hexagon
        ShapePath {
            id: shapePath

            startX: rect.width / 2 * Math.cos(
                        2 * Math.PI * 0 / sides - pRotate) + rect.width / 2
            startY: rect.height / 2 * Math.sin(
                        2 * Math.PI * 0 / sides - pRotate) + rect.height / 2
            fillColor: "#cac4d0"
            strokeColor: "#cac4d0"
        }
        Instantiator {
            model: sides + 1
            onObjectAdded: (index, object) => shapePath.pathElements.push(
                               object)
            PathLine {
                x: rect.width / 2 * Math.cos(
                       2 * Math.PI * index / sides - pRotate) + rect.width / 2
                y: rect.height / 2 * Math.sin(
                       2 * Math.PI * index / sides - pRotate) + rect.height / 2
            }
        }
    }
}
