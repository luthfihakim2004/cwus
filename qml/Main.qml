import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Cwus 1.0  // Your registered module
import "components"
import org.kde.layershell 1.0 as LayerShell

Window {
    title: "CWUS"
    color: "transparent"
    flags: Qt.FramelessWindowHint
    // Assuming LayerShell is registered and works
    LayerShell.Window.layer: LayerShell.Window.LayerOverlay
    LayerShell.Window.anchors: LayerShell.Window.AnchorTop | LayerShell.Window.AnchorLeft
    LayerShell.Window.exclusionZone: 0


    property bool expanded: false
    Behavior on height{
        NumberAnimation { duration: 250; easing.type: Easing.InOutQuad }
    }

    Controller {
        id: ctrl
    }

    width: 450
    height: expanded ? 200 : 40

    Rectangle {
        anchors.fill: parent
        Binding on color {
            value: themeLoader.background
        }
        radius: 15

        clip: true

        Rectangle {
            id: header
            width: parent.width
            height: 40
            Binding on color {
                value: themeLoader.accent
            }
            radius: 15
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            MouseArea {
                anchors.fill: parent
                onClicked: expanded = !expanded
            }

            Item {
                anchors.fill: parent

                Text {
                    visible: expanded
                    anchors.centerIn: parent
                    text: "CWUS"
                    font.pixelSize: 18
                    font.bold: true
                    Binding on color {
                        value: themeLoader.foreground
                    }
                }

                MarqueeText {
                    visible: !expanded
                    anchors.fill: parent
                    anchors.leftMargin: 15
                    anchors.rightMargin: 15
                    text: ctrl.title ? ctrl.title + " - " + ctrl.artist : "CWUS"
                    font.pixelSize: 18
                    font.bold: true
                    Binding on color {
                        value: themeLoader.foreground
                    }
                }
            }
        }

        Item{
            id: contentWrapper
            visible: expanded
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            Layout.fillWidth: true
            height: 140

            Item {
                anchors.fill: parent
                //width: Math.max(300, parent.width - 20)  // Stretches but keeps minimum width for floating window
                height: mainLayout.implicitHeight

                ColumnLayout {
                    id: mainLayout
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 5

                    // Title Marquee
                    MarqueeText {
                        Layout.fillWidth: true
                        text: ctrl.title ? ctrl.title : "CMUS is stopped"
                        font.pixelSize: 20
                        font.bold: true
                        Layout.alignment: Text.AlignHCenter
                        Binding on color {
                            value: themeLoader.foreground
                        }
                    }

                    Label {
                        text: ctrl.artist
                        font.pixelSize: 15
                        Binding on color {
                            value: themeLoader.foreground
                        }
                    }

                    Label {
                        text: ctrl.album
                        wrapMode: Text.Wrap
                        Layout.fillWidth: true
                        font.italic: true
                        font.pixelSize: 15
                        Binding on color {
                            value: themeLoader.foreground
                        }
                    }

                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 10

                        RoundButton {
                            id: prevBtn
                            Layout.preferredWidth: 35
                            Layout.preferredHeight: 35
                            flat: true
                            width: 35
                            height: width
                            Text{
                                anchors.centerIn: parent
                                text: "󰒮"
                                Binding on color {
                                    value: prevBtn.down ? themeLoader.darkaccent : (prevBtn.hovered ? themeLoader.background : themeLoader.foreground)
                                }
                                font.family: "JetBrainsMonoNL Nerd Font"
                                font.pixelSize: 20
                            }

                            onClicked: ctrl.prevTrack()

                            background: Rectangle {
                                Binding on color {
                                    value: prevBtn.down ? themeLoader.foreground : (prevBtn.hovered ? themeLoader.foreground : "transparent")
                                }
                                Binding on border.color {
                                    value: themeLoader.foreground
                                }
                                border.width: 1
                                radius: height / 2

                                Behavior on color {
                                    ColorAnimation { duration: 150 }
                                }
                            }
                        }

                        RoundButton {
                            id: playBtn
                            Layout.preferredWidth: 35
                            Layout.preferredHeight: 35
                            flat: true
                            width: 35
                            height: width
                            Text{
                                anchors.centerIn: parent
                                text: "󰐎"
                                Binding on color {
                                    value: playBtn.down ? themeLoader.darkaccent : (playBtn.hovered ? themeLoader.background : themeLoader.foreground)
                                }
                                font.family: "JetBrainsMonoNL Nerd Font"
                                font.pixelSize: 20
                            }

                            onClicked: ctrl.playPause()

                            background: Rectangle {
                                Binding on color {
                                    value: playBtn.down ? themeLoader.foreground : (playBtn.hovered ? themeLoader.foreground : "transparent")
                                }
                                Binding on border.color {
                                    value: themeLoader.foreground
                                }
                                border.width: 1
                                radius: height / 2

                                Behavior on color {
                                    ColorAnimation { duration: 150 }
                                }
                            }
                        }

                        RoundButton {
                            id: nextBtn
                            Layout.preferredWidth: 35
                            Layout.preferredHeight: 35
                            flat: true
                            width: 35
                            height: width
                            Text{
                                anchors.centerIn: parent
                                text: "󰒭"
                                Binding on color {
                                    value: nextBtn.down ? themeLoader.darkaccent : (nextBtn.hovered ? themeLoader.background : themeLoader.foreground)
                                }
                                font.family: "JetBrainsMonoNL Nerd Font"
                                font.pixelSize: 20
                            }


                            onClicked: ctrl.nextTrack()

                            background: Rectangle {
                                Binding on color {
                                    value: nextBtn.down ? themeLoader.foreground : (nextBtn.hovered ? themeLoader.foreground : "transparent")
                                }
                                Binding on border.color {
                                    value: themeLoader.foreground
                                }
                                border.width: 1
                                radius: height / 2

                                Behavior on color {
                                    ColorAnimation { duration: 150 }
                                }
                            }
                        }
                    }

                    Slider {
                        id : control
                        from: 0
                        to: ctrl.duration
                        value: ctrl.position
                        onMoved: ctrl.progressTrack(value)
                        Layout.fillWidth: true

                        handle: Rectangle{
                            width: 10
                            height: 10
                            radius: 5
                            x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
                            y: control.topPadding + (control.availableHeight - height * scaleEffect.yScale) / 2

                            transformOrigin: Item.Center  // Ensures scale happens from center
                            transform: Scale {
                                id: scaleEffect
                                xScale: hover.hovered ? 2 : 1
                                yScale: hover.hovered ? 2 : 1
                            }

                            HoverHandler{
                                id: hover
                                acceptedDevices: PointerDevice.AllDevices
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Text {
                            text: ctrl.timeStart
                            Binding on color {
                                value: themeLoader.foreground
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        Text {
                            text: ctrl.timeEnd
                            Binding on color {
                                value: themeLoader.foreground
                            }
                        }
                    }
                }
            }
        }
    }
    visible: true
}
