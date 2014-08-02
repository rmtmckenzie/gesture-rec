import QtQuick 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtMultimedia 5.0

import Motion.View 1.0
import Motion.HandRec 1.0

import "InterfaceElements"

ApplicationWindow {
    title: qsTr("Camera Capture")
    id: root
    width: 640 + 80
    height: 480

    visible: true


    //    width:320
    //    height:240
    HandRec {
        id: handrec
        camNumber: 0
        running: true
        blur: blurbox.value
    }

    Row {
        anchors.fill: parent
        spacing: 2
        CameraView {
            id: view

            api: handrec

            height: parent.height
            width: root.width - 80

            anchors {
                verticalCenter: parent.verticalCenter
            }

            onNameChanged: {
                update()
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var c = view.getColor(mouse.x, mouse.y)
                    //                    console.log(c)
                    handrec.addHandColor(c)
                }
            }
        }

        Column {
            height: parent.height - 20
            width: parent.width - view.width - 4
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5

            YellowButton {
                text: "Reset\nColours"
                anchors {
                    left: parent.left
                    right: parent.right
                }
                onClicked: {
                    handrec.resetHandColors()
                }
            }

            GreenButton {
                text: "Take\nBackground"
                anchors {
                    left: parent.left
                    right: parent.right
                }
                onClicked: {
                    handrec.invokeTakeBackgroundImage()
                }
            }

            Text {
                text: "Blur:"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            SpinBox {
                id: blurbox
                activeFocusOnPress: false
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }

            Column {
                anchors {
                    left: parent.left
                    right: parent.right
                }
                spacing: 1

                ExclusiveGroup {
                    id: views
                }

                Text {
                    text: "Views:"
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                    }
                }

                GreenButton {
                    text: "Normal\nWebcam"
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    checked: true
                    checkable: true
                    exclusiveGroup: views
                    onClicked: {
                        //                    checked = true
                        handrec.stage = 0
                    }
                }

                GreenButton {
                    text: "Filtered\nFrames"
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    exclusiveGroup: views
                    checkable: true
                    onClicked: {
                        handrec.stage = 1
                    }
                }

                GreenButton {
                    text: "Parsed\nHand"
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    exclusiveGroup: views
                    checkable: true
                    onClicked: {
                        handrec.stage = 2
                    }
                }

                GreenButton {
                    text: "View\nBackground"
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    exclusiveGroup: views
                    checkable: true
                    onClicked: {
                        handrec.stage = 3
                    }
                }

                GreenButton {
                    text: "View\nSubtracted"
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    exclusiveGroup: views
                    checkable: true
                    onClicked: {
                        handrec.stage = 4
                    }
                }

                //                Text {
                //                    id: RotationText
                //                    text: "Unknown"
                //                    anchors {
                //                        left:parent.left
                //                        right:parent.right
                //                    }
                //                }
                Item {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    height: 20
                    Rectangle {
                        anchors {
                            left: parent.left
                            right: parent.horizontalCenter
                            top: parent.top
                            bottom: parent.bottom
                        }
                        color: handrec.lowColor
                    }
                    Rectangle {
                        anchors {
                            left: parent.horizontalCenter
                            right: parent.right
                            top: parent.top
                            bottom: parent.bottom
                        }
                        color: handrec.highColor
                    }
                }
            }
        }
    }

    Item {
        id: inputhandler
        focus: true
        Keys.onSpacePressed: {
            (view.playing ? view.pause() : view.play())
        }
        Keys.onDigit0Pressed: ;
    }

    //    VideoOutput{
    //        id : vid
    //        source : camera
    //        anchors.fill : parent
    //        focus : visible
    //    }

    //    Camera {
    //        id: camera
    //    }

    //    ShaderEffect {
    //        anchors.fill: parent
    //        property variant source: ShaderEffectSource{sourceItem: vid;hideSource:true}
    //        property real dividerValue: .5;
    //        fragmentShader: "
    //            uniform float dividerValue;
    //            const float step_w = 0.0015625;
    //            const float step_h = 0.0027778;

    //            uniform sampler2D source;
    //            uniform lowp float qt_Opacity;
    //            varying vec2 qt_TexCoord0;

    //            void main()
    //            {
    //                vec2 uv = qt_TexCoord0.xy;
    //                vec3 t1 = texture2D(source, vec2(uv.x - step_w, uv.y - step_h)).rgb;
    //                vec3 t2 = texture2D(source, vec2(uv.x, uv.y - step_h)).rgb;
    //                vec3 t3 = texture2D(source, vec2(uv.x + step_w, uv.y - step_h)).rgb;
    //                vec3 t4 = texture2D(source, vec2(uv.x - step_w, uv.y)).rgb;
    //                vec3 t5 = texture2D(source, uv).rgb;
    //                vec3 t6 = texture2D(source, vec2(uv.x + step_w, uv.y)).rgb;
    //                vec3 t7 = texture2D(source, vec2(uv.x - step_w, uv.y + step_h)).rgb;
    //                vec3 t8 = texture2D(source, vec2(uv.x, uv.y + step_h)).rgb;
    //                vec3 t9 = texture2D(source, vec2(uv.x + step_w, uv.y + step_h)).rgb;
    //                vec3 xx = t1 + 2.0*t2 + t3 - t7 - 2.0*t8 - t9;
    //                vec3 yy = t1 - t3 + 2.0*t4 - 2.0*t6 + t7 - t9;
    //                vec3 rr = sqrt(xx * xx + yy * yy);
    //                vec3 col = rr * 2.0 * t5;
    //                if (uv.x < dividerValue)
    //                    gl_FragColor = qt_Opacity * vec4(col, 1.0);
    //                else
    //                    gl_FragColor = qt_Opacity * texture2D(source, uv);
    //            }
    //        "
    //    }
}
