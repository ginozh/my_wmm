import QtQuick 2.0
import QtQuick.Controls 1.4

import SceneGraphRendering 1.0
import TextBoxPlugin 1.0
import QtGraphicalEffects 1.0
// our module
import org.example 1.0

Item {
    id: box
    property alias currentText: textlabel.text
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
    // our dyanmic model
    DynamicEntryModel {
        id: dynamic
        onCountChanged: {
            // we print out count and the last entry when count is changing
            print('new count: ' + count);
            print('last entry: ' + get(count-1));
        }
    }
    //view
    Repeater 
    {
        id: view
        // set our dynamic model to the views model property
        model: dynamic
        delegate: ListDelegate {
            x:200
            width: 200;//ListView.view.width
            // construct a string based on the models proeprties
            text: 'hsv(' +
                  Number(model.hue).toFixed(2) + ',' +
                  Number(model.saturation).toFixed() + ',' +
                  Number(model.brightness).toFixed() + ')'
            // sets the font color of our custom delegates
            color: model.name

            onClicked: {
                // make this delegate the current item
                view.currentIndex = index
                view.focus = true
            }
            onRemove: {
                // remove the current entry from the model
                dynamic.remove(index)
            }
        }
    }
    TextEntry {
        id: textEntry
        y:200
        onAppend: {
            // called when the user presses return on the text field
            // or clicks the add button
            dynamic.append(color)
        }

        onUp: {
            // called when the user presses up while the text field is focused
            view.decrementCurrentIndex()
        }
        onDown: {
            // same for down
            view.incrementCurrentIndex()
        }

    }
    TextEdit {
        id: textlabel
        ////visible: false; //no
        //width: 240
        textFormat: TextEdit.RichText
        //text: "<b>Hello</b> <i>World!</i>"
        text: "Hello World!"
        //font.family: "Helvetica"
        font.family: "宋体"
        font.pointSize: 20
        font.letterSpacing: 0.9
        font.strikeout : true
        font.hintingPreference: Font.PreferNoHinting
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
    DropShadow {
        anchors.fill: textlabel
        source: textlabel
        horizontalOffset: 5
        verticalOffset: 5
        color: "#80000000"
        radius: 8.0
        samples: 16
    }
    Text { font.pointSize: 24; text: "Outline";style: Text.Outline; styleColor: "red" }
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
    Item {
      x:parent.width-width
      width: 200; height: 250

      ListModel {
          id: myModel
          ListElement { type: "Dog"; age: 8 }
          ListElement { type: "Cat"; age: 5 }
      }

      Component {
          id: myDelegate
          Text { text: type + ", " + age }
      }

      ListView {
          anchors.fill: parent
          model: myModel
          delegate: myDelegate
      }
    }
    Row {
        y:parent.height-50
      Repeater {
          model: 3
          Rectangle {
              width: 100; height: 40
              border.width: 1
              color: "yellow"
          }
      }
    }

}
