import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    height: 600
    width: 800
    title: "RSMesh"
    visible: true

    ColumnLayout {
        anchors.fill: parent
        MenuBar {
            id: menuBar
            Layout.fillWidth: true
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
        ModelViewer {
            id: modelViewer
            Layout.fillWidth: true
            Layout.fillHeight: true
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
