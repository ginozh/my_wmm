import QtQuick 2.7
import QtQuick.Controls 2.1
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
            id: textbox
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
                textbox.visible=true;
                parent.clicked();
                //view.clicked();
                print('TextEdit onClicked');
            }
        }
        onEditingFinished:  {
            box.textChanged(textlabel.text); 
            textbox.visible=false;
        }
    }
	ShaderEffectSource {
		id: textrecttheSource
		sourceItem: textlabel
    }
	ShaderEffectSource {
		id: sourceBlackSource
		sourceItem: sourceBlack
	}
	Rectangle {
		visible:false 
		width: parent.width; height: parent.height
        id: sourceBlack
        color: "transparent"
		//color: '#000000'
	}
	ShaderEffect {
		//visible:false 
		visible: true
		id: effect2
		width: parent.width; height: parent.height
		// default vertex shader code
		vertexShader: "
		uniform highp mat4 qt_Matrix;
		attribute highp vec4 qt_Vertex;
		attribute highp vec2 qt_MultiTexCoord0;
		varying highp vec2 qt_TexCoord0;
		void main() {
			qt_TexCoord0 = qt_MultiTexCoord0;
			gl_Position = qt_Matrix * qt_Vertex;
		}"
        //property variant mSample0: theSource
        property variant mSample0: sourceBlackSource;
        property variant mSample1: textrecttheSource; //sourceImage;// textrecttheSource;// sourceBlack;// textrect;// textrect;// textlabel;// sourceImage
        //property color tint: root.sliderToColor(colorizeSlider.value)
        property real u_global_time: colorizeSlider.value
        property real u_total_time: 100
        fragmentShader: "qrc:qmlMotionEffect.frag"
        //fragmentShader: "qrc:shadereffects/content/shaders/colorize1.frag"
		// default fragment shader code
        Slider {
            id: colorizeSlider
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
			from: 1
			value: 25
			to: 100
            height: 40
        }
	}
}
