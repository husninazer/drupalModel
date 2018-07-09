import QtQuick 2.9
import QtQuick.Window 2.2

import DrupalModel 0.1

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    DrupalModel {
            id: drupalModel
            onCountChanged: {
                console.log("COUNT CHANGED..................")
            }


    }

    ListView {
        anchors.fill: parent
        model: drupalModel
        delegate: Row {
//            Text {
//                id: title
//                text: index + '    ' + model.title.value
//                color: 'red'
//            }

//            Text {
//                id: title2
//                text: model.body.value
//                color: 'grey'
//            }

            Text {
                id: field_asd
                text: field_project_category.value
                color: 'green'
            }

//            Text {
//                id: field_ss
//                text: model.field_top_image.url
//                color: 'blue'
//            }

//            Image {
//                id: name
//                source: model.field_top_image.url
//            }
        }
    }
}

