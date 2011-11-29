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

    File:        CameraVideoMode.cpp

    Maintainer:  Georg Hartinger
    Email:       georg.hartinger@ini.rub.de
    Date:        2011 08 01

    Description: Implementation of the CameraVideoMode enum-type class

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "CameraVideoMode.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES


cedar::aux::EnumType<cedar::dev::sensors::visual::CameraVideoMode>
            cedar::dev::sensors::visual::CameraVideoMode::mType("cedar::dev::sensors::visual::CameraVideoMode::");

#ifndef MSVC
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_NOT_SET;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_0 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_1 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_2 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_3 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_4 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_5 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_6 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_7 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_8 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_9 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_160x120_YUV444  ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_320x240_YUV422  ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_640x480_YUV411  ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_640x480_YUV422  ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_640x480_RGB8    ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_640x480_MONO8   ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_640x480_MONO16  ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_800x600_YUV422  ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_800x600_RGB8    ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_800x600_MONO8   ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_1024x768_YUV422 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_1024x768_RGB8   ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_1024x768_MONO8  ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_800x600_MONO16  ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_1024x768_MONO16 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_1280x960_YUV422 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_1280x960_RGB8   ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_1280x960_MONO8  ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_1600x1200_YUV422;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_1600x1200_RGB8  ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_1600x1200_MONO8 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_1280x960_MONO16 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_1600x1200_MONO16;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_EXIF ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_0 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_1 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_2 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_3 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_4 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_5 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_6 ;
const cedar::dev::sensors::visual::CameraVideoMode::Id cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_7 ;
#endif // MSVC


//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

void cedar::dev::sensors::visual::CameraVideoMode::construct()
{
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_NOT_SET,
                                      "MODE_NOT_SET",
                                      "The best available mode will be used"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_0,
                                      "MODE_BY_NUM_0",
                                      "First available mode"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_1,
                                      "MODE_BY_NUM_1",
                                      "Second available mode"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_2,
                                      "MODE_BY_NUM_2",
                                      "Third available mode"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_3,
                                      "MODE_BY_NUM_3",
                                      "Fourth available mode"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_4,
                                      "MODE_BY_NUM_4",
                                      "Fifth available mode"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_5,
                                      "MODE_BY_NUM_5",
                                      "Sixth available mode"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_6,
                                      "MODE_BY_NUM_6",
                                      "Seventh available mode"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_7,
                                      "MODE_BY_NUM_7",
                                      "Eighth available mode"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_8,
                                      "MODE_BY_NUM_8",
                                      "Nineth available mode"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_BY_NUM_9,
                                      "MODE_BY_NUM_9",
                                      "Tenth available mode"
                                    ));

  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_160x120_YUV444,
                                      "MODE_160x120_YUV444",
                                      "MODE_160x120_YUV444"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_320x240_YUV422,
                                      "MODE_320x240_YUV422",
                                      "MODE_320x240_YUV422"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_640x480_YUV411,
                                      "MODE_640x480_YUV411",
                                      "MODE_640x480_YUV411"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_640x480_YUV422,
                                      "MODE_640x480_YUV422",
                                      "MODE_640x480_YUV422"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_640x480_RGB8,
                                      "MODE_640x480_RGB8",
                                      "MODE_640x480_RGB8"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_640x480_MONO8,
                                      "MODE_640x480_MONO8",
                                      "MODE_640x480_MONO8"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_640x480_MONO16,
                                      "MODE_640x480_MONO16",
                                      "MODE_640x480_MONO16"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_800x600_YUV422,
                                      "MODE_800x600_YUV422",
                                      "MODE_800x600_YUV422"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_800x600_RGB8,
                                      "MODE_800x600_RGB8",
                                      "MODE_800x600_RGB8"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_800x600_MONO8,
                                      "MODE_800x600_MONO8",
                                      "MODE_800x600_MONO8"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_1024x768_YUV422,
                                      "MODE_1024x768_YUV422",
                                      "MODE_1024x768_YUV422"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_1024x768_RGB8,
                                      "MODE_1024x768_RGB8",
                                      "MODE_1024x768_RGB8"
                                    ));

  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_1024x768_MONO8,
                                      "MODE_1024x768_MONO8",
                                      "MODE_1024x768_MONO8"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_800x600_MONO16,
                                      "MODE_800x600_MONO16",
                                      "MODE_800x600_MONO16"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_1024x768_MONO16,
                                      "MODE_1024x768_MONO16",
                                      "MODE_1024x768_MONO16"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_1280x960_YUV422,
                                      "MODE_1280x960_YUV422",
                                      "MODE_1280x960_YUV422"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_1280x960_RGB8,
                                      "MODE_1280x960_RGB8",
                                      "MODE_1280x960_RGB8"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_1280x960_MONO8,
                                      "MODE_1280x960_MONO8",
                                      "MODE_1280x960_MONO8"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_1600x1200_YUV422,
                                      "MODE_1600x1200_YUV422",
                                      "MODE_1600x1200_YUV422"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_1600x1200_RGB8,
                                      "MODE_1600x1200_RGB8",
                                      "MODE_1600x1200_RGB8"
                                    ));

  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_1600x1200_MONO8,
                                      "MODE_1600x1200_MONO8",
                                      "MODE_1600x1200_MONO8"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_1280x960_MONO16,
                                      "MODE_1280x960_MONO16",
                                      "MODE_1280x960_MONO16"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_1600x1200_MONO16,
                                      "MODE_1600x1200_MONO16",
                                      "MODE_1600x1200_MONO16"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_EXIF,
                                      "MODE_EXIF",
                                      "MODE_EXIF"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_0,
                                      "MODE_FORMAT7_0",
                                      "MODE_FORMAT7_0"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_1,
                                      "MODE_FORMAT7_1",
                                      "MODE_FORMAT7_1"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_2,
                                      "MODE_FORMAT7_2",
                                      "MODE_FORMAT7_2"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_3,
                                      "MODE_FORMAT7_3",
                                      "MODE_FORMAT7_3"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_4,
                                      "MODE_FORMAT7_4",
                                      "MODE_FORMAT7_4"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_5,
                                      "MODE_FORMAT7_5",
                                      "MODE_FORMAT7_5"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_6,
                                      "MODE_FORMAT7_6",
                                      "MODE_FORMAT7_6"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraVideoMode::MODE_FORMAT7_7,
                                      "MODE_FORMAT7_7",
                                      "MODE_FORMAT7_7"
                                    ));


}
//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------


const cedar::aux::EnumBase& cedar::dev::sensors::visual::CameraVideoMode::type()
{
  return *cedar::dev::sensors::visual::CameraVideoMode::mType.type();
}

const cedar::dev::sensors::visual::CameraVideoMode::TypePtr& cedar::dev::sensors::visual::CameraVideoMode::typePtr()
{
  return cedar::dev::sensors::visual::CameraVideoMode::mType.type();
}
