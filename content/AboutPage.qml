
/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.4
import QtQuick.Controls 6.4
import ACross

Rectangle {
    width: Constants.width
    height: Constants.height

    color: Colors.background

    Text {
        text: qsTr("Hello ACross") + " 05"
        anchors.centerIn: parent
        font.family: Constants.largeFont.family
        font.pixelSize: Constants.largeFont.pixelSize
        color: "white"
    }
}
