import QtQuick 2.7
import QtQuick.Controls 2.1

import SceneGraphRendering 1.0
// our module
import org.example 1.0
import org.qtproject.example 1.0

Item {
    id: box
    ///property alias currentText: textlabel.text
    width: 600
    height: 400
    signal textChanged(string msg)

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
        delegate: ShaderEffect{
            visible:model.visible
            width:box.width;height:box.height;
            x:model.posx
            y:model.posy
/*
            Rectangle {
                visible:true;
                id: textlabelItem
                width:box.width;height:box.height;
                color:"transparent"
                */
                TextEdit 
                {
                    //visible:false;
                    width:box.width;height:box.height;
                    id: textlabel
                    text: model.text
                    x:model.posx
                    y:model.posy
                    // sets the font color of our custom delegates
                    color: model.color
                    //horizontalAlignment: TextEdit.AlignHCenter
                    font.family: "宋体"
                    font.pixelSize: 200
                    //font.bold:true
                    textFormat: TextEdit.PlainText;//TextEdit.RichText
                    //font.hintingPreference: Font.PreferNoHinting
                    //focus: true
                    Component.onCompleted: forceActiveFocus()
                    MouseArea {
                        anchors.fill: parent
                        drag.target: textlabel
                        drag.axis: Drag.XAndYAxis
                    }
                }
            //}
            // default vertex shader code
            // /*
            vertexShader: "
            uniform highp mat4 qt_Matrix;
            attribute highp vec4 qt_Vertex;
            attribute highp vec2 qt_MultiTexCoord0;
            varying highp vec2 qt_TexCoord0;
            void main() {
                qt_TexCoord0 = qt_MultiTexCoord0;
                gl_Position = qt_Matrix * qt_Vertex;
            }"
            // */
           /*
           property variant uResolution: Qt.size(box.width, box.height)
           property variant uTranslation: Qt.size(0.0, 0.0)
           property variant uScale: Qt.size(1.0, 1.0)
           //property variant uRotation: Qt.size(1.0, 0.0)
           property real u_degrees: 45
           vertexShader: "
            uniform vec2 uResolution;
            uniform vec2 uTranslation;
            uniform vec2 uScale;
            ///uniform vec2 uRotation;
            uniform float u_degrees;

            uniform highp mat4 qt_Matrix;
            attribute highp vec4 qt_Vertex;
            attribute highp vec2 qt_MultiTexCoord0;
            varying highp vec2 qt_TexCoord0;
            void main() {
                //qt_TexCoord0 = qt_MultiTexCoord0;
                //gl_Position = qt_Matrix * qt_Vertex;

                // 缩放
                vec2 scaledPosition = (qt_Matrix * qt_Vertex).xy * uScale;

                // 旋转
                float angleInRadians = u_degrees * 3.14159 / 180.0;
                //vec2 uRotation = vec2(sin(angleInRadians), cos(angleInRadians));
                vec2 uRotation = vec2(sin(angleInRadians), cos(angleInRadians));
                vec2 rotatedPosition = vec2(
                    scaledPosition.x * uRotation.y + scaledPosition.y * uRotation.x,
                    scaledPosition.y * uRotation.y - scaledPosition.x * uRotation.x
                );

                // 平移
                vec2 position = rotatedPosition + uTranslation;
                //vec2 position = scaledPosition + uTranslation;
                vec2 zeroToOne = position / uResolution;
                vec2 zeroToTwo = zeroToOne * 2.0;
                vec2 clipSpace = zeroToTwo - 1.0;

                gl_Position = vec4(clipSpace * vec2(1, -1), 0, 1);

                gl_Position = vec4((qt_Matrix * qt_Vertex).xy + uTranslation, (qt_Matrix * qt_Vertex).z, (qt_Matrix * qt_Vertex).w);
                gl_Position = vec4(position, (qt_Matrix * qt_Vertex).z, (qt_Matrix * qt_Vertex).w);
                gl_Position = vec4(clipSpace * vec2(1, -1), (qt_Matrix * qt_Vertex).z, (qt_Matrix * qt_Vertex).w);

                //qt_TexCoord0 = qt_MultiTexCoord0;
                vec2 newPos = (qt_MultiTexCoord0 * uTexsize) / uImagesize; // image relative position
                qt_TexCoord0 = (uTexpos / uImagesize) + newPos;
            }"
            */
            //property variant mSample0: theSource
            property variant mSample0: sourceBlackSource;
            //property variant mSample1: sourceRedSource;//textrecttheSource; //sourceImage;// textrecttheSource;// sourceBlack;// textrect;// textrect;// textlabel;// sourceImage
            property variant mSample1: ShaderEffectSource {
                //sourceItem: textlabelItem
                //sourceItem: sourceRed
                sourceItem: textlabel
                //sourceItem: image
                //hideSource: true
                //live: true
            }
            //property color tint: root.sliderToColor(colorizeSlider.value)
            property real u_global_time: colorizeSlider.value
            property real u_total_time: 100
            property variant u_resolution: Qt.size(box.width, box.height)
            fragmentShader: "qrc:qmlMotionEffect.frag"
            //fragmentShader: "qrc:shadereffects/content/shaders/colorize1.frag"
            // default fragment shader code
            Slider {
                id: colorizeSlider
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                from: 1
                value: 70
                to: 100
                height: 40
            }
        }
        /*
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
                print('Repeater onClicked index: '+index);
                view.currentIndex = index
                view.focus = true
            }
        }
        */
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
	ShaderEffectSource {
		id: sourceRedSource
		sourceItem: sourceRed
	}
	Rectangle {
		visible:false 
		//visible: true
		width: 480; height: 240
		id: sourceRed
		color: '#ffff0000'
	}
	ShaderEffectSource {
		id: textrecttheSource
		//sourceItem: textrect
		sourceItem: textlabel
    }
    Image {
        id: image
        //anchors.fill: mask
        //source: "qrc:rotate.jpg"
        source: "qrc:rotate1.jpg"
    }
    ShaderEffectSource {
        id: sourceImage
        hideSource: true
        sourceItem: image
    }
}
