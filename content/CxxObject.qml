import QtQuick 2.15

Item {
    id: cxxObject
    function createObject(url, parent) {
        let component = Qt.createComponent(url)
        if (component.status === Component.Ready)
            return component.createObject(parent)
    }

    property var acrossGroupModel: (typeof cxxGroupModel
                                    !== "undefined") ? cxxGroupModel : cxxObject.createObject(
                                                           "GroupModelExample.qml",
                                                           cxxObject)
    property var acrossNodeModel: (typeof cxxNodeModel
                                   !== "undefined") ? cxxNodeModel : cxxObject.createObject(
                                                          "NodeModelExample.qml",
                                                          cxxObject)
}
