import QtQuick 2.15
import QtQuick.Controls 2.15

Dialog {
    id: infoDialog
    modal: true
    visible: false // 默认不可见
    title: "Please Wait"
    property string message: "Loading..." // 默认提示文字

    // 确保对话框不可由用户手动关闭
    standardButtons: Dialog.Close

    Column {
        spacing: 10
        padding: 20

        Text {
            text: infoDialog.message
            font.pointSize: 16
        }

        // ActivityIndicator {
        //     id: activityIndicator
        //     running: infoDialog.visible
        //     width: 40
        //     height: 40
        //     // 使用 Qt Quick Controls 2 的样式特性进行美化
        //     Component.onCompleted: {
        //         activityIndicator.indicatorColor = "blue" // 修改指示器颜色
        //     }
        // }
    }
}