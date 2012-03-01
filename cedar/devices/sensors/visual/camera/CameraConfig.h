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

    Description: Header of the @em cedar::devices::visual::CameraConfig class.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_DEV_SENSORS_VISUAL_CAMERA_STATE_H
#define CEDAR_DEV_SENSORS_VISUAL_CAMERA_STATE_H

// LOCAL INCLUDES
#include "cedar/devices/sensors/visual/CameraGrabber.h"
#include "cedar/devices/sensors/visual/camera/defines.h"
#include "cedar/auxiliaries/ConfigurationInterface.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <set>


/*! \class cedar::dev::sensors::visual::CameraConfig
 *  \brief Stores the properties and the settings of one camera in a configuration file
 *  \remarks This class is used by the CameraStateAndConfig class to store the actual settings of
 *          this camera
 */

class cedar::dev::sensors::visual::CameraConfig
:
public cedar::aux::ConfigurationInterface
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*!@brief Constructor
   * \param videoCapture The already created cv::VideoCapture object from the CameraGrabber class
   * \param channelPrefix The channel number as a prefix for the properties stored in the configuration file
   * \param supportedProperties A set which contains all supported properties
   * \param videoCaptureLock A boost::shared_pointer to an Object of QReadWriteLock
   *                         (also created by the CameraGrabber class)
   * \param configFileName The filename to store the actual values of the properties and features. You can use the
   *                        configuration file of the CameraGrabber class
   */
  CameraConfig(
                           //cv::VideoCapture videoCapture,
                           //QReadWriteLockPtr videoCaptureLock,
                           const std::string& configFileName,
                           unsigned int channel,
                           //SupportedPropertiesSet supportedProperties,
                           CameraSettings &camSettings,
                           CameraPropertyValues &camPropertyValues
                         );
  //!@brief Destructor
  ~CameraConfig();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  ///! Save the configuration to the configuration file specified in the constructor
   bool saveConfiguration();



  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:

   ///! Set a parameter in the cv::VideoCapture class
   // implements the cv::VideoCapture.set() method with respect to concurrent access
  // bool setProperty(unsigned int propId, double value);

   ///! Get a parameter form the cv::VideoCapture
   // implements the cv::VideoCapture.get() method with respect to concurrent access
   //double getProperty(unsigned int propId);

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
   /*! \brief This Method reads all parameters and settings from the camera and stores them inside the buffers
    *  \remarks
   *    This is needed for the configurationInterface
   *    The parameters and settings will be stored within the mCamProperties and mCamSettings vectors
   */
  //void getAllParametersFromCam();

  /*! \brief This Method sets all parameters and settings read from the configfile to the camera
   *  \remarks This is needed for the configurationInterface
   *  The parameters and settings will be synchronized from the mCamProperties and mCamSettings vector
   */
  //bool setAllParametersToCam();

  /*! \brief Create local buffer for camera properties and settings
   *  \remarks needed for the ConfigurationInterface class
   */
  //void createParameterStorage();

  ///! Interface to ConfigurationInterface class;
  bool declareParameter();

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet (hopefully never!)
protected:
  // none yet
private:

  /*! \brief This is the struct, for the values of the camera settings
   *  \remarks This is the local storage for the camera settings
   *    and is only needed for ConfigurationInterface class
   */
  CameraSettings &mCamSettings;

  /*! \brief This is the map, where all supported properties and their actual values stored in
   *  \remarks This is the local storage for the supported camera properties
   *    and is only needed for ConfigurationInterface class
   */
  CameraPropertyValues &mCamPropertyValues;

  ///! The channel number for this configuration. This is only used to display this information on the console
  unsigned int mChannel;

  /*! \brief The channel prefix used in the configuration file.
   *  \remarks This is essential if the configuration of more than one camera should be
   *    saved in the configuration file
   */
  std::string mChannelPrefix;


  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet (hopefully never!)
protected:
  // none yet

private:


}; // cedar::dev::sensors::visual::CameraConfig

#endif // CEDAR_DEV_SENSORS_VISUAL_CAMERA_STATE_H

