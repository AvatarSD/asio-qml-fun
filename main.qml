import QtQuick 2.7
import QtQuick.Window 2.2
import QtQml 2.2


Window {
    visible: true
    width: 640
    height: 480

    title: "S.D."
    id: mainrect

    signal colorChanged(color col)
    signal backcolChanged(color col)

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

    function color(msg)
    {
        rect.randcolor = false;
        if(msg)
            rect.textcolor = msg;
        else
            rect.textcolor = Qt.rgba(Math.random(),Math.random(),Math.random(),1);
    }
    function back(msg)
    {
        rect.randcolor = false;
        if(msg)
            rect.color = msg;
        else
            rect.color = Qt.rgba(Math.random(),Math.random(),Math.random(),1);

    }
    function rand(msg)
    {
        if(msg)
        {
            rect.randcolor = true;
            randrec.speed = msg;
        }
        else
        {
            rect.randcolor = false;
            rect.setrandcolor();
        }
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
        objectName: "rect"
        color: "#000000"
        property string text: "S.D."
        property int size: 8
        property color textcolor: "#ffffff"
        property bool randcolor: false
        property bool dich: false

        onColorChanged: backcolChanged(rect.color)


        anchors.fill: parent
        visible: true;
        function setrandcolor()
        {
            rect.color = Qt.rgba(Math.random(),Math.random(),Math.random(),1);
            rect.textcolor = Qt.rgba(Math.random(),Math.random(),Math.random(),1);
        }
        Component.onCompleted: setrandcolor()

        Text {
            id: fdm
            anchors.centerIn:  parent
            color: rect.textcolor
            text: rect.text
            font.pixelSize: (parent.height*rect.size)/10
            visible: false;
            onColorChanged: colorChanged(fdm.color)
        }

        Text {
            visible: true;
            id: date
            anchors.fill:  parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: rect.height
            color: rect.textcolor
            onVisibleChanged: rect.setrandcolor()
        }

    }

    Rectangle{
        id: randrec
        anchors.fill: parent

        visible: false;

        property int speed: 3

        function setTimeToScreen()
        {
            date.text =  Qt.formatTime(new Date(),"hh:mm")
        }

        Timer{
            id: timeclock
            interval: 1000
            running: date.visible & !dichtimer.running; repeat: true
            onTriggered: randrec.setTimeToScreen()
            onRunningChanged: randrec.setTimeToScreen()
        }
        Timer{
            id: randrimer
            interval: randrec.speed*100
            running: rect.randcolor; repeat: true
            onTriggered: rect.setrandcolor()
            onRunningChanged: rect.setrandcolor()
        }
        Timer{
            id: dichtimer
            interval: randrec.speed*66
            running: rect.dich; repeat: true
            onRunningChanged:  date.text = Math.random();
            onTriggered:      date.text = Math.random();

        }

    }

}
