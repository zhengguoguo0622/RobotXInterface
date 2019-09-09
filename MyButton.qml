import QtQuick 2.0

Rectangle {
    id: myButton
    border.color: "black"
    color: "transparent"
    width: btnWidth; height: btnHeight
    scale: 1
    state: ""

    property real btnWidth: 80
    property real btnHeight: 30

    property string btnText: btnText1
    property string btnText1: "打开遥控串口"
    property string bTntext2: "关闭遥控串口"

    signal btnClicked()

    SequentialAnimation{
        id: animations
        running: false
        NumberAnimation{
            target: myButton; property: "scale"
            from: 1; to: 0.8; duration: 100
        }
        NumberAnimation{
            target: myButton; property: "scale"
            from: 0.8; to: 1; duration: 100
        }
    }

    states: [
        State {
            name: ""
            PropertyChanges {target: myButton; btnText: btnText1}
        },
        State {
            name: "active"
            PropertyChanges {target: myButton; btnText: bTntext2}
        }
    ]

    Text {
        id: btnName
        anchors.centerIn: parent
        text: btnText
    }

    MouseArea{
        anchors.fill: parent
        enabled: parent.opacity == 1 ? true : false
        onClicked: {
            animations.start()
            if(myButton.state == "active") myButton.state = ""
            else myButton.state = "active"
            btnClicked()
        }
    }
}
