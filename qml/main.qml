import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Window {
    id: app

    visible: true
    width: 640
    height: 480
    title: qsTr("Redirect Test App")

    StackView {
        id: stackView

        anchors.fill: parent

        initialItem: TestApp {
        }
    }

}
