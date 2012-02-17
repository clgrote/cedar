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

    File:        Component.h

    Maintainer:  Mathis Richter
    Email:       mathis.richter@ini.rub.de
    Date:        2010 08 30

    Description: Abstract component of a robot (e.g., a kinematic chain).

    Credits:

======================================================================================================================*/


#ifndef CEDAR_DEV_ROBOT_COMPONENT_H
#define CEDAR_DEV_ROBOT_COMPONENT_H

// CEDAR INCLUDES
#include "cedar/devices/robot/namespace.h"
#include "cedar/auxiliaries/Base.h"
#include "cedar/devices/communication/Communication.h"

// SYSTEM INCLUDES
#include <vector>
#include <string>
#include <set>

/*!@brief Base class for components of robots.
 *
 * @todo More detailed description of the class.
 */
class cedar::dev::robot::Component : public virtual cedar::aux::Base
{
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:

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
protected:
  //! name of the category the component is in (in the configuration file)
  std::string mCategoryName;
  //! pointer to the robot the component belongs to
  RobotPtr mpRobot;
  //! pointer to the communication device
  cedar::dev::com::Communication *mpeCommunication;

private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //! name of the parent component (if it exists)
  std::string _mParentName;
private:
  // none yet
}; // class cedar::dev::robot::Component
#endif // CEDAR_DEV_ROBOT_COMPONENT_H
