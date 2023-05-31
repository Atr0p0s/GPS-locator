import QtQuick
import QtQuick.Window
import QtPositioning
import QtQuick.Controls
import QtQuick.Dialogs
import com.myinc.SaveGPS

Window {
//    width: 640
//    height: 480
    visible: true
    title: "GPS-locator"

    Column {
        anchors.centerIn: parent
        anchors.margins: 10
        spacing: 3

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            property string txt: posSource.active ? "Running" : "Stopped"
            text: "Status: <b>" + txt + "</b>"
        }
        Text {
            topPadding: 10
            id: lat
            property string latitude
            text: "Latitude: " + latitude
        }

        Text {
            id: lng
            property string longitude
            text: "Longitude: " + longitude
            anchors.bottomMargin: 20
        }
        Text {
            id: fileName
            property string clr: "#000000"
            property string txt: "Not Selected"
//            text: "Selected file: " + txt
            text: "Selected file: <font color=\"%1\">%2</font>".arg(clr).arg(txt)
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Choose File"
            onClicked: saveDialog.open()
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            topPadding: 10
            spacing: 10
            Button {
                id: startButton
                text: "Start"
                onClicked: {
                    if (saveDialog.selectedFile != "." + saveDialog.defaultSuffix && saveDialog.selectedFile != "") {
                        savegps.openFile(saveDialog.selectedFile)
                        savegps.opened = true
                        posSource.active = true
                        fileName.clr = "#000000"
                    } else {
                        fileName.clr = "#FF0000"
                    }
                }
            }
            Button {
                id: stopButton
                text: "Stop"
                onClicked: {
                    if (savegps.opened === true) {
                        posSource.active = false
                        savegps.closeFile()
                        savegps.opened = false
                    }
                }
            }
        }
    }

    PositionSource {
        id: posSource
        name: "geoclue2"
        updateInterval: 1000
        active: false
        preferredPositioningMethods: PositionSource.SatellitePositioningMethods
        onPositionChanged: {
            var coord = posSource.position;
//            console.log(coord.timestamp);
//            console.log("Coordinate: ", coord.coordinate.longitude, coord.coordinate.latitude);

            lat.latitude = coord.coordinate.latitude;
            lng.longitude = coord.coordinate.longitude;
//            var data = coord.timestamp + "," + coord.coordinate.latitude + "," + coord.coordinate.longitude + "\n";
//            var data = "%1,%2,%3\n".arg(coord.timestamp).arg(coord.coordinate.latitude).arg(coord.coordinate.longitude)
//            console.log(data);
            savegps.write(coord.timestamp, coord.coordinate.latitude, coord.coordinate.longitude);
        }
    }

    FileDialog {
        id: saveDialog
        fileMode: FileDialog.SaveFile
        defaultSuffix: "nmea"
        nameFilters: [ "NMEA File(*.nmea)", "All files (*)" ]
        onAccepted: {
            fileName.clr = "#000000"
            fileName.txt = String(selectedFile).slice(String(selectedFile).lastIndexOf("/")+1)
        }
    }

    SaveGPS {
        id: savegps
        property bool opened: false
        onError: function (message) {
            if (savegps.opened === true) {
                posSource.active = false
                savegps.closeFile()
                savegps.opened = false
            }
        }
    }

    onClosing: {
        if (savegps.opened === true) {
            posSource.active = false
            savegps.closeFile()
            savegps.opened = false
        }
    }
}
