import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

FileDialog {
    id: fileDialog

    signal fileChosen(string fileUrl)

    title: "Please choose a file"

    onAccepted: {
        fileChosen(selectedFile);
        // fileLoader.openFile(selectedFile);
        modelViewer.setModelSource(selectedFile);
    }
    onRejected: {
        console.log("FileDialog dismissed.");
    }
}