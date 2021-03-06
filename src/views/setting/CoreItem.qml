/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

import Arktoria.ACross
//import QtQuick.Dialogs
import Qt.labs.platform
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property int fontSize: 14
    property int keyBoxWidth: 72

    FileDialog {
        id: coreFileDialog

        title: qsTr("Select V2Ray Core Path")
        nameFilters: ["Select V2Ray core (v2ray*)", "All files (*)"]
        onAccepted: {
            acrossConfig.corePath = currentFile;
        }
    }

    FolderDialog {
        id: assetsFileDialog

        title: qsTr("Select Assets Directory")
        onAccepted: {
            acrossConfig.assetsPath = currentFolder;
        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: acrossConfig.itemSpacing * 4
        columns: 5
        rowSpacing: acrossConfig.itemSpacing * 2
        columnSpacing: acrossConfig.itemSpacing * 2

        Label {
            Layout.columnSpan: 5
            text: qsTr("Core Configuration")
            font.pointSize: Math.round(fontSize * 1.2)
            color: acrossConfig.textColor
        }

        Label {
            Layout.preferredWidth: keyBoxWidth
            text: qsTr("Log Level")
            color: acrossConfig.textColor
        }

        DropDownBox {
            Layout.fillWidth: true
            Layout.columnSpan: 4
            model: ["current", "debug", "info", "warning", "error"]
            displayText: acrossConfig.logLevel
            onEditTextChanged: {
                acrossConfig.logLevel = editText;
            }
        }

        Label {
            Layout.preferredWidth: keyBoxWidth
            text: qsTr("Core path")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 3
            placeholderText: acrossConfig.corePath === "" ? qsTr("Enter V2Ray Core Executable Path Here") : acrossConfig.corePath
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight
            text: qsTr("Select")
            onClicked: {
                coreFileDialog.open();
            }
        }

        Label {
            Layout.preferredWidth: keyBoxWidth
            text: qsTr("Assets path")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            Layout.fillWidth: true
            Layout.columnSpan: 3
            placeholderText: acrossConfig.assetsPath === "" ? qsTr("Enter GeoIP and GeoSite Directory Here") : acrossConfig.assetsPath
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight
            text: qsTr("Select")
            onClicked: {
                assetsFileDialog.open();
            }
        }

        Label {
            Layout.preferredWidth: keyBoxWidth
            text: qsTr("Core Info")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: core_info

            Layout.fillWidth: true
            Layout.columnSpan: 3
            text: acrossConfig.coreName + acrossConfig.coreVersion
            readOnly: true
            color: acrossConfig.textColor
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight
            text: qsTr("Check")
            onClicked: {
                core_info.text = acrossConfig.coreName + acrossConfig.coreVersion;
            }
        }

        Label {
            text: qsTr("Enable API")
            color: acrossConfig.textColor
            font.pointSize: fontSize
        }


        SwitchBox {
            id: apiSwitch

            Layout.alignment: Qt.AlignRight
            checked: acrossConfig.apiEnable
            onCheckedChanged: {
                acrossConfig.apiEnable = checked;
                if (checked)
                    coreItemCard.implicitHeight = 320;
                else
                    coreItemCard.implicitHeight = 280;
            }
        }

        Label {
            text: qsTr("Test Result")
            color: acrossConfig.textColor
        }

        TextFieldBox {
            id: testResult

            Layout.fillWidth: true
            readOnly: true
        }

        ButtonBox {
            Layout.alignment: Qt.AlignRight
            text: qsTr("Test")
            enabled: apiSwitch.checked
            onClicked: {
                // sync input information
                acrossConfig.testAPI();
            }

            Connections {
                function onApiStatsChanged(stats) {
                    if (stats && acrossConfig.apiResultText === "") {
                        testResult.text = qsTr("Success");
                        testResult.color = acrossConfig.styleColor;
                    } else {
                        testResult.text = acrossConfig.apiResultText;
                        testResult.color = acrossConfig.warnColor;
                    }
                }

                target: acrossConfig
            }

        }

    }

}
