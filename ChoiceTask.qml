import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id: choiceTask
    width: 200; height: 100

    border.color: "black"
    color: "transparent"

    signal taskTrans(var num)

    MyCheckBox{
        id: circleTask
        x: 0; y: 0
        cBtext: "circle"
        onCBclicked: {
            if(checked) {
                taskTrans(1)
                scoutTask.transCheckState(!checked)
                eightAndCircleTask.transCheckState(!checked)
                avoidanceTask.transCheckState(!checked)
                rest.transCheckState(!checked)
            }
        }
    }

    MyCheckBox{
        id: eightAndCircleTask
        x: 0; y: 40
        cBtext: "scout"
        onCBclicked: {
            if(checked) {
                taskTrans(2)
                scoutTask.transCheckState(!checked)
                circleTask.transCheckState(!checked)
                avoidanceTask.transCheckState(!checked)
                rest.transCheckState(!checked)
            }
        }
    }

    MyCheckBox{
        id: scoutTask
        x: 0; y: 80
        cBtext: "scout"
        onCBclicked: {
            if(checked) {
                taskTrans(3)
                eightAndCircleTask.transCheckState(!checked)
                circleTask.transCheckState(!checked)
                avoidanceTask.transCheckState(!checked)
                rest.transCheckState(!checked)
            }
        }
    }

    MyCheckBox{
        id: avoidanceTask
        x: 100; y: 0
        cBtext: "avoidance"
        onCBclicked: {
            if(checked) {
                taskTrans(4)
                eightAndCircleTask.transCheckState(!checked)
                circleTask.transCheckState(!checked)
                scoutTask.transCheckState(!checked)
                rest.transCheckState(!checked)
            }
        }
    }

    MyCheckBox{
        id: rest
        x: 100; y: 40
        cBtext: "rest"
        onCBclicked: {
            if(checked) {
                taskTrans(5)
                eightAndCircleTask.transCheckState(!checked)
                circleTask.transCheckState(!checked)
                scoutTask.transCheckState(!checked)
                avoidanceTask.transCheckState(!checked)
            }
        }
    }
}
