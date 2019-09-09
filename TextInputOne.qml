import QtQuick 2.0

Rectangle {
    id: textInputRect
    width: 80
    height: 20
    color: "transparent"
    border.color: "black"
    opacity: 1

    // textInput属性
    property alias text: textInput.text
    property alias textName: myText.text
    property alias fontcolor: textInput.color
    property int fontsize: 13
    property real textLeftMargin: 0

    signal inputAccepted

    Text {
        id: myText
        anchors.right: parent.left
        anchors.rightMargin: 10
    }
    TextInput{
        id: textInput
        opacity: textInputRect.opacity
        enabled: opacity == 1 ? true : false
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 5
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        selectByMouse: true
        selectedTextColor: "red"
        font.family: "monaco"
        font.pixelSize: fontsize
        color: "black"
        onAccepted: inputAccepted()
    }
}
