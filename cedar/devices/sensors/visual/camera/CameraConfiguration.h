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

    File:        CameraConfiguration.h

    Maintainer:  Georg.Hartinger
    Email:       georg.hartinger@ini.rub.de
    Date:        2011 12 01

    Description: Header of the @em cedar::devices::visual::CameraConfiguration class.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_DEV_SENSORS_VISUAL_CAMERA_CONFIGURATION_H
#define CEDAR_DEV_SENSORS_VISUAL_CAMERA_CONFIGURATION_H

// LOCAL INCLUDES
#include "cedar/devices/sensors/visual/CameraGrabber.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES


/*! \class cedar::dev::sensors::visual::CameraConfiguration
 *  \brief This class manage the properties and capabilities of a camera.
 *
 *  \remarks
 *    With the methods of this class, the CameraGraber class can evaluate the available properties
 *    and their possible values.
 *
 *  \par
 *    The main purpose of this class is to manage all the capabilities and properties of one used camera.
 *    There will be two classes created: <br>
 *    1. CameraCapabilities() <br>
 *        This class have a separate configuration file for the capabilities of the used camera. All supported
 *        properties and the range of their values will be managed by this class.<br>
 *    2. CameraState()  <br>
 *        This class main purpose is to encapsulate the ConfigurationInterface() for the actual set properties
 *        of the used camera. The configuration normally is written to the same file that your cameragrabber use.
 *
 */
class cedar::dev::sensors::visual::CameraConfiguration
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //! @brief The standard constructor.
  // \param VideoCapture The cv::VideoCapture object, which this configuration is assigned to
  //        This will be used, to read and set the values
  // \param videoCaptureLock The lock, for the single access to the cv::VideoCapture object
  // \param configurationFileName The filename for the configuration file used to store camera properties in
  //        This could be the same file, which the cameragrabber uses for configuration storage
  // \param capabilitiesFileName The filename of the capabilities. This file have to be adjusted for the used camera
  CameraConfiguration(
                       cv::VideoCapture videoCapture,
                       QReadWriteLockPtr videoCaptureLock,
                       unsigned int channel,
                       const std::string configurationFileName,
                       const std::string capabilitiesFileName
                     );


  //!@brief Destructor
  ~CameraConfiguration();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:

  ///! Set a parameter in the cv::VideoCapture class
  // implements the cv::VideoCapture.set() method with respect to concurrent access
  //bool setProperty(unsigned int prop_id, double value);

  ///! Get a parameter form the cv::VideoCapture
  // implements the cv::VideoCapture.get() method with respect to concurrent access
  //double getProperty(unsigned int prop_id);

  //returns the capabilities of a given property
  /*! \brief Get the minimum possible value that can be set of the given property
   *  \param propId The id of the property
   */
  int getMinValue(CameraProperty::Id propId);

  /*! \brief Get the maximum possible value that can be set of the given property
   *  \param propId The id of the property
   */
  int getMaxValue(CameraProperty::Id propId);

  /*! \brief This method tells you, if the given property is supported by the used camera
   *  \param propId The id of the  property
   */
  bool isSupported(CameraProperty::Id propId);

  /*! \brief This method tells you, if the given property is readable by the used camera
   *  \param propId The id of the  property
   */
  bool isReadable(CameraProperty::Id propId);

  /*! \brief This method tells you, if the given property supports the OnePushAuto mode
   *  \remarks OnePushAuto is a special mode.
   *     It is used as follows: Set a value to a property and then to OnePushAuto mode.
   *     The camera now will try to hold this value automatically.
   *  \param propId The id of the  property
   */
  bool isOnePushCapable(CameraProperty::Id propId);

  /*! \brief This method tells you, if the given property could be turn off and on
   *  \param propId The id of the  property
   */
  bool isOnOffCapable(CameraProperty::Id propId);

  /*! \brief This method tells you, if the given property can be set to auto-mode
   *  \param propId The id of the  property
   */
  bool isAutoCapable(CameraProperty::Id propId);

  /*! \brief This method tells you, if the given property can be set manually
   *  \param propId The id of the  property
   */
  bool isManualCapable(CameraProperty::Id propId);

  /*! \brief This method tells you, if the given property can be set to an absolute value
   *  \param propId The id of the  property
   */
  bool isAbsoluteCapable(CameraProperty::Id propId);
  
  /*! \brief This method is called from the CameraGrabber when the configuration
   *   should be saved
   */
  bool writeConfiguration();

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet (hopefully never!)
protected:

  //bool doInit();



private:

  /// @cond SKIPPED_DOCUMENTATION

  ///! The CameraCapability class manage the capabilities of the camera
  CameraCapabilitiesPtr mpCamCapabilities;

  ///! The CameraState class manage the properties and settings of the camera, i.e. the actual state
  CameraStatePtr mpCamState;

  ///! The filename of the configuration file for camera settings
  std::string mConfigurationFileName;

  ///! The filename of the capabilities file for camera capabilities
  std::string mCapabilitiesFileName;

  ///! The channel number for this configuration. This is only used to display this information on the console
  unsigned int mChannel;

  ///! A short string, where the channel number is stored in.
  // Used as a prefix to store the properties in the configuration file
  std::string mChannelPrefix;


  ///! The already created cv::VideoCapture object from the CameraGrabber class
  cv::VideoCapture mVideoCapture;

  ///! The lock for concurrent access to the VideoCapture
  QReadWriteLockPtr mpVideoCaptureLock;

  /// @endcond

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet (hopefully never!)
protected:
  // none yet
private:

}; // class cedar::dev::sensors::visual::CameraConfiguration

#endif // CEDAR_DEV_SENSORS_VISUAL_CAMERA_CONFIGURATION_H

