/*
 * Copyright 2016-2017 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQml.Models 2.2
import QtGraphicalEffects 1.0

FocusScope {
    id: rootFocusScope

    property alias currentIndex: viewModeView.currentIndex
    property double textOpacity
    property double maximumSize

    signal switchView(int index)

    implicitWidth: elisaTheme.dp(500)

    Rectangle {
        anchors.fill: parent

        color: myPalette.base
        border {
            color: (rootFocusScope.activeFocus ? myPalette.highlight : "transparent")
            width: 1
        }

        ScrollView {
            focus: true

            anchors.fill: parent

            clip: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff


            ListView {
                id: viewModeView

                focus: true
                z: 2

                anchors.topMargin: elisaTheme.layoutHorizontalMargin * 2

                model: DelegateModel {
                    id: pageDelegateModel

                    model: ListModel {
                        id: pageModel
                    }

                    delegate: MouseArea {
                        id: itemMouseArea

                        height: elisaTheme.viewSelectorDelegateHeight * 1.4
                        width: viewModeView.width

                        hoverEnabled: true
                        acceptedButtons: Qt.LeftButton

                        Loader {
                            anchors.fill: parent
                            active: itemMouseArea && itemMouseArea.containsMouse && !nameLabel.visible

                            sourceComponent: ToolTip {
                                delay: Qt.styleHints.mousePressAndHoldInterval
                                text: nameLabel.text
                                visible: itemMouseArea && itemMouseArea.containsMouse && !nameLabel.visible

                                contentItem: Label {
                                    text: nameLabel.text
                                    color: myPalette.highlightedText
                                }

                                enter: Transition { NumberAnimation { properties: "opacity"; easing.type: Easing.InOutQuad; from: 0.0; to: 1.0; duration: 300; } }
                                exit: Transition { NumberAnimation { properties: "opacity"; easing.type: Easing.InOutQuad; from: 1.0; to: 0.0; duration: 300; } }

                                background: Rectangle {
                                    color: myPalette.shadow
                                    radius: elisaTheme.tooltipRadius

                                    layer.enabled: true
                                    layer.effect: DropShadow {
                                        horizontalOffset: elisaTheme.shadowOffset
                                        verticalOffset: elisaTheme.shadowOffset
                                        radius: 8
                                        samples: 17
                                        color: myPalette.shadow
                                    }
                                }
                            }
                        }

                        Image {
                            id: viewIcon

                            z: 2

                            anchors {
                                verticalCenter: parent.verticalCenter
                                leftMargin: elisaTheme.layoutHorizontalMargin
                                left: parent.left
                            }

                            height: elisaTheme.viewSelectorDelegateHeight
                            width: elisaTheme.viewSelectorDelegateHeight

                            sourceSize {
                                width: elisaTheme.viewSelectorDelegateHeight
                                height: elisaTheme.viewSelectorDelegateHeight
                            }

                            source: iconName

                            visible: false
                        }

                        ColorOverlay {
                            source: viewIcon

                            z: 2

                            anchors {
                                verticalCenter: parent.verticalCenter
                                leftMargin: elisaTheme.layoutHorizontalMargin
                                left: parent.left
                                rightMargin: nameLabel.visible ? 0 : elisaTheme.layoutHorizontalMargin
                            }

                            height: elisaTheme.viewSelectorDelegateHeight
                            width: elisaTheme.viewSelectorDelegateHeight

                            color: (index === viewModeView.currentIndex || itemMouseArea.containsMouse ? myPalette.highlight : "transparent")

                            Behavior on color {
                                ColorAnimation {
                                    duration: 300
                                }
                            }
                        }

                        LabelWithToolTip {
                            id: nameLabel

                            z: 2

                            anchors.verticalCenter: parent.verticalCenter
                            anchors.leftMargin: elisaTheme.layoutHorizontalMargin
                            anchors.left: viewIcon.right
                            anchors.right: parent.right
                            anchors.rightMargin: elisaTheme.layoutHorizontalMargin
                            verticalAlignment: "AlignVCenter"

                            font.pointSize: elisaTheme.defaultFontPointSize * 1.4

                            text: model.name
                            elide: Text.ElideRight

                            opacity: textOpacity
                            visible: opacity > 0

                            Behavior on opacity {
                                NumberAnimation {
                                    duration: 150
                                }
                            }

                            color: (viewModeView.currentIndex === index || itemMouseArea.containsMouse ? myPalette.highlight : myPalette.text)

                            Behavior on color {
                                ColorAnimation {
                                    duration: 300
                                }
                            }
                        }

                        onClicked:
                        {
                            viewModeView.currentIndex = index
                            rootFocusScope.focus = true
                            switchView(index)
                        }
                    }
                }

                footer: MouseArea {
                    width: viewModeView.width
                    height: viewModeView.height - y

                    acceptedButtons: Qt.LeftButton

                    onClicked:
                    {
                        rootFocusScope.focus = true
                    }
                }
            }
        }

        Behavior on border.color {
            ColorAnimation {
                duration: 300
            }
        }
    }

    Connections {
        target: elisa

        onInitializationDone: {
            pageModel.insert(0, {"name": i18nc("Title of the view of the playlist", "Now Playing"), "iconName": elisaTheme.playlistIcon})
            pageModel.insert(1, {"name": i18nc("Title of the view of all albums", "Albums"), "iconName": elisaTheme.albumIcon})
            pageModel.insert(2, {"name": i18nc("Title of the view of all artists", "Artists"), "iconName": elisaTheme.artistIcon})
            pageModel.insert(3, {"name": i18nc("Title of the view of all tracks", "Tracks"), "iconName": elisaTheme.tracksIcon})
            pageModel.insert(4, {"name": i18nc("Title of the view of all genres", "Genres"), "iconName": elisaTheme.genresIcon})

            console.log(elisa.fileBrowserProxyModel)

            if (elisa.fileBrowserProxyModel) {
                pageModel.insert(5, {"name": i18nc("Title of the file browser view", "Files"), "iconName": elisaTheme.folderIcon})
            }

            viewModeView.currentIndex = 1
            switchView(1)
        }
    }

    Behavior on implicitWidth {
        NumberAnimation {
            duration: 150
        }
    }

    Behavior on width {
        NumberAnimation {
            duration: 150
        }
    }

    states: [
        State {
            name: 'iconsAndText'
            when: maximumSize >= elisaTheme.viewSelectorSmallSizeThreshold
            PropertyChanges {
                target: rootFocusScope
                textOpacity: 1
                implicitWidth: elisaTheme.dp(500)
            }
        },
        State {
            name: 'iconsOnly'
            when: maximumSize < elisaTheme.viewSelectorSmallSizeThreshold
            PropertyChanges {
                target: rootFocusScope
                textOpacity: 0
                implicitWidth: elisaTheme.viewSelectorDelegateHeight + 2 * elisaTheme.layoutHorizontalMargin
            }
        }
    ]
}
