import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

// import Qt.labs.platform 1.1

FileDialog {
    id: exportDialog

    // selectMultiple: false

    signal fileChosen(string fileUrl)

    fileMode: FileDialog.SaveFile

    nameFilters: ["point cloud file (*.xyz)", "Wavefront file (*.obj)", "STL file (*.stl)"]

    onAccepted: {
        let fileUrl = Qt.resolvedUrl(selectedFile).toString();
        let selectedFormat = fileUrl.substring(fileUrl.lastIndexOf('.') + 1);
        console.log("Selected format: " + selectedFormat);
        if (selectedFormat.toLowerCase() === "xyz") {
            selectedFormat = "xyz";
        } else if (selectedFormat.toLowerCase() === "obj") {
            selectedFormat = "obj";
        } else if (selectedFormat.toLowerCase() === "stl") {
            selectedFormat = "stl";
        } else {
            console.log("Unsupported file format");
            return;
        }
        fileChosen(fileUrl);
        fileExporter.exportFile(fileUrl, selectedFormat);
    }
    onRejected: {
        console.log("FileDialog dismissed.");
    }
}
