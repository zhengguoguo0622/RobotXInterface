import QtQuick 2.0
import QtCharts 2.3
import QtQuick.Controls 2.0

Item {
    id: mapChart
    width: 500; height: 500
    clip: true

    // 字体属性
    property real fontpixelSize: 10
    property string fontfamily: "Monaco"

    // 坐标轴属性
    property real axisX_min: 0
    property real axisX_max: 800
    property real axisY_min: 0
    property real axisY_max: 800

    property double boatX: 00
    property double boatY: 00
    property double boatYaw: 30

    property alias pathseries: pathseries

    ChartView{
        id: myChart
        anchors.fill: parent
        anchors.centerIn: parent
        antialiasing: true          //是否抗锯齿
        legend.visible: true        // 线 可视
        backgroundColor: "white"

        margins.top: 0
        margins.bottom: 0
        margins.left: 0
        margins.right: 0

        ValueAxis{
            id: axisX
            min: axisX_min
            max: axisX_max
            lineVisible: true
            labelsVisible: true
            gridVisible: true
            labelsColor: "red"
            labelsFont.family: fontfamily
            labelsFont.pixelSize: fontpixelSize
        }
        ValueAxis{
            id: axisY
            min: axisY_min
            max: axisY_max
            lineVisible: true
            labelsVisible: true
            gridVisible: true
            labelsColor: "red"
            labelsFont.family: fontfamily
            labelsFont.pixelSize: fontpixelSize
        }

        
        
        LineSeries{
            id: pathseries
            axisX: axisX
            axisY: axisY
            useOpenGL: true        // 使char 添加点的时候更加流畅
        }
    }

    Image {
        id: boat
        source: "image/boat.png"
        x: boatX; y: boatY
        width: 30; height: 30
        rotation: boatYaw
    }
}
