import QtQuick 2.7
import QtQuick.Window 2.2
import QtQml 2.2


Window {
    visible: true
    width: 640
    height: 480

    title: "S.D."

    function text(msg)
    {
        fdm.visible = true;
        date.visible = false;
        rect.dich = false;
        if(msg)
            rect.text = msg;
    }
    function clock(msg)
    {
        fdm.visible = false;
        date.visible = true
        rect.dich = false;
    }

    function back(msg)
    {
        rect.randcolor = false;
        if(msg)
            rect.textcolor = msg;
        else
            rect.textcolor = "#ffffff";
    }
    function color(msg)
    {
        rect.randcolor = false;
        //rect.textcolor = "#ffffff"
        if(msg)
            rect.color = msg;
        else
            rect.color = "#000000";
    }
    function rand(msg)
    {
        rect.randcolor = true;
        if(msg)
            randrec.speed = msg;
    }

    function size(msg)
    {
        if(msg)
        rect.size = msg
    }

    function dich(msg)
    {
        rect.dich = true;
        date.visible = true
        fdm.visible = false;
    }

    Rectangle{
        id: rect
        anchors.fill: parent
        objectName: "rect"
        visible: true;

        color: "#000000"
        property string text: "S.D."
        property int size: 5
        property color textcolor: "#ffffff"
        property bool randcolor: false
        property bool dich: false

        Text {
            id: fdm
            anchors.centerIn:  parent
            color: rect.textcolor
            text: rect.text
            font.pixelSize: (parent.height*rect.size)/10
            visible: true;
        }

        Text {
            visible: false;
            id: date
            anchors.fill:  parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: rect.height
            color: rect.textcolor
        }

    }

    Rectangle{
        id: randrec
        anchors.fill: parent

        visible: false;

        property int speed: 5

        Timer{
            id: timeclock
            interval: 1000
            running: date.visible & !dichtimer.running; repeat: true
            onTriggered: {
                date.text =  Qt.formatTime(new Date(),"hh:mm")
            }
        }
        Timer{
            id: randrimer
            interval: randrec.speed*100
            running: rect.randcolor; repeat: true
            onTriggered: {
                rect.color = Qt.rgba(Math.random(),Math.random(),Math.random(),1);
                rect.textcolor = Qt.rgba(Math.random(),Math.random(),Math.random(),1);
            }


        }
        Timer{
            id: dichtimer
            interval: randrec.speed*66
            running: rect.dich; repeat: true
            onTriggered: {
                date.text = Math.random();
            }
        }

    }

}
