import QtQuick
import QtQuick3D


Window {
    visible: true
    width: 800
    height: 600
    View3D {
        anchors.fill: parent
        environment: SceneEnvironment {
            backgroundMode: SceneEnvironment.Color
            clearColor: "white"
        }
        PerspectiveCamera {
            position: Qt.vector3d(0, 0, 100)
            eulerRotation.x: -45
        }
        Model {
            source: "E:/Code/Cpp_project/RSMesh/tools/meshes/defaultobject_mesh.mesh"  // 确保这个路径是正确的
            materials: [ defaultMaterial ]
        }

        PrincipledMaterial {
            id: defaultMaterial
            baseColor: "#ff999999"
            indexOfRefraction: 1
        }
    }
    
}

