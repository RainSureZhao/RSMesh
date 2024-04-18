// ModelViewer.qml
import QtQuick
import QtQuick.Window
import QtQuick3D
import Qt3D.Render

View3D {  //定义一个3d场景
    id: view3d
    width: parent.width
    height: parent.height
    anchors.bottom: parent.bottom
    camera: cameraPerspective     //绑定摄像机
    environment: sceneEnvironment //绑定环境
    focus: true // 确保View3D可以捕获鼠标事件
    property string modelSource: "#Cube"

    SceneEnvironment {
        id: sceneEnvironment
        antialiasingMode: SceneEnvironment.MSAA           //这个属性控制渲染场景时应用的抗锯齿模式，具体参数取值，可F1查找帮助文档
        antialiasingQuality: SceneEnvironment.VeryHigh    //这个属性设置场景的抗锯齿级别。行为依赖于使用的antialiasingMode。当antialiasingMode属性设置为NoAA时，这个属性没有作用。
        backgroundMode: SceneEnvironment.Transparent  　　 //该参数可设置 3d场景的背景
    }
    PerspectiveCamera {
        id: cameraPerspective
        x: -0
        y: 23.861
        eulerRotation.z: 0
        eulerRotation.y: -0
        z: 201.37027
        eulerRotation.x: -3.46366
        fieldOfViewOrientation: PerspectiveCamera.Horizontal
        clipNear: 1
        fieldOfView: 43
    }

    // DirectionalLight { //灯光
    //     id: lightDirectional
    //     x: -1554.672
    //     y: -254.996
    //     brightness: 3.0
    //     eulerRotation: Qt.vector3d(-52.99892, -61.62584, 55.92922)
    //     // z: -4.38876
    // }
    DirectionalLight {
    }

    // EnvironmentLight {
    //     // brightness: 1.0
    //     irradiance: TextureLoader {
    //         source: "qrc:/textures/Textures/04-Gloucester-Beach.jpg"
    //     }
    //     specular: TextureLoader {
    //         source: "qrc:/textures/Textures/04-Gloucester-Beach.jpg"
    //     }
    // }


    // Timer {
    //     id: timer
    //     interval: 30
    //     running: true
    //     repeat: true
    //     onTriggered: {
    //         model.eulerRotation.y += 1;
    //     }
    // }

    Timer {
        id: boundsCheckTimer
        interval: 100
        repeat: true
        running: true
        onTriggered: {
            if (model.bounds !== undefined) {
                adjustModel();
                boundsCheckTimer.stop();
            } else {
                boundsCheckTimer.restart();
            }
        }
    }

    Component.onCompleted: {
        boundsCheckTimer.running = true; // 启动定时器
    }

    PrincipledMaterial {
        id: defaultMaterial_material
        objectName: "DefaultMaterial"
        baseColor: "#808080"
        indexOfRefraction: 1
        roughness: 0.1
    }
    function checkBounds() {
        if (model.bounds.valid) {
            adjustModel();
        } else {
            // 检查边界框每100毫秒一次，直到它们有效
            Qt.callLater(checkBounds);
        }
    }
    function adjustModel() {
        const bounds = model.bounds;
        const center = {
            x: (bounds.minimum.x + bounds.maximum.x) / 2,
            y: (bounds.minimum.y + bounds.maximum.y) / 2,
            z: (bounds.minimum.z + bounds.maximum.z) / 2
        };
        const size = Math.max(
            bounds.maximum.x - bounds.minimum.x,
            bounds.maximum.y - bounds.minimum.y,
            bounds.maximum.z - bounds.minimum.z
        );
        console.log("size = " + size);
        const scaleFactor = 50 / size; // 假设我们希望模型最大边长为200单位
        model.scale = Qt.vector3d(scaleFactor, scaleFactor, scaleFactor);
        console.log("scale = " + scaleFactor);
        model.position = Qt.vector3d(-center.x * scaleFactor, -center.y * scaleFactor, -center.z * scaleFactor);
    }

    Model {
        id: model
        source: view3d.modelSource
        // materials: [defaultMaterial] //材质设置
        scale: Qt.vector3d(1.0, 1.0, 1.0)
        onSourceChanged: {
            boundsCheckTimer.restart();  // 重新启动定时器来检查边界并调整模型
        }
        // Component.onCompleted: checkBounds()
        materials: [
            defaultMaterial_material
        ]
    }

    MouseArea {
        anchors.fill: parent
        property real lastX: 0
        property real lastY: 0
        property bool isPressAndHold: false // 标记是否是长按状态

        onPressed: function(mouse) {
            lastX = mouse.x;
            lastY = mouse.y;
            isPressAndHold = true; // 每次按下时重置长按状态
        }

        onPositionChanged: function(mouseEvent) {
            if (isPressAndHold) { // 仅在长按状态下处理旋转
                let dx = mouseEvent.x - lastX
                let dy = mouseEvent.y - lastY

                model.eulerRotation.y += dx / 5
                model.eulerRotation.x += dy / 5

                lastX = mouseEvent.x
                lastY = mouseEvent.y
            }
        }

        onPressAndHold: function() {
            isPressAndHold = true; // 长按时设置状态为true
        }

        onWheel: function(wheelEvent) {
            // const fovAdjustment = wheelEvent.angleDelta.y / 120
            // cameraPerspective.fieldOfView = Math.max(1, Math.min(cameraPerspective.fieldOfView - fovAdjustment, 179))
            const zoomSpeed = 2; // 控制缩放速度，这个值可以根据需要调整
            const delta = wheelEvent.angleDelta.y / 120;
            let newZ = cameraPerspective.z - (delta * zoomSpeed);

            // 控制缩放界限
            newZ = Math.max(50, Math.min(newZ, 500)); // 这里的50和500是摄像机z轴的最小值和最大值，根据需要调整

            cameraPerspective.z = newZ;
        }
    }
}