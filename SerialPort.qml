import QtQuick 2.0
import io.serialport 1.0

Rectangle {
    id: serialPort
    width: 300; height: 200

    property alias myObservation: myObservation
    property alias comm2: comm2
    property double currentX
    property double currentY
    property string currentYaw

    signal coordinate()

    MyButton{
        id: handleBtn
        x: 0; y: 0
        btnText1: "打开遥控串口"
        bTntext2: "关闭遥控串口"
        onBtnClicked: {
            if(handleBtn.state == "active"){
                handleMessage.opacity = 1
                comm1.startSlave(comm1.portName, comm1.response);
            }
            else{
                mytimer.stop()
                comm1.closeSlave()
            }
        }
    }

    SerialPortMessage{
        id: handleMessage
        x: 0; y: 50
        onSendClicked: comm1.sendResponse()
    }

    MyButton{
        id: radioBtn
        x: 200; y: 0
        btnText1: "打开数传串口"
        bTntext2: "关闭数传串口"
        onBtnClicked: {
            if(radioBtn.state == "active"){
                radioMessage.opacity = 1
                comm2.startSlave(comm2.portName, comm2.response)
            }
            else{
                comm2.closeSlave()
            }
        }
    }

    SerialPortMessage{
        id: radioMessage
        x: 0; y: 400
        commName: "COM4"
        onSendClicked: comm2.sendResponse()
    }

    MyCheckBox{
        id: autoSend
        x: 100; y: 0
        onCBclicked: {
            if(checked) mytimer.start()
            else mytimer.stop()
        }
    }

    MyCheckBox{
        id: remote
        x: 800; y: 0
        cBtext: "remote"
        onCBclicked: {
            if(checked) autonomous.transCheckState(!checked)
        }
    }

    MyCheckBox{
        id: autonomous
        x: 800; y: 50
        cBtext: "autonomous"
        onCBclicked: {
            if(checked) remote.transCheckState(!checked)
        }
    }

    Comm{
        id: comm1
        portName: "COM3"
        response: handleMessage.mySendText.text
    }

    Comm{
        id: comm2
        portName: "COM4"
    }

    Item {
        id: moshi

        Connections{
            target: remote
            onCBclicked: {
                if(checked){
                    comm1.remoteToAutonomous(1)
                }else comm1.remoteToAutonomous(0)
            }
        }

        Connections{
            target: autonomous
            onCBclicked: {
                if(checked){
                    comm1.remoteToAutonomous(2)
                }else comm1.remoteToAutonomous(0)
            }
        }
    }

    Item {
        id: handleConnect

        Connections{
            target: comm1
            onRecvMsgChanged:{
                handleMessage.myRecvText.receive(comm1.recvMsg)
                var r =  comm1.recvMsg.split('\r\n')[1]
                comm2.response = r.split('To Bottom: ')[1]
                comm2.sendResponse()
            }
        }
    }

    Item {
        id: radioConnect

        Connections{
            target: comm2
            onRecvMsgChanged:{
                radioMessage.myRecvText.receive(comm2.recvMsg)
                var msg = comm2.recvMsg

                myObservation.latText = msg.split(',')[3]
                myObservation.logText = msg.split(',')[4]
                myObservation.speedText = msg.split(',')[5]
                myObservation.yawText = msg.split(',')[6]
                myObservation.pingerText = msg.split(',')[7]
                if(msg.split(',')[8] === "A")
                    myObservation.modeText = "自主"
                else if(msg.split(',')[8] === "R")
                    myObservation.modeText = "遥控"
                if(msg.split(',')[9] === "Y")
                    myObservation.radioStateText = "正常"
                else if(msg.split(',')[9] === "N")
                    myObservation.radioStateText = "异常"
                myObservation.radioTaskText = msg.split(',')[10]
                myObservation.doorText = msg.split(',')[11]
                myObservation.colorText = msg.split(',')[12]
                myObservation.errorYawText = msg.split(',')[13]
                myObservation.npText = msg.split(',')[16]
                myObservation.xpText = msg.split(',')[17]
                myObservation.seText = msg.split(',')[18]
                myObservation.cflagText = msg.split(',')[19]
                myObservation.pflagText = msg.split(',')[20]
                myObservation.driverReturnText = msg.split(',')[21]
            }
            onCoordinateChanged:{
                currentX = shipX
                currentY = shipY
                currentYaw = yaw
                myObservation.targetXText = pointX
                myObservation.targetYText = pointY

                coordinate()
            }
        }
    }

    Observation{
        id: myObservation
        x: 500; y: 100
    }

    ChoiceTask{
        id: myChoiceTask
        x: 1000; y: 0
        onTaskTrans: {
            if(num === 1){
                comm1.transTask("C")
            }
            if(num === 2){
                comm1.transTask("E")
            }
            if(num === 3){
                comm1.transTask("S")
            }
            if(num === 4){
                comm1.transTask("A")
            }
            if(num === 5){
                comm1.transTask("R")
            }
        }
    }

    Timer {
        id: mytimer
        interval: 500; running: false; repeat: true
        onTriggered: comm1.sendResponse()
    }
}

