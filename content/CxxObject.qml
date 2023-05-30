import QtQuick 2.15

Item {
    id: cxxObject
    property var acrossGroupModel: (typeof cxxGroupModel
                                    !== "undefined") ? cxxGroupModel : Qt.createQmlObject(`
                                                                                          import QtQml.Models

                                                                                          ListModel {
                                                                                          ListElement {
                                                                                          name: "Group 1"
                                                                                          modifiedAt: "Updated on 5 - 18"
                                                                                          type: "Basic"
                                                                                          subType: "Raw"
                                                                                          count: 120
                                                                                          }

                                                                                          ListElement {
                                                                                          name: "Group 2"
                                                                                          modifiedAt: "Updated on 5 - 18"
                                                                                          type: "Subscription"
                                                                                          subType: "SIP008"
                                                                                          count: 20
                                                                                          }

                                                                                          ListElement {
                                                                                          name: "Group 3"
                                                                                          modifiedAt: "Updated on 5 - 18"
                                                                                          type: "Subscription"
                                                                                          subType: "OOCv1"
                                                                                          count: 5
                                                                                          }

                                                                                          ListElement {
                                                                                          name: "Group 4"
                                                                                          modifiedAt: "Updated on 5 - 18"
                                                                                          type: "Subscription"
                                                                                          subType: "Raw"
                                                                                          count: 6
                                                                                          }
                                                                                          }
                                                                                          `,
                                                                                          cxxObject)
}
