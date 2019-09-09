import QtQuick 2.0
import QtQuick.Controls 2.1

Rectangle {
    id: rect
    width: 100; height: 20
    color: "black"
    property string fontFamily: "Monaco"
    property color fontColor: "white"
    property double fontSize: 13

    property string cBtext: "定时发送" // 文本内容
    property real textLeftMargin: 8 // 用来调整文本的位置

    function transCheckState(checked){
        checkBox.checked = checked;
    }

    signal cBclicked(var checked)

    CheckBox {
        id: checkBox
        opacity: rect.opacity
        width: 85; height: 20
        text: cBtext
        indicator: Image{
            height: 13
            width: 13
            anchors.verticalCenter: parent.verticalCenter
            source: parent.checked ? "image/checked.png" : "image/unchecked.png"
        }

        contentItem: Text {
            anchors.left: parent.left; anchors.leftMargin: textLeftMargin
            text: parent.text
            color: fontColor
            font.family: fontFamily
            font.pixelSize: fontSize
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: cBclicked(checkBox.checked)
    }

}
