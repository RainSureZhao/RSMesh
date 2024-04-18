import QtQuick
import QtQuick3D

Node {
    id: node

    // Resources
    PrincipledMaterial {
        id: defaultMaterial_material
        objectName: "DefaultMaterial"
        baseColor: "#ff999999"
        indexOfRefraction: 1
    }

    // Nodes:
    Node {
        id: bunny_obj
        objectName: "Bunny.obj"
        Model {
            id: defaultobject
            objectName: "defaultobject"
            source: "meshes/defaultobject_mesh.mesh"
            materials: [
                defaultMaterial_material
            ]
        }
    }

    // Animations:
}
