import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 1366
    height: 800
    title: qsTr("Hello World")

    SerialPort{
        id: mySerialPort
        x: 20; y: 20
        onCoordinate: {
            myChart.pathseries.append(mySerialPort.currentX,mySerialPort.currentY)
            myChart.boatX = mySerialPort.currentX
            myChart.boatY = mySerialPort.currentY
            myChart.boatYaw = mySerialPort.currentYaw
        }
    }

    MyChart{
        id: myChart
        x: 800; y: 150
    }
}
