import "../typescripts/help.js" as HelpJS
import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

CardBox {
    id: aboutItem

    property int fontSize: 14
    property bool isDev: false

    implicitWidth: 680
    implicitHeight: 320

    RowLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 4
        spacing: acrossConfig.itemSpacing * 2

        Item {
            property int logoBasicSize: 128

            Layout.preferredWidth: logoBasicSize * 2
            Layout.fillHeight: true

            SVGBox {
                id: logoImage

                anchors.centerIn: parent
                sourceWidth: parent.logoBasicSize
                sourceHeight: parent.logoBasicSize
                source: HelpJS.getLogo(acrossConfig.iconStyle)

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: HelpJS.getLogo(acrossConfig.iconStyle, aboutItem, logoImage)
                }

            }

        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            GridLayout {
                anchors.fill: parent
                columns: 2
                rowSpacing: acrossConfig.itemSpacing * 2
                columnSpacing: acrossConfig.itemSpacing * 2

                Label {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    text: "ACross"
                    font.pointSize: Math.round(fontSize * 1.2)
                    color: acrossConfig.textColor
                }

                Label {
                    text: qsTr("Version")
                    color: acrossConfig.textColor
                }

                Label {
                    Layout.fillWidth: true
                    text: acrossConfig.guiVersion
                    color: acrossConfig.highlightColor
                }

                Label {
                    text: qsTr("Build Info")
                    color: acrossConfig.textColor
                }

                Label {
                    Layout.fillWidth: true
                    text: acrossConfig.buildInfo
                    color: acrossConfig.textColor
                }

                Label {
                    text: qsTr("Build Time")
                    color: acrossConfig.textColor
                }

                Label {
                    Layout.fillWidth: true
                    text: acrossConfig.buildTime
                    color: acrossConfig.textColor
                }

                Label {
                    text: qsTr("Source Code")
                    color: acrossConfig.textColor
                }

                URLBox {
                    Layout.fillWidth: true
                    urlText: acrossConfig.sourceCodeURL
                }

                Label {
                    text: qsTr("Licenses")
                    color: acrossConfig.textColor
                }

                URLBox {
                    Layout.fillWidth: true
                    urlText: acrossConfig.licenseURL
                }

                Label {
                    text: qsTr("Extra Info")
                    color: acrossConfig.textColor
                }

                TextAreaBox {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: acrossConfig.extraInfo
                    color: acrossConfig.textColor
                    wrapMode: Text.WordWrap
                    readOnly: true
                    selectByMouse: true
                    selectedTextColor: acrossConfig.highlightTextColor
                    selectionColor: acrossConfig.highlightColor
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    spacing: acrossConfig.itemSpacing * 2

                    Item {
                        Layout.fillWidth: true
                    }

                    URLBox {
                        id: newVersionText

                        visible: false
                        urlText: acrossConfig.releaseURL
                        color: acrossConfig.styleColor
                    }

                    ButtonBox {
                        text: qsTr("Report Bugs")
                        basicColor: acrossConfig.warnColor
                        basicState: "WarnState"
                        onClicked: Qt.openUrlExternally(acrossConfig.reportURL)
                    }

                    ButtonBox {
                        text: qsTr("Check Update")
                        onClicked: {
                            acrossConfig.checkUpdate();
                        }
                    }

                    Connections {
                        function onUpdatedChanged(versionInfo) {
                            newVersionText.visible = true;
                            newVersionText.text = versionInfo;
                        }

                        target: acrossConfig
                    }

                }

            }

        }

    }

}
