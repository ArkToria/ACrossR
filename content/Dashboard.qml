import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    clip: true
    id: dashboard
    width: 220
    height: 220
    property var content: null
    function setContent(qmlCode) {
        if (content !== null) {
            content.destory()
        }
        content = Qt.createQmlObject(qmlCode, dashboard, "dashboardContent")
    }
    Component.onCompleted: {
        let qmlCode = `import QtQuick 2.15
        import QtQuick.Controls 2.15
        import QtQuick.Shapes 1.5

        Rectangle {
        id: rectangle
        width: 220
        height: 220
        color: "#dadce0"

        Shape {
        ShapePath {
        startX: 28
        startY: 0

        PathLine {
        x: 56
        y: 56
        }
        PathLine {
        x: 0
        y: 56
        }
        PathLine {
        x: 28
        y: 0
        }

        fillColor: "#9aa0a6"
        strokeColor: "#9aa0a6"
        }
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 40

        width: 56
        height: 56
        }

        Rectangle {
        width: 56
        height: 56
        color: "#9aa0a6"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 40
        anchors.bottomMargin: 40
        radius: 3
        }

        Shape {
        id: circle

        ShapePath {
        startX: 28
        startY: 0
        PathArc {
        x: 28
        y: 56
        radiusX: 28
        radiusY: 28
        useLargeArc: true
        }
        PathArc {
        x: 28
        y: 0
        radiusX: 28
        radiusY: 28
        useLargeArc: true
        }
        fillColor: "#9aa0a6"
        strokeColor: "#9aa0a6"
        }

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 40
        anchors.bottomMargin: 40

        width: 56
        height: 56
        }
        }
        `
        dashboard.setContent(qmlCode)
    }
}
