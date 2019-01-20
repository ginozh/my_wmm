import QtQuick 2.0
import QtQuick.Controls 1.4

import SceneGraphRendering 1.0
// our module
import org.example 1.0
import org.qtproject.example 1.0

Item {
    id: box
    ///property alias currentText: textlabel.text
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
    DynamicEntryModel 
    {
        id: dynamic
        onCountChanged: {
            // we print out count and the last entry when count is changing
            print('new count: ' + count);
            ///print('last entry: ' + get(count-1));
        }
    }
    //view
    Repeater 
    {
        id: view
        // set our dynamic model to the views model property
        model: dynamic
        delegate: ListDelegate 
        {
            visible:model.visible
            x:model.posx
            y:model.posy
            //width: 200;//ListView.view.width
            // construct a string based on the models proeprties
            text: model.text
            // sets the font color of our custom delegates
            color: model.color
            horizontalAlignment: TextEdit.AlignHCenter
            font.family: "宋体"
            font.pointSize: 20
            font.letterSpacing: 0.9
            font.bold: true
            font.underline: true
            font.strikeout : true
            font.italic : true

            onClicked: { //<=> ListDelegate signal clicked()
                // make this delegate the current item
                print('Repeater onClicked');
                view.currentIndex = index
                view.focus = true
            }
/*
            onRemove: {
                // remove the current entry from the model
                dynamic.remove(index)
            }
*/
        }
    }
    function addNewText(txt) { //json
        print('new text: ' + txt);
        dynamic.append(txt);
        return dynamic.count-1;
    }
    function modify(idx, txt) { //json
        print('modify txt: ' + txt);
        dynamic.modify(idx, txt);
    }
    function disappear(txt) { //json: [idx, idx]
    }
    function remove(index) {
        dynamic.remove(index);
    }
}
