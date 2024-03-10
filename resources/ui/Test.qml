// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

//![0]
import QtQuick

Window {
    id: window
    width: 1280
    height: 720
    visible: true
    Rectangle {
        id: page

        color: "lightgray"
        height: 480
        width: 320

        Text {
            id: helloText

            anchors.horizontalCenter: page.horizontalCenter
            font.bold: true
            font.pointSize: 24
            text: "Hello world!"
            y: 30
        }
        Grid {
            id: colorPicker

            anchors.bottom: page.bottom
            anchors.bottomMargin: 4
            columns: 3
            rows: 2
            spacing: 3
            x: 4

            //![1]
            Cell {
                cellColor: "red"

                onClicked: function (cellColor) {
                    helloText.color = cellColor;
                }
            }
            //![1]
            Cell {
                cellColor: "green"

                onClicked: function (cellColor) {
                    helloText.color = cellColor;
                }
            }
            Cell {
                cellColor: "blue"

                onClicked: function (cellColor) {
                    helloText.color = cellColor;
                }
            }
            Cell {
                cellColor: "yellow"

                onClicked: function (cellColor) {
                    helloText.color = cellColor;
                }
            }
            Cell {
                cellColor: "steelblue"

                onClicked: function (cellColor) {
                    helloText.color = cellColor;
                }
            }
            Cell {
                cellColor: "black"

                onClicked: function (cellColor) {
                    helloText.color = cellColor;
                }
            }
        }
    }
    //![0]
}


