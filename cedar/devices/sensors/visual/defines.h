/*======================================================================================================================

    Copyright 2011 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany

    This file is part of cedar.

    cedar is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    cedar is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with cedar. If not, see <http://www.gnu.org/licenses/>.

========================================================================================================================

    Institute:   Ruhr-Universitaet Bochum
                 Institut fuer Neuroinformatik

    File:        defines.h

    Maintainer:  Georg.Hartinger
    Email:       georg.hartinger@ini.rub.de
    Date:        2011 08 01

    Description: Defines for @em cedar::devices::sensors::visual.

    Credits:

======================================================================================================================*/


#ifndef CEDAR_DEV_SENSORS_VISUAL_DEFINES_H
#define CEDAR_DEV_SENSORS_VISUAL_DEFINES_H

//------------------------------------------------------------------------
// Defines the default file formats of recordings and snapshot.
// This extension will only be used, if there is no extension
// in the new name parameter given to setSnapshotName() or setRecordName()
//------------------------------------------------------------------------
#define GRABBER_DEFAULT_RECORD_EXTENSION ".avi"
#define GRABBER_DEFAULT_SNAPSHOT_EXTENSION ".jpg"

//------------------------------------------------------------------------
// Defines the additions to the filename.
// Will be used in a stereo-grabber if you use setSnapshotName(string)
// or setRecordName(string) to set both filenames at once
//------------------------------------------------------------------------
#define GRABBER_SAVE_FILENAMES_ADDITION_CHANNEL_0 "[ch0]"
#define GRABBER_SAVE_FILENAMES_ADDITION_CHANNEL_1 "[ch1]"

//------------------------------------------------------------------------
// Defines the how often getFpsMeasured() will be updated (in frames).
// Default value will be every 5 frames
//------------------------------------------------------------------------
#define UPDATE_FPS_MEASURE_FRAME_COUNT 5

//------------------------------------------------------------------------
// Enable the CTRL-C handler.
// If CTRL-C handler is activated, also some additions to the
// GrabberInterface will be enabled:
// 1) a static member vector to store pointer to all instances of created grabbers
// 2) a static member function which handle the interrupt signal
// 3) some additional code in GrabberInterface.cpp to handle these issues
//------------------------------------------------------------------------
#define ENABLE_CTRL_C_HANDLER


//------------------------------------------------------------------------
// CameraGrabber default values
//------------------------------------------------------------------------
// Default filename for the config-file of the camera capabilities
#define CAMERAGRABBER_CAM_CAPABILITIES_FILENAME(GUID) ("camera_"+boost::lexical_cast<string>(GUID)+".capabilities")

// default values used when the value in the configuration interface doesn't match
#define CAMERA_DEFAULT_FRAMERATE CameraFrameRate::FRAMERATE_15
#define CAMERA_DEFAULT_ISO_SPEED CameraIsoSpeed::ISO_200

// If cameras opened by guid, first every camera have to be opened in order
// to search their bus-ID
// After this search, all cameras will be closed and reopened by their bus-ID
// Between this two steps a short pause should be inserted to settle the cameras
// This value defines the time in seconds
#define CAMERA_GRABBER_WAIT_ON_OPEN_BYGUID 5

//------------------------------------------------------------------------
// Enable this, if you want to see some informations on initialization
//------------------------------------------------------------------------
#define SHOW_INIT_INFORMATION_VIDEOGRABBER
#define SHOW_INIT_INFORMATION_NETGRABBER
#define SHOW_INIT_INFORMATION_PICTUREGRABBER
#define SHOW_INIT_INFORMATION_CAMERAGRABBER

//------------------------------------------------------------------------
// Enable some warning output during operation
//------------------------------------------------------------------------
#define ENABLE_GRABBER_WARNING_OUTPUT

//------------------------------------------------------------------------
// Enable class-debugging output
//------------------------------------------------------------------------

#define DEBUG_GRABBER_INTERFACE
//#define DEBUG_GRABBER_STEP

#define DEBUG_NETGRABBER
#define DEBUG_VIDEOGRABBER
#define DEBUG_PICTUREGRABBER
#define DEBUG_CAMERAGRABBER

#endif

