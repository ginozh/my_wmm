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
/*
        BorderImage {
            id: borderImage
            anchors.fill: parent
            source: "lineedit-bg.png"
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
    TextInput {
            id: textInput
          text: "TextInput"
          cursorVisible: true
        MouseArea {
            anchors.fill: parent
            //! [drag]
            drag.target: textInput
            drag.axis: Drag.XAndYAxis
            //! [drag]
        }
    }
*/
/*
	TextArea {
      //width: 240
	  backgroundVisible: false
      text:
          "Lorem ipsum dolor sit amet, consectetur adipisicing elit ";
  }
*/

/*
Rectangle {
    property alias mouseArea: mouseArea
    property alias textEdit: textEdit

    //width: 360
    //height: 360
        width: textlabel.width
        height: textlabel.height

    MouseArea {
        id: mouseArea
        anchors.rightMargin: 39
        anchors.bottomMargin: 253
        anchors.leftMargin: -39
        anchors.topMargin: -253
        anchors.fill: parent

        BorderImage {
            id: borderImage
            x: 77
            y: 90
            width: 100
            height: 100
            source: "lineedit-bg.png"
        }
    }

    TextEdit {
        id: textEdit
        text: qsTr("Enter some text...")
        verticalAlignment: Text.AlignVCenter
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        Rectangle {
            anchors.fill: parent
            anchors.margins: -10
            color: "transparent"
            border.width: 1
        }
    }
}
*/

/*
  Flickable {
       id: flick

       width: 300; height: 200;
       contentWidth: edit.paintedWidth
       contentHeight: edit.paintedHeight
       clip: true

       function ensureVisible(r)
       {
           if (contentX >= r.x)
               contentX = r.x;
           else if (contentX+width <= r.x+r.width)
               contentX = r.x+r.width-width;
           if (contentY >= r.y)
               contentY = r.y;
           else if (contentY+height <= r.y+r.height)
               contentY = r.y+r.height-height;
       }

       TextEdit {
           id: edit
           width: flick.width
           height: flick.height
           focus: true
           wrapMode: TextEdit.Wrap
           onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
           text: "<b>Hello</b> <i>World!</i>"
       }
   }
*/
/*
    Text {
        id: label
        anchors.bottom: renderer.bottom
        anchors.left: renderer.left
        anchors.right: renderer.right
        anchors.margins: 40
        wrapMode: Text.WordWrap
        text: "测试\ntest\ntest\ntest."
    }
*/
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
