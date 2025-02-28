import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Controls
import QtLocation
import QtPositioning
import org.kde.kirigami as Kirigami
import com.github.pamugk.urania as Urania

Kirigami.ApplicationWindow {
    id: root

    width: 800
    height: 600

    title: "Urania"

    Plugin {
        id: mapPlugin

        name: "osm"
    }

    ListModel {
        id: searchResultsModel
    }

    Urania.GeoApiClient {
        id: geoApiClient

        onSearchByNameCompleted: results => {
            searchResultsModel.clear();
            for (var i = 0; i < results.length; i++) {
                searchResultsModel.append({ "name": results[i] });
            }
        }
    }

    pageStack.initialPage: Kirigami.Page {
        padding: 0

        Map {
            anchors.fill: parent
            id: map

            plugin: mapPlugin
            center: QtPositioning.coordinate(55.751244, 37.618423)
            zoomLevel: 14
            property geoCoordinate startCentroid

            ColumnLayout {
                width: 352
                x: 16
                y: 8

                Kirigami.SearchField {
                    id: searchField

                    delaySearch: true

                    onAccepted: geoApiClient.searchByName(searchField.text)
                }

                Controls.ScrollView {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    ListView {
                        id: searchResultsView
                        model: searchResultsModel
                        delegate: Controls.Label {
                            required property string name

                            background: Rectangle {
                                color: Kirigami.Theme.backgroundColor
                            }
                            text: name
                            width: searchResultsView.width
                        }
                    }
                }
            }

            ColumnLayout {
                x: parent.width - width - 16
                y: (parent.height - height) / 2

                Controls.Button {
                    enabled: map.zoomLevel < map.maximumZoomLevel
                    icon.name: "zoom-in-map-symbolic"
                    onClicked: map.zoomLevel = Math.round(map.zoomLevel + 1)
                }
                Controls.Button {
                    enabled: map.zoomLevel > map.minimumZoomLevel
                    icon.name: "zoom-out-map-symbolic"
                    onClicked: map.zoomLevel = Math.round(map.zoomLevel - 1)
                }
            }

            PinchHandler {
                id: pinch

                grabPermissions: PointerHandler.TakeOverForbidden
                target: null

                onActiveChanged: if (active) {
                    map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
                }
                onScaleChanged: (delta) => {
                    map.zoomLevel += Math.log2(delta)
                    map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                }
                onRotationChanged: (delta) => {
                    map.bearing -= delta
                    map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
                }
            }
            WheelHandler {
                id: wheel

                acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                    ? PointerDevice.Mouse | PointerDevice.TouchPad
                    : PointerDevice.Mouse
                rotationScale: 1/120
                property: "zoomLevel"
            }
            DragHandler {
                id: drag

                target: null
                onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
            }
            Shortcut {
                enabled: map.zoomLevel < map.maximumZoomLevel
                sequence: StandardKey.ZoomIn
                onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
            }
            Shortcut {
                enabled: map.zoomLevel > map.minimumZoomLevel
                sequence: StandardKey.ZoomOut
                onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
            }
        }
    }
}
