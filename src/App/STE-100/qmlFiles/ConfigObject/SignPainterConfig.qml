/*
@FileName: SignPainterConfig.qml
@Describe: 配置体测签字版 弹窗尺寸以及坐标的单例对象
@Author: xutianci
@Attentions:
@Create Date:  2022-07-28  11:59:21
*/
pragma Singleton
import QtQuick 2.12
import QtQuick.Window 2.12

QtObject{
    property int otoDisplayWidth: 1400
    property int otoDisplayHeight: 622
    property int otoXpos: 250
    property int otoYpos: 1130

    property int otmDisplayWidth: 1632
    property int otmDisplayHeight: 493
    property int otmXpos: 135
    property int otmYpos: 1699



    function initSignPainter( isDirectModel ){
        if( isDirectModel ){
            changeOtoSignPainter()
        }else{
            changeOtmSignPainter()
        }
    }


    function initOto( width, height, x, y ){
        otoDisplayWidth = width
        otoDisplayHeight = height
        otoXpos = x
        otoYpos = y

        changeOtoSignPainter()
    }

    function initOtm( width, height, x, y ){
        otmDisplayWidth = width
        otmDisplayHeight = height
        otmXpos = x
        otmYpos = y

        changeOtmSignPainter()
    }

    function changeOtoSignPainter(){
        InterAction.changedSignPainterPos( otoDisplayWidth, otoDisplayHeight, otoXpos, otoYpos )
    }

    function changeOtmSignPainter(){
        InterAction.changedSignPainterPos( otmDisplayWidth, otmDisplayHeight, otmXpos, otmYpos )
    }

}
