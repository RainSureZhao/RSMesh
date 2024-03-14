// ModelViewer.qml
import QtQuick
import QtQuick3D

View3D {
    id: view3d

    property string modelSource: ""

    environment: SceneEnvironment {
        backgroundMode: SceneEnvironment.Color
        clearColor: "#4d4d4f" // 深灰色背景
    }

    PerspectiveCamera {
        id: camera
        position: Qt.vector3d(0, 0, 120)
        eulerRotation.x: -15
    }

    Model {
        id: model
        // 预设的source属性可以稍后通过设置属性或使用属性绑定来修改
        scale: Qt.vector3d(1, 1, 1) // 根据模型的实际大小调整缩放
        materials: [PrincipledMaterial {
            baseColor: "gray"
        }]
        source: modelSource // 绑定模型
    }

    function setModelSource(source) {
        modelSource = source
    }
}
