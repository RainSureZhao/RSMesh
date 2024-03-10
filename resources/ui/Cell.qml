// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

//![0]
import QtQuick

//![1]
Item {
    id: container

    //![4]
    property alias cellColor: rectangle.color

    //![4]
    //![5]
    signal clicked(color cellColor)

    height: 25
    //![5]

    width: 40

    //![1]

    //![2]
    Rectangle {
        id: rectangle

        anchors.fill: parent
        border.color: "white"
    }
    //![2]

    //![3]
    MouseArea {
        anchors.fill: parent

        onClicked: container.clicked(container.cellColor)
    }
    //![3]
}
//![0]
