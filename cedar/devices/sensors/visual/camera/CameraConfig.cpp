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

    File:        CameraConfig.cpp

    Maintainer:  Georg.Hartinger
    Email:       georg.hartinger@ini.rub.de
    Date:        2011 12 01

    Description: Implementation of the @em cedar::devices::visual::CameraConfig class.

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "cedar/devices/sensors/visual/camera/CameraConfig.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <boost/lexical_cast.hpp> //for reading values from the configfile

using namespace cv;
using namespace cedar::dev::sensors::visual;

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
CameraConfig::CameraConfig(
                            const std::string& configFileName,
                            unsigned int channel,
                            CameraSettings &camSettings,
                            CameraPropertyValues &camPropertyValues
                          )
:
ConfigurationInterface(configFileName),
mCamSettings(camSettings),
mCamPropertyValues(camPropertyValues)
{
  #ifdef DEBUG_CAMERAGRABBER
    std::cout << "[CameraConfig::CameraState] channel "<< channel << " Config-file: " << configFileName << std::endl;
  #endif

    mChannel = channel;
    mChannelPrefix = "ch"+boost::lexical_cast<std::string>(channel)+"_";

    bool result = declareParameter();
    ConfigurationInterface::readOrDefaultConfiguration();

    if (not result )
    {
      std::string err = "[CameraCapabilities::CameraCapabilities] channel "
                        + boost::lexical_cast<std::string>(channel)
                        + " - Critical error in constructor";

      //throwing an exception in a shared-pointer managed class is catched by shared-pointer
      std::cout << err << std::endl;
      CEDAR_THROW(cedar::aux::exc::InitializationException,err);
    }
}

CameraConfig::~CameraConfig()
{
  #ifdef DEBUG_CAMERAGRABBER
    std::cout << "[CameraConfig::~CameraState] channel "<< mChannel << " Destroy class" << std::endl;
  #endif
}


//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

bool CameraConfig::saveConfiguration()
{
  #ifdef DEBUG_CAMERAGRABBER
    std::cout << "[CameraConfig::writeConfiguration] channel "<< mChannel << std::endl;
  #endif

  ConfigurationInterface::writeConfiguration();
  return true;
}

//----------------------------------------------------------------------------------------------------
bool CameraConfig::declareParameter()
{
  //map local buffer of camera properties and settings
  //to the ConfigurationInterface class
  std::string param_value;
  std::string param_name;
  bool result = true;

  // -------------------------------
  // settings
  // -------------------------------
  param_value = CameraVideoMode::type().get(CameraVideoMode::MODE_NOT_SET).name();
  param_name = mChannelPrefix + "cam_mode";
  result = (addParameter(&mCamSettings.mode, param_name, param_value) == CONFIG_SUCCESS) && result;
  //std::cout << "set videomode to default: " << param_value << std::endl;

  param_value = CameraFrameRate::type().get(CameraFrameRate::FRAMERATE_NOT_SET).name();
  param_name = mChannelPrefix + "cam_framerate";
  result = (addParameter(&mCamSettings.fps, param_name, param_value) == CONFIG_SUCCESS) && result;
  //std::cout << "set FPS to default: " << param_value << std::endl;

  param_value = CameraIsoSpeed::type().get(CameraIsoSpeed::ISO_NOT_SET).name();
  param_name = mChannelPrefix + "cam_iso_speed";
  result = (addParameter(&mCamSettings.iso_speed, param_name, param_value) == CONFIG_SUCCESS) && result;

  // -------------------------------
  // properties
  // -------------------------------
  CameraPropertyValues::iterator it;
  for (it = mCamPropertyValues.begin(); it != mCamPropertyValues.end(); it++)
  {
    double *param_adr = &(it->second);
    cedar::aux::EnumId param_id = static_cast<cedar::aux::EnumId> (it->first);
    std::string param_name = mChannelPrefix + CameraProperty::type().get(param_id).name();
    double param_old_val = it->second;

    //std::stringstream param_name_ch;
    //param_name_ch<<"ch"<<channel<<"_"<<param_name;

    result = (addParameter(param_adr, param_name, CAMERA_PROPERTY_MODE_AUTO) == CONFIG_SUCCESS) && result;

    //save status not supported in our map after configuration interface have modified it
    if (param_old_val == CAMERA_PROPERTY_NOT_SUPPORTED)
    {
      it->second = CAMERA_PROPERTY_NOT_SUPPORTED;
    }
  }
  return result;
}


