import QtQuick 2.5
import QtQuick.Layouts 1.2
import TextBoxPlugin 1.0
import QtGraphicalEffects 1.0

Item {
    id: root
    width: ListView.view.width
    height: 48
    property alias text: textlabel.text
    property alias color: textlabel.color

    signal clicked()
    signal remove()

    TextEdit 
    {
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
        //font.hintingPreference: Font.PreferNoHinting
        color: "blue"
        focus: true
		TextBox {
			//customitems/painteditem/textballoons.qml
            anchors.fill: parent
			width: parent.width
			height: parent.height
		}
        MouseArea {
            anchors.fill: parent
            drag.target: textlabel
            drag.axis: Drag.XAndYAxis
        }
    }
}
