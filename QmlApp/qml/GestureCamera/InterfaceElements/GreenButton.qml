import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Button{
    style:
        ButtonStyle{
            background: Rectangle {
                property int pressed: control.pressed

                border.width: control.activeFocus ? 2 : 1
                border.color: "#888"
                radius: 2
                gradient: Gradient {
                    GradientStop {
                        position: 0 ;
                        color: control.hovered ? control.pressed ? "#0c0" : "#0f0" : "#0e0"
                    }
                    GradientStop {
                        position: 1 ;
                        color: control.hovered ? control.pressed ? "#0a0" : "#0d0" : "#0c0"
                    }
                }
            }
    }
}
