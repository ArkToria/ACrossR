import QtQml.Models

ListModel {
    ListElement {
        name: "Node 1"
        address: "example.arktoria.org"
        protocol: "vmess"
        routings: "default_routings"
        latency: 114
    }

    ListElement {
        name: "Node 2"
        address: "example.arktoria.org"
        protocol: "vmess"
        routings: "default_routings"
        latency: 114
    }

    ListElement {
        name: "Node 3LongLongLongLongLongLongLongLong"
        address: "example.arktoria.orgLongLongLongLongLongLongLongLongLongLongLongLongLongLongLong"
        protocol: "vmessLongLongLongLongLongLongLongLongLongLongLongLongLong"
        routings: "default_routingsLongLongLongLongLongLongLongLongLongLongLongLong"
        latency: 2147483647
    }

    ListElement {
        name: "Node 4"
        address: "example.arktoria.org"
        protocol: "vmess"
        routings: "default_routings"
        latency: 114
    }
}
