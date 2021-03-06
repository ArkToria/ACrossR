import Arktoria.ACross
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: popNotifyControl

    implicitWidth: 320
    implicitHeight: popNotifyListView.count >= 3 ? 100 * 3 : 100 * popNotifyListView.count
    clip: true
    visible: false

    ListModel {
        id: popNotifyModel
    }

    ListView {
        id: popNotifyListView

        anchors.fill: parent
        verticalLayoutDirection: ListView.BottomToTop
        model: acrossNotifications
        move: add

        onCountChanged: {
            if (count === 0)
                popNotifyControl.visible = false;
            else
                popNotifyControl.visible = true;
        }

        delegate: PopMessageBox {
            title: model.title
            message: model.message
            from: model.from
            value: model.value
            to: model.to
        }

        add: Transition {
            NumberAnimation {
                properties: "x,y"
                easing.type: Easing.OutBack
            }

        }

    }
    function numAnimation(parent) {
        return Qt.createQmlObject("import QtQuick; NumberAnimation {}", parent);
    }

    function notify(title = "", message = "", from = 0.0, to = 1.0, value = 0.0, duration = -1) {
        let notification = acrossNotifications.append(title,message,0.0,1.0,0.0);

        if(duration != -1) {
            let animation = new numAnimation(notification);
            animation.from = from;
            animation.to = to;
            animation.duration = duration;
            animation.target = notification;
            animation.property = "value";
            animation.start();

            function removeNotification() {
                acrossNotifications.remove(notification.index);
            }

            animation.finished.connect(removeNotification);
        }

        return notification;
    }
}
