import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0


Window {
    width: Screen.width
    height: Screen.height
    visible: true
    title: qsTr("Calibration")
    color: "gray"
    Image {
        id: yonga
        source: "file:/home/selim/Downloads/Yonga.jpg"
        x: imageRect.x + imageRect.width/2 - yonga.width/2
        y:20
        width: 270
        height: 170
    }

    Rectangle{
        id: imageRect
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: 800
        height: 600

        color: "transparent"
        border.color: "black"
        border.width: 3

        Image{
            id: opencvImage
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            property bool counter: false
            visible: false
            source: "image://live/image"
            asynchronous: false
            cache: false

            function reload()
            {
                counter = !counter
                source = "image://live/image?id=" + counter
            }

        }
    }

    Button{
        id: startButton //Start.
        x: imageRect.x/2 - startButton.width/2
        y:imageRect.y
        width: 200
        text: "Start"

        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 40
            color: startButton.hovered ? "white" : "gray"
            border.color: startButton.down ? "green" : "red" //"#17a81a" : "#21be2b"
            border.width: 2
            radius: 4
        }
        onClicked: {
            VideoStreamer.openVideoCamera(1)
            opencvImage.visible = true
        }


    }
    Button{
        id: button2  //Find Values.
        x: imageRect.x/2 - button2.width/2
        y:imageRect.y + startButton.height + 50
        width: 200
        text: "Find Values"

        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 40
            color: button2.hovered ? "white" : "gray"
            border.color: button2.down ? "green" : "red" //"#17a81a" : "#21be2b"
            border.width: 2
            radius: 4
        }

        onClicked: {
            VideoStreamer.findCValues()
            VideoStreamer.getstat(text8.text)
            opencvImage.visible = true
        }


    }

    Button{
        id: button3  //Draw.
        x: imageRect.x/2 - button3.width/2
        y:text8.y+ text8.height + 50
        width: 200
        text: "Draw"

        property bool aBool1: false
        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 40
            color: button3.hovered ? "white" : "gray"
            border.color: button3.aBool1 ? "green" : "red" //"#17a81a" : "#21be2b"
            border.width: 2
            radius: 4
        }

        onClicked: {
            VideoStreamer.draw()
            opencvImage.visible = true
            button3.aBool1 = !button3.aBool1
        }


    }



    Button{
        id: button4  //Apply.
        x: imageRect.x/2 - button4.width/2
        y:button3.y + button3.height + 50
        width: 200
        text: "Apply"
        property bool aBool: false
        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 40
            color: button4.hovered ? "white" : "gray"
            border.color: button4.aBool ? "green" : "red" //"#17a81a" : "#21be2b"
            border.width: 2
            radius: 4
        }


        onClicked: {
            VideoStreamer.apply()
            opencvImage.visible = true
            button4.aBool = !button4.aBool

        }


    }




//    Button{
//        id: button5  //Find Objects.
//        x: imageRect.x/2 - button5.width/2
//        y:600
//        width: 200
//        text: "Find Objects "

//        background: Rectangle {
//            implicitWidth: 200
//            implicitHeight: 40
//            color: button5.hovered ? "white" : "gray"
//            border.color: button5.down ? "green" : "red" //"#17a81a" : "#21be2b"
//            border.width: 2
//            radius: 4
//        }


//        onClicked: {
//            VideoStreamer.obDet()
//            VideoStreamer.less(canny2.text)
//            VideoStreamer.bigger(canny3.text)
//            VideoStreamer.cannyT(canny4.text)
//            opencvImage.visible = true
//        }


//    }

//    TextField{
//        id:canny2
//        x: imageRect.x/2 - button5.width/2
//        y: 650
//        placeholderText: "Less Than"
//        cursorVisible: true

//        width: button5.width
//    }

//    TextField{
//        id:canny3
//        x: imageRect.x/2 - button5.width/2
//        y: 700
//        placeholderText: "Bigger Than"
//        cursorVisible: true

//        width: button5.width
//    }
//    TextField{
//        id:canny4
//        x: imageRect.x/2 - button5.width/2
//        y: 750
//        placeholderText: "Canny Threshold"
//        cursorVisible: true

//        width: button5.width
//    }


    Button{
        id: button6  //Save.
        x: imageRect.x + imageRect.width + button6.width/2
        y:imageRect.y
        width: 200
        text: "Save"
        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 40
            color: button6.hovered ? "white" : "gray"
            border.color: button6.down ? "green" : "red" //"#17a81a" : "#21be2b"
            border.width: 2
            radius: 4
        }


        onClicked: {
            VideoStreamer.save(text6.text)
            opencvImage.visible = true
        }


    }
    TextField{
        id:text6
        x: imageRect.x + imageRect.width + button6.width/2
        y: imageRect.y + button6.height + 10
        placeholderText: "Save Path"
        cursorVisible: true

        width: button6.width
    }


    Button{
        id: button7  //Load.
        x: imageRect.x + imageRect.width + button7.width/2
        y:imageRect.y + button6.height + text6.height + 20
        width: 200
        text: "Load"

        background: Rectangle {
            implicitWidth: 200
            implicitHeight: 40
            color: button7.hovered ? "white" : "gray"
            border.color: button7.down ? "green" : "red" //"#17a81a" : "#21be2b"
            border.width: 2
            radius: 4
        }


        onClicked: {
            VideoStreamer.load(text7.text)
            opencvImage.visible = true
        }


    }
    TextField{
        id:text7
        x: imageRect.x + imageRect.width + button7.width/2
        y: button7.y + button7.height + 10
        placeholderText: "Load Path"
        cursorVisible: true

        width: button7.width
    }

    TextField{
        id:text8
        x: imageRect.x/2 - text8.width/2
        y: button2.y+ button2.height + 10
        placeholderText: "if 6 write 1, if 15 write 2"
        cursorVisible: true

        width: button7.width
    }




    Connections{
        target: liveImageProvider

        function onImageChanged()
        {
            opencvImage.reload()
        }

    }
}
