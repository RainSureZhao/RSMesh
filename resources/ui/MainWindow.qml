import QtQuick
import QtQuick.Controls

ApplicationWindow {
    height: 600
    title: "RSMesh"
    visible: true
    width: 800

    MenuBar {
        Menu {
            title: "文件"

            MenuItem {
                text: "新建"
                onTriggered: {
                }
            }
            MenuItem {
                text: "打开"

                onTriggered: myFileDialog.open()
            }
            MenuItem {
                text: "导出"
                onTriggered: myExportFileDialog.open()
            }

            MenuItem {
                text: "退出"

                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: "编辑"

        }
    }
    FileDialog {
        id: myFileDialog

        onFileChosen: function (selectedFileUrl) {
            console.log("File chosen: " + selectedFileUrl);
        }
    }

    ExportFileDialog {
        id: myExportFileDialog

        onFileChosen: function (selectedFileUrl) {
            console.log("File chosen: " + selectedFileUrl);
        }
    }
}
