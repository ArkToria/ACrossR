import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ACross

Item {
    implicitHeight: 96
    implicitWidth: 120

    function getLogo(style) {
        if (style === "light") {
            return "qrc:/misc/design/logo_pure.svg"
        } else {
            return "qrc:/misc/design/logo.svg"
        }
    }

    ColumnLayout {
        anchors.fill: parent

        SVGBox {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

            sourceWidth: 64
            sourceHeight: 64
            source: getLogo(acrossConfig.iconStyle)
        }

        Label {
            text: "ACross"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

            font.pixelSize: 24
            color: acrossConfig.textColor
        }
    }
}
