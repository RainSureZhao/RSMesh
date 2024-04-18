import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

FileDialog {
    id: fileDialog

    signal fileChosen(string fileUrl)

    title: "Please choose a file"

    onAccepted: {
        fileChosen(selectedFile);
        loadingDialog.message = "Loading model..."
        loadingDialog.open();
        // fileLoader.openFile(selectedFile);
        // 调用balasm工具将obj, stl等格式文件转为.mesh文件并进行展示
        modelConverter.convertModel(selectedFile, appDirPath + "/temp");
        // modelViewer.setModelSource(selectedFile);
    }
    onRejected: {
        console.log("FileDialog dismissed.");
    }
}