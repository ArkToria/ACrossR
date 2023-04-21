

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.4
import QtQuick.Controls 6.4
import ACross
import Qt5Compat.GraphicalEffects
import QtQuick.Layouts 6.3

Rectangle {
    id: rectangle
    width: Constants.width
    height: Constants.height

    color: Colors.background

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        HomePageDrawer {
            Layout.leftMargin: 1
            Layout.fillHeight: true
        }
        ColumnLayout {
            spacing: 8
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillHeight: true

            HomePageCard {
                Layout.topMargin: 8
                Layout.rowSpan: 6
                Layout.rightMargin: 8
                Layout.leftMargin: 8
                Layout.fillWidth: true
            }
            RowLayout {
                id: rowLayout1
                width: 100
                height: 100
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                spacing: 5
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.fillWidth: false
                layoutDirection: Qt.LeftToRight

                CustomButton {
                    id: createButton
                    text: qsTr("Create")
                    imageSource: "../misc/icons/dark/add_circle.svg"
                    width: 100
                    height: 40
                }
                CustomButton {
                    id: copyButton
                    text: qsTr("Copy")
                    imageSource: "../misc/icons/dark/copy.svg"
                    width: 100
                    height: 40
                }
                CustomButton {
                    id: qrcodeButton
                    text: qsTr("QRCode")
                    imageSource: "../misc/icons/dark/qr_code.svg"
                    width: 100
                    height: 40
                }
                CustomButton {
                    id: searchbutton
                    text: qsTr("Search")
                    imageSource: "../misc/icons/dark/search.svg"
                    width: 100
                    height: 40
                }
                CustomButton {
                    id: locatebutton
                    text: qsTr("Locate")
                    imageSource: "../misc/icons/dark/location.svg"
                    width: 100
                    height: 40
                }
            }
            CardGrid {
                Layout.rightMargin: 0
                Layout.leftMargin: 8
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
