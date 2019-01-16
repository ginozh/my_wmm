import QtQuick 2.0
import QtQuick.Controls 1.4

import SceneGraphRendering 1.0
import TextBoxPlugin 1.0

Item {
    id: box
    width: 400
    height: 400

    Renderer {
        id: renderer
        anchors.fill: parent
        //anchors.margins: 10

        // The transform is just to show something interesting..
        opacity: 0
        Component.onCompleted: renderer.opacity = 1;
    }
    TextEdit {
        id: textlabel
        //width: 240
        textFormat: TextEdit.RichText
        text: "<b>Hello</b> <i>World!</i>"
        //font.family: "Helvetica"
        font.family: "宋体"
        font.pointSize: 20
        color: "blue"
        focus: true
		TextBox {
			//customitems/painteditem/textballoons.qml
            anchors.fill: parent
			width: parent.width
			height: parent.height
		}
/*
        Rectangle {
            anchors.fill: parent
            anchors.margins: -10
            color: "transparent"
            border.width: 1
        }
*/
        MouseArea {
            anchors.fill: parent
/*
            onClicked: { parent.color = 'red' }
*/
            drag.target: textlabel
            drag.axis: Drag.XAndYAxis
/*
            drag.minimumX: 0
            drag.maximumX: box.width - parent.width
            drag.minimumY: 0
            drag.maximumY: box.height - parent.width
*/
        }
    }
/*
    Rectangle {
        id: labelFrame
        width: textlabel.width
        height: textlabel.height
        x:textlabel.x
        y:textlabel.y
        //anchors.margins: -10
        //radius: 5
        color: "white"
        border.color: "black"
        opacity: 0.5
        //anchors.fill: label
        anchors.fill: textlabel
    }
*/

}
