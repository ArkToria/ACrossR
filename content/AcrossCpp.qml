import QtQuick 2.15

Item {
    id: cxxObject
    function createObject(url, parent) {
        let component = Qt.createComponent(url)
        if (component.status === Component.Ready)
            return component.createObject(parent)
    }
    function testObject(o, default_object){
        return (typeof o !== "undefined") ? o : default_object
    }

    property var groupModel: testObject(acrossCxx.groupModel,cxxObject.createObject(
                                                           "GroupModelExample.qml",
                                                           cxxObject))
    property var nodeModel:  testObject(acrossCxx.nodeModel,cxxObject.createObject(
                                                          "NodeModelExample.qml",
                                                          cxxObject))
}
