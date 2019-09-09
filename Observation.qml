import QtQuick 2.0

Rectangle {
    id: opservation
    width: 200; height: 300

    property alias latText: lat.text
    property alias logText: log.text
    property alias speedText: speed.text
    property alias yawText: yaw.text
    property alias pingerText: pinger.text
    property alias modeText: mode.text
    property alias radioStateText: radioState.text
    property alias radioTaskText: radioTask.text
    property alias doorText: door.text
    property alias colorText: color.text
    property alias errorYawText: errorYaw.text
    property alias targetXText: targetX.text
    property alias targetYText: targetY.text
    property alias npText: np.text
    property alias xpText: xp.text
    property alias seText: se.text
    property alias cflagText: cflag.text
    property alias pflagText: pflag.text
    property alias driverReturnText: driverReturn.text

    TextInputOne{
        id: lat
        textName: "lat"
        x: 0; y: 0
    }
    TextInputOne{
        id: log
        textName: "log"
        x: 0; y: 30

    }
    TextInputOne{
        id: speed
        textName: "speed"
        x: 0; y: 60

    }
    TextInputOne{
        id: yaw
        textName: "yaw"
        x: 0; y: 90

    }
    TextInputOne{
        id: pinger
        textName: "pinger"
        x: 0; y: 120

    }
    TextInputOne{
        id: mode
        textName: "mode"
        x: 0; y: 150

    }
    TextInputOne{
        id: radioState
        textName: "radioState"
        x: 0; y: 180
    }
    TextInputOne{
        id: radioTask
        textName: "radioTask"
        x: 0; y: 210
    }
    TextInputOne{
        id: door
        textName: "进出门序号"
        x: 0; y: 240
    }
    TextInputOne{
        id: color
        textName: "颜色序列"
        x: 150; y: 0
    }
    TextInputOne{
        id: errorYaw
        textName: "errorYaw"
        x: 150; y: 30
    }
    TextInputOne{
        id: targetX
        textName: "targetX"
        x: 150; y: 60
    }
    TextInputOne{
        id: targetY
        textName: "targetY"
        x: 150; y: 90
    }
    TextInputOne{
        id: np
        textName: "Np"
        x: 150; y: 120
    }
    TextInputOne{
        id: xp
        textName: "Xp"
        x: 150; y: 150
    }
    TextInputOne{
        id: se
        textName: "SE"
        x: 150; y: 180
    }
    TextInputOne{
        id: cflag
        textName: "Cflag"
        x: 150; y: 210
    }
    TextInputOne{
        id: pflag
        textName: "Pflag"
        x: 150; y: 240
    }
    TextInputOne{
        id: driverReturn
        textName: "driverReturn"
        x: 0; y: 300
    }
}
