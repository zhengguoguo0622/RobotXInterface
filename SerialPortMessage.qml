import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import io.serialport 1.0

Rectangle{
    id: serialportMessage
    width: 430; height: 200
    color: Qt.rgba(255, 0, 0, .4)
    border.color: Qt.rgba(255, 0, 0, .4)
    border.width: 2
    opacity: 0
    radius: 4
    x: -380
    y: 230

    // 串口信息
    property string commName: "COM3"
    property string buadRate: "9600"
    property string dataBits: "Data8"
    property string stopBits: "OneStop"
    property string parity: "NoParity"

    //字体
    property string fontfamily: "Monaco"
    property color fontcolor: "white"
    property real fontpixelSize: 13

    property alias mySendText: sendText
    property alias myRecvText: recvScreen

    signal sendClicked()

    states:  State {
        name: "active"
        PropertyChanges {target: serialportMessage; opacity: 1; x: 2}
    }

    transitions: [
        Transition {
            from: "";  to: "active"; reversible: false
            NumberAnimation{properties: "opacity, x"; duration: 100; easing.type: Easing.Linear}
        },
        Transition {
            from: "active"; to: ""; reversible: false
            NumberAnimation{properties: "opacity, x"; duration: 100; easing.type: Easing.Linear}
        }
    ]

    // 选项卡中的串口信息
    Text{
        id: comInfoName
        text: "遥控串口"
        font.family: fontfamily
        color: fontcolor
        anchors.top: parent.top; anchors.topMargin: 5
        anchors.left: parent.left; anchors.leftMargin: 12
        font.pixelSize: fontpixelSize
        Rectangle{
            width: 250; height: 1
            color: fontcolor
            anchors.top: parent.bottom
            anchors.left: parent.left
        }
    }

    Grid{
        id: comInfo
        anchors.top: comInfoName.bottom; anchors.topMargin: 5
        anchors.left: parent.left; anchors.leftMargin: 12
        rows: 5
        spacing: 5
        Text{text:"串口: " + commName; color: fontcolor; font.family: fontfamily; font.pixelSize: fontpixelSize}
        Text{text:"波特率: " + buadRate; color: fontcolor; font.family: fontfamily; font.pixelSize: fontpixelSize}
        Text{text:"数据位: " + dataBits; color: fontcolor; font.family: fontfamily; font.pixelSize: fontpixelSize}
        Text{text:"停止位: " + stopBits; color: fontcolor; font.family: fontfamily; font.pixelSize: fontpixelSize}
        Text{text:"校验位: " + parity; color: fontcolor; font.family: fontfamily; font.pixelSize: fontpixelSize}
    }


    // 串口状态文本
    Text{
        id: comState
        text: "未连接..."
        font.family: fontfamily
        font.pixelSize: fontpixelSize
        color: fontcolor
        anchors.top: comInfoName.top
        anchors.left: comInfoName.right; anchors.leftMargin: 200
    }

    // 接受框
    Text{
        id: recvName
        text: "串口接受数据:"
        font.family: fontfamily
        font.pixelSize: fontpixelSize
        color: fontcolor
        anchors.top: comInfoName.bottom; anchors.topMargin: 5
        anchors.left: parent.left; anchors.leftMargin: 150
    }
    TextScreen{
        id: recvScreen
        rect_width: 250; rect_height: 20
        border.color: fontcolor
        anchors.top: recvName.bottom; anchors.topMargin: 5
        anchors.left: recvName.left
    }

    // 发送框
    Rectangle{
        id: sendScreen
        color: Qt.rgba(255, 0, 0, .4)
        border.color: fontcolor
        width: 200; height: 20
        anchors.top: recvScreen.bottom; anchors.topMargin: 10
        anchors.left: recvScreen.left
        anchors.leftMargin: 40
        TextInput{
            id: sendText
            anchors.left: parent.left; anchors.leftMargin: 2
            color: fontcolor
            font.family: fontfamily
            font.pixelSize: fontpixelSize
            selectedTextColor: "red"
            selectByMouse: true
            text: "AA 55 AF 01 "
        }
    }

    // 发送按钮
    ButtonOne{
        id: sendBtn
        btnWidth: 30; btnHeight: 20
        btnText: "发送"
        anchors.verticalCenter: sendScreen.verticalCenter
        anchors.left: sendScreen.right; anchors.leftMargin: 5
        onClicked: sendClicked()
    }

}
