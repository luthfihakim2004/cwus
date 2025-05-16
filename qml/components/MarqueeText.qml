import QtQuick 2.15

Item {
    id: root
    implicitHeight: textItem.implicitHeight
    width: parent ? parent.width : implicitWidth
    height: implicitHeight

    property alias text: textItem.text
    property alias font: textItem.font
    property alias color: textItem.color
    property int spacing: 20  // Gap between text and its duplicate for seamless looping

    // Container that masks and scrolls
    Item {
        id: textContainer
        width: root.width
        height: root.height
        clip: true

        //  Moving text
        Item {
            id: contentItem
            height: root.height
            width: textItem.width + spacing + duplicateText.width
            x: 0

            // Text #1
            Text {
                id: textItem
                text: ""
                font: root.font
                anchors.verticalCenter: parent.verticalCenter
            }

            // Text #2 (for seamless looping)
            Text {
                id: duplicateText
                text: textItem.text
                font: root.font
                color: textItem.color
                x: textItem.width + spacing
                anchors.verticalCenter: parent.verticalCenter
                visible: textItem.width > root.width
            }

            // The animation
            NumberAnimation on x {
                id: scrollAnim
                from: 0
                to: -textItem.width - spacing
                duration: (textItem.width + spacing) * 30
                loops: Animation.Infinite
                running: textItem.width > root.width
            }
        }
    }

    // Handle new text
    onTextChanged: {
        if (textItem.width > root.width) {
            scrollAnim.stop()
            contentItem.x = 0
            scrollAnim.start()
        } else {
            scrollAnim.stop()
            contentItem.x = 0
        }
    }
}
