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

    File:        CameraBackendType.h

    Maintainer:  Georg Hartinger
    Email:       georg.hartinger@ini.rub.de
    Date:        2012 07 04

    Description:  Header for CameraBackendType enum-type class

    Credits:

======================================================================================================================*/

#ifndef CEDAR_CEDAR_DEV_SENSORS_VISUAL_CAMERA_CAMERABACKENDTYPE_H
#define CEDAR_CEDAR_DEV_SENSORS_VISUAL_CAMERA_CAMERABACKENDTYPE_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/EnumType.h"
#include "cedar/devices/sensors/visual/namespace.h"

// SYSTEM INCLUDES

/*!@brief Enum class to determine the used backend for the camera grabber
 */
class cedar::dev::sensors::visual::CameraBackendType
{
  //--------------------------------------------------------------------------------------------------------------------
  // typedefs
  //--------------------------------------------------------------------------------------------------------------------
//!@cond SKIPPED_DOCUMENTATION
public:
  typedef cedar::aux::EnumId Id;
public:
  typedef boost::shared_ptr<cedar::aux::EnumBase> TypePtr;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.

  //!@brief Destructor

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  static void construct();

  static const cedar::aux::EnumBase& type();
  static const cedar::dev::sensors::visual::CameraBackendType::TypePtr& typePtr();

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  static cedar::aux::EnumType<cedar::dev::sensors::visual::CameraBackendType> mType;
  //!@endcond

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
public:

  /*! @brief Use the default value of the OpenCV::VideoCapture object
   *
   *  It is possible, that no all properties could be detected
   */
  static const Id AUTO = 0;

  /*! @brief Use only basic functionality of the cv::Capture object
   */
  static const Id CVCAPTURE = 1;

#ifdef CEDAR_USE_LIB_DC1394
  /*! @brief Use the DC1394 backend and settings
   *
   *   This is only possible if CEDAR is build with libdc support
   */
  static const Id DC1394 = 2;
#endif //CEDAR_USE_LIB_DC1394

  /*! @brief Use the Video For Linux backend
   */
  static const Id VFL = 3;



protected:
  // none yet
private:
  // none yet

}; // cedar::dev::sensors::visual::CameraBackendType


#endif // CEDAR_CEDAR_DEV_SENSORS_VISUAL_CAMERA_CAMERABACKENDTYPE_H

