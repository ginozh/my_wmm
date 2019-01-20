import QtQuick 2.5
import QtQuick.Layouts 1.2
import TextBoxPlugin 1.0
import QtGraphicalEffects 1.0
import org.qtproject.example 1.0

Item {
    id: root
    //width: ListView.view.width
    //height: 48
    property alias text: textlabel.text
    property alias color: textlabel.color
    property alias horizontalAlignment: textlabel.horizontalAlignment
    property alias font: textlabel.font
    signal clicked()
    signal remove()

    TextEdit 
    {
        id: textlabel
        textFormat: TextEdit.PlainText;//TextEdit.RichText
        //font.hintingPreference: Font.PreferNoHinting
        //focus: true
        Component.onCompleted: forceActiveFocus()
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
            //onClicked: { parent.focus = true }
            onClicked: { 
                parent.forceActiveFocus(); 
                //view.clicked();
                print('TextEdit onClicked');
            }
        }
    }
}
