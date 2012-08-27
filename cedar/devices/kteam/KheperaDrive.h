/*======================================================================================================================

    Copyright 2011, 2012 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany

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

    File:        KheperaDrive.h

    Maintainer:  Mathis Richter
    Email:       mathis.richter@ini.rub.de
    Date:        2012 04 17

    Description: The drive component of the mobile Khepera robot.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_DEV_KTEAM_KHEPERA_DRIVE_H
#define CEDAR_DEV_KTEAM_KHEPERA_DRIVE_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/math/namespace.h"
#include "cedar/devices/kteam/Drive.h"
#include "cedar/devices/communication/namespace.h"

// SYSTEM INCLUDES

/*!@brief The drive component of the mobile Khepera robot.
 *
 * The constructor expects an initialized serial communication object. After creation of the KheperaDrive
 * object, all its parameters (e.g., wheel radius, hardware speed limits) are read from a configuration file.
 */
class cedar::dev::kteam::KheperaDrive : public cedar::dev::kteam::Drive
{
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief Constructor
  KheperaDrive(cedar::dev::com::SerialCommunicationPtr communication);

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  // documented in base class
  void readConfiguration(const cedar::aux::ConfigurationNode& node);

  void openGripper();
  void closeGripper();
  void setArmPosition(unsigned int position);
  unsigned int getArmPosition();
  unsigned int getGripperPosition();
  unsigned int getGripperOpticalSensor();
  unsigned int getGripperResistivity();
  void setLEDState(unsigned int ledId, bool state);
  std::vector<unsigned int> getProximitySensors();
  std::vector<unsigned int> getAmbientSensors();

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  virtual char getCommandCharacterSetGripperPosition() const;
  virtual std::string getCommandCharacterGetGripperPosition() const;
  virtual char getCommandCharacterSetArmPosition() const;
  virtual std::string getCommandCharacterGetArmPosition() const;
  virtual char getCommandCharacterGetGripperOpticalSensor() const;
  virtual char getCommandCharacterGetGripperResistivity() const;
  virtual char getCommandCharacterSetLEDState() const;
  virtual char getCommandCharacterGetProximitySensors() const;
  virtual char getCommandCharacterGetAmbientSensors() const;

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  void setGripperPosition(bool open);

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

private:
  //! limits for the arm position
  cedar::aux::math::UIntLimitsParameterPtr _mArmPositionLimits;
}; // class cedar::dev::kteam::KheperaDrive

#endif // CEDAR_DEV_KTEAM_KHEPERA_DRIVE_H
