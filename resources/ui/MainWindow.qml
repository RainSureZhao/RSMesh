import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ModelConversion 1.0

ApplicationWindow {
    id: rootWindow
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
        ModelConverter {
            id: modelConverter
            onConversionSuccess: {
                console.log("Model conversion finished");
                loadingDialog.close();
                // 在这里执行转换完成后的操作，例如更新 UI 或加载新模型
                modelViewer.modelSource = appDirPath + "/temp/meshes/defaultobject_mesh.mesh";
            }
            onConversionFailure: {
                // 处理转换失败的逻辑，例如显示错误消息
                console.log("Conversion failed");
                // loadingDialog.close();
                loadingDialog.message = "转换失败";
                // loadingDialog.open();
                // 可以在这里弹出一个错误对话框或者显示一条错误信息
            }
        }
        InfoDialog {
            id: loadingDialog
            x: rootWindow.width / 2 - width / 2
            y: rootWindow.height / 2 - height / 2
            width: rootWindow.width * 0.2
            height: rootWindow.height * 0.2
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
