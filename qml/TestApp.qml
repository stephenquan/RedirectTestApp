import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ArcGIS.AppFramework.Proto 1.0

Page {
    id: page

    QtObject {
        id: properties

        property url defaultUrl: "http://localhost:1234/api/debug301?a=1&b=2"
        property int error: 0
        property string errorString: ""
        property int statusCode: 0
        property var headers: null
        property var response: null
        property NetworkReply networkReply
    }

    QtObject {
        id: styles

        property double textPointSize: 10
        property color headerBackgroundColor: "#e0e0e0"
        property color headerTextColor: "black"
        property double headerTextPointSize: 12
        property color inactiveAddressBackgroundColor: "#f0f0f0"
        property color inactiveAddressBorderColor: "#e0e0e0"
        property color activeAddressBorderColor: "#8080ff"
        property color activeAddressBackgroundColor: "white"
        property double heading1TextPointSize: 14
        property bool heading1TextBold: false
        property color frameBackgroundColor: "#f0f0f0"
        property color frameBorderColor: "#e0e0e0"
        property double frameBorderRadius: 5
        property double iconSize: 32
        property double smallIconSize: 16
    }

    header: Frame {
        background: Rectangle {
            color: styles.headerBackgroundColor
        }

        Text {
            text: app.title
            font.pointSize: styles.headerTextPointSize
            color: styles.headerTextColor
        }
    }

    Flickable {
        id: flickable

        anchors.fill: parent
        anchors.margins: 10

        contentWidth: columnLayout.width
        contentHeight: columnLayout.height
        clip: true

        ColumnLayout {
            id: columnLayout

            width: flickable.width
            spacing: 20

            RowLayout {
                Layout.fillWidth: true

                ComboBox {
                    id: methodComboBox

                    model: [ 'GET', 'POST' ]
                    font.pointSize: styles.textPointSize
                }

                Frame {
                    Layout.fillWidth: true

                    background: Rectangle {
                        color: addressTextInput.activeFocus
                               ? styles.activeAddressBackgroundColor
                               : styles.inactiveAddressBackgroundColor
                        border.color: addressTextInput.activeFocus
                                      ? styles.activeAddressBorderColor
                                      : styles.inactiveAddressBorderColor
                    }

                    TextInput {
                        id: addressTextInput

                        width: parent.width

                        text: properties.defaultUrl
                        font.pointSize: styles.textPointSize
                        validator: RegExpValidator {
                            regExp: /..*/
                        }
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                        onAccepted: navigate()
                    }
                }
            }

            Frame {
                Layout.fillWidth: true

                visible: methodComboBox.currentText === 'POST'

                background: Rectangle {
                    color: styles.frameBackgroundColor
                    border.color: styles.frameBorderColor
                    radius: styles.frameBorderRadius
                }

                ColumnLayout {
                    width: parent.width

                    Text {
                        text: qsTr("Params")
                        font.pointSize: styles.heading1TextPointSize
                        font.bold: styles.heading1TextBold
                    }

                    ListView {
                        id: listView

                        Layout.fillWidth: true
                        Layout.preferredHeight: contentHeight

                        model: bodyParams

                        delegate: RowLayout {
                            width: listView.width
                            spacing: 0

                            Frame {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                RowLayout {
                                    width: parent.width

                                    Text {
                                        Layout.fillWidth: true

                                        text: key
                                        font.pointSize: styles.textPointSize
                                        visible: !keyTextInput.visible

                                        MouseArea {
                                            anchors.fill: parent

                                            enabled: !header

                                            onClicked: {
                                                keyTextInput.text = key;
                                                keyTextInput.forceActiveFocus();
                                            }
                                        }
                                    }

                                    TextInput {
                                        id: keyTextInput

                                        Layout.fillWidth: true
                                        font.pointSize: styles.textPointSize

                                        visible: activeFocus

                                        onAccepted: {
                                            key = text;
                                        }
                                    }
                                }
                            }

                            Frame {
                                Layout.fillWidth: true
                                Layout.preferredWidth: 100
                                RowLayout {
                                    width: parent.width

                                    Text {
                                        Layout.fillWidth: true

                                        text: value
                                        font.pointSize: styles.textPointSize
                                    }

                                    TextInput {
                                        Layout.fillWidth: true

                                        visible: !header
                                    }
                                }
                            }

                            Item {
                                Layout.preferredWidth: styles.smallIconSize
                                Layout.preferredHeight: styles.smallIconSize

                                Image {
                                    anchors.fill: parent

                                    visible: !header
                                    source: "Images/trash-32.svg"
                                    sourceSize: Qt.size(width, height)

                                    MouseArea {
                                        anchors.fill: parent

                                        onClicked: removeParam(index)
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Frame {
                Layout.fillWidth: true

                visible: properties.error

                background: Rectangle {
                    color: styles.frameBackgroundColor
                    border.color: styles.frameBorderColor
                    radius: styles.frameBorderRadius
                }

                ColumnLayout {
                    width: parent.width

                    Text {
                        Layout.fillWidth: true
                        text: qsTr("Errors")
                        font.pointSize: styles.heading1TextPointSize
                        font.bold: styles.heading1TextBold
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }

                    Text {
                        Layout.fillWidth: true
                        text: qsTr("%1 - %2")
                                  .arg(properties.error)
                                  .arg(properties.errorString)
                        font.pointSize: styles.textPointSize
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }
                }
            }

            Frame {
                Layout.fillWidth: true

                visible: properties.statusCode

                background: Rectangle {
                    color: styles.frameBackgroundColor
                    border.color: styles.frameBorderColor
                    radius: styles.frameBorderRadius
                }

                ColumnLayout {
                    width: parent.width

                    Text {
                        Layout.fillWidth: true
                        text: qsTr("Status")
                        font.pointSize: styles.heading1TextPointSize
                        font.bold: styles.heading1TextBold
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }

                    Text {
                        Layout.fillWidth: true
                        text: properties.statusCode
                        font.pointSize: styles.textPointSize
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }
                }
            }

            Frame {
                Layout.fillWidth: true

                visible: Object.keys(properties.headers || ({}) ).length > 0

                background: Rectangle {
                    color: styles.frameBackgroundColor
                    border.color: styles.frameBorderColor
                    radius: styles.frameBorderRadius
                }

                ColumnLayout {
                    width: parent.width

                    Text {
                        Layout.fillWidth: true
                        text: qsTr("Headers")
                        font.pointSize: styles.heading1TextPointSize
                        font.bold: styles.heading1TextBold
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }

                    Text {
                        Layout.fillWidth: true
                        text: JSON.stringify(properties.headers, undefined, 2)
                        font.pointSize: styles.textPointSize
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }
                }
            }

            Frame {
                Layout.fillWidth: true

                visible: properties.response

                background: Rectangle {
                    color: styles.frameBackgroundColor
                    border.color: styles.frameBorderColor
                    radius: styles.frameBorderRadius
                }

                ColumnLayout {
                    width: parent.width

                    Text {
                        Layout.fillWidth: true
                        text: qsTr("Response")
                        font.pointSize: styles.heading1TextPointSize
                        font.bold: styles.heading1TextBold
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }

                    Text {
                        Layout.fillWidth: true
                        text: properties.response || ""
                        font.pointSize: styles.textPointSize
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }
                }
            }

        }
    }

    NetworkAccessManager {
        id: networkAccessManager

        /*
        onFinished: {
            properties.error = reply.error;
            properties.errorString = reply.errorString;
            properties.statusCode = reply.statusCode;
            properties.headers = reply.headers;
            properties.response = reply.readAll();
            console.log("response: ", properties.response);
        }
        */
    }

    Connections {
        target: properties.networkReply

        onFinished: {
            properties.error = properties.networkReply.error;
            properties.errorString = properties.networkReply.errorString;
            properties.statusCode = properties.networkReply.statusCode;
            properties.headers = properties.networkReply.headers;
            properties.response = properties.networkReply.readAll();
        }
    }

    ListModel {
        id: bodyParams
        ListElement { key: "Key"; value: "Value"; header: true }
        ListElement { key: "c"; value: "3" }
        ListElement { key: "d"; value: "4" }

        Component.onCompleted: blankParamAtEnd()
    }

    function removeParam(index) {
        bodyParams.remove(index);
        blankParamAtEnd();
    }

    function blankParamAtEnd() {
        let last = bodyParams.get(bodyParams.count - 1);
        if (!last.key || !last.value) {
            bodyParams.append( { key : "", value: "" } );
        }
    }

    function navigate() {
        let url = addressTextInput.text;

        properties.error = 0;
        properties.errorString = null;
        properties.statusCode = 0;
        properties.headers = null;
        properties.response = null;

        let params = { };
        for (let i = 0; i < bodyParams.count; i++) {
            let { key, value, header } = bodyParams.get(i);
            if (header) {
                continue;
            }
            params[key] = value;
        }

        switch (methodComboBox.currentText) {
        case "GET":
            properties.networkReply = networkAccessManager.get(url);
            break;

        default:
            console.log( "params: ", JSON.stringify( params ) );
            properties.networkReply = networkAccessManager.post(url, params);
            break;

        }
    }

}
