import QtQuick 2.7
import QtQuick.Window 2.2
import QtQml 2.2


Window {
    visible: true
    width: 640
    height: 480

    title: "Hello World"

    function text(msg)
    {
        rect.visible = true;
        randrec.visible = false;
        if(msg)
            rect.text = msg;
    }

    function color(msg)
    {
        rect.color = msg;
    }

    function size(msg)
    {
        rect.size = msg
    }

    function rand(msg)
    {
        rect.visible = false;
        randrec.visible = true;
        if(msg)
            randrec.speed = msg;
    }

    Rectangle{
        id: rect
        anchors.fill: parent
        objectName: "rect"
        color: "#000000"

        property string text: "S.D."
        property int size: 5

        Text {
            id: fdm
            anchors.centerIn:  parent
            color: "#ffffff"
            text: rect.text
            font.pixelSize: (parent.height*rect.size)/10
        }
    }

    Rectangle{
        id: randrec
        anchors.fill: parent

        visible: false;

        property int speed: 5

        Timer{
            interval: randrec.speed*100
            running: randrec.visible; repeat: true
            onTriggered: {
                randrec.color = Qt.rgba(Math.random(),Math.random(),Math.random(),1);
                date.color = Qt.rgba(Math.random(),Math.random(),Math.random(),1);

                date.text =  Qt.formatTime(new Date(),"hh:mm")
            }


        }
        Text {
            id: date
            anchors.fill:  parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: randrec.height
        }

    }

}
