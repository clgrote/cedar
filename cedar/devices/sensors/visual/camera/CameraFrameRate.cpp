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

    File:        CameraFrameRate.cpp

    Maintainer:  Georg Hartinger
    Email:       georg.hartinger@ini.rub.de
    Date:        2011 08 01

    Description: Implementation of the CameraFrameRate enum-type class

    Credits:

======================================================================================================================*/

#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/devices/sensors/visual/camera/CameraFrameRate.h"

// SYSTEM INCLUDES

cedar::aux::EnumType<cedar::dev::sensors::visual::CameraFrameRate>
            cedar::dev::sensors::visual::CameraFrameRate::mType("cedar::dev::sensors::visual::CameraFramerate::");

//!@cond SKIPPED_DOCUMENTATION
#ifndef CEDAR_COMPILER_MSVC
const cedar::dev::sensors::visual::CameraFrameRate::Id cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_NOT_SET;
const cedar::dev::sensors::visual::CameraFrameRate::Id cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_2;
const cedar::dev::sensors::visual::CameraFrameRate::Id cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_4;
const cedar::dev::sensors::visual::CameraFrameRate::Id cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_8;
const cedar::dev::sensors::visual::CameraFrameRate::Id cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_15;
const cedar::dev::sensors::visual::CameraFrameRate::Id cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_30;
const cedar::dev::sensors::visual::CameraFrameRate::Id cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_60;
const cedar::dev::sensors::visual::CameraFrameRate::Id cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_120;
const cedar::dev::sensors::visual::CameraFrameRate::Id cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_240;
#endif // CEDAR_COMPILER_MSVC
//!@endcond

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

void cedar::dev::sensors::visual::CameraFrameRate::construct()
{
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_NOT_SET,
                                      "FRAMERATE_NOT_SET",
                                      "auto"
                                    ));

  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_2,
                                      "FRAMERATE_1_875",
                                      "1.875 fps"
                                    ));

  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_4,
                                      "FRAMERATE_3_75",
                                      "3.75 fps"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_8,
                                      "FRAMERATE_7_5",
                                      "7.5 fps"
                                     ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_15,
                                      "FRAMERATE_15",
                                      "15 fps"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_30,
                                      "FRAMERATE_30",
                                      "30 fps"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_60,
                                      "FRAMERATE_60",
                                      "60 fps"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_120,
                                      "FRAMERATE_120",
                                      "120 fps"
                                    ));
  mType.type()->def(cedar::aux::Enum(
                                      cedar::dev::sensors::visual::CameraFrameRate::FRAMERATE_240,
                                      "FRAMERATE_240",
                                      "240 fps"
                                    ));
}
//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------


const cedar::aux::EnumBase& cedar::dev::sensors::visual::CameraFrameRate::type()
{
  return *cedar::dev::sensors::visual::CameraFrameRate::mType.type();
}

const cedar::dev::sensors::visual::CameraFrameRate::TypePtr& cedar::dev::sensors::visual::CameraFrameRate::typePtr()
{
  return cedar::dev::sensors::visual::CameraFrameRate::mType.type();
}
