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
/*
    Rectangle {
        anchors.fill: parent
        color: '#ffffff'
        opacity: 0.2
        border.color: Qt.darker(color)
    }
*/
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

/*
        DropShadow {
            anchors.fill: parent
            source: parent
            horizontalOffset: 5
            verticalOffset: 5
            color: "#80000000"
            radius: 8.0
            samples: 16
        }
*/

    }
/*
    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                id: label
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 8
                color: '#f7f7f7'
                font.pixelSize: 24
            }
            MouseArea {
                anchors.fill: parent
                onClicked: root.clicked()
            }
        }
        Item {
            Layout.fillHeight: true
            Layout.preferredWidth: icon.width
            Image {
                id: icon
                anchors.centerIn: parent
                source: 'remove.png'
            }
            MouseArea {
                anchors.fill: parent
                onClicked: root.remove()
            }
        }
    }
*/
}
