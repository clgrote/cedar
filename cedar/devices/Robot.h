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

    File:        Robot.h

    Maintainer:  Mathis Richter
    Email:       mathis.richter@ini.rub.de
    Date:        2012 11 23

    Description: Manages all components of a robot.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_DEV_ROBOT_H
#define CEDAR_DEV_ROBOT_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/NamedConfigurable.h"
#include "cedar/auxiliaries/FileParameter.h"
#include "cedar/auxiliaries/namespace.h"
#include "cedar/devices/namespace.h"

// SYSTEM INCLUDES
#include <boost/enable_shared_from_this.hpp>
#include <vector>
#include <string>
#include <map>

/*!@brief Base class for robots.
 *
 * @todo More detailed description of the class.
 */
class cedar::dev::Robot : public cedar::aux::NamedConfigurable,
                          public boost::enable_shared_from_this<cedar::dev::Robot>
{
  //--------------------------------------------------------------------------------------------------------------------
  // friends
  //--------------------------------------------------------------------------------------------------------------------
  friend std::ostream& operator<<(std::ostream& stream, const cedar::dev::Robot& robot);

  friend std::ostream& operator<<(std::ostream& stream, cedar::dev::ConstRobotPtr robot);

  friend std::ostream& operator<<(std::ostream& stream, cedar::dev::RobotPtr robot);

  //--------------------------------------------------------------------------------------------------------------------
  // types
  //--------------------------------------------------------------------------------------------------------------------
private:
  typedef std::map<std::string, cedar::dev::ComponentSlotPtr> ComponentSlotParameter;

  typedef cedar::aux::ObjectMapParameterTemplate<cedar::dev::Channel, cedar::aux::allocationPolicies::OnDemand<cedar::dev::Channel> > ChannelParameter;
  CEDAR_GENERATE_POINTER_TYPES_INTRUSIVE(ChannelParameter);

  typedef cedar::aux::NamedConfigurable Super;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief constructor
  Robot();

  //!@brief destructor
  virtual ~Robot();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*! @brief Returns a pointer to the component docked to the component slot with the name @em componentSlotName.
   *
   * @return Pointer to the requested component.
   * @param[in] componentSlotName name of the component slot, whose component is to be returned.
   */
  ComponentPtr getComponent(const std::string& componentSlotName) const;

  /*! @brief Returns a vector containing all the available slot names of the robot.
   *
   * @return vector of available slot names
   */
  std::vector<std::string> getComponentSlotNames() const;

  /*! @brief Returns the component slot with the name @em componentSlotName.
   *
   * @return component slot
   * @param[in] componentSlotName name of the component slot that is to be returned
   */
  ComponentSlotPtr getComponentSlot(const std::string& componentSlotName) const;

  //!@brief Read a configuration for all registered parameters from a cedar::aux::ConfigurationNode.
  virtual void readConfiguration(const cedar::aux::ConfigurationNode& node);

  //!@returns A list of all components of this robot.
  std::vector<std::string> listComponentSlots() const;

  //!@returns A list of all channels of this robot.
  std::vector<std::string> listChannels() const;

  //!@brief Sets the channel for all components in the robot.
  void setChannel(const std::string& channel);

  //!@brief Sets the channel for all components in the robot.
  cedar::dev::ConstChannelPtr getChannel(const std::string& channel) const;

  //!@brief Sets the channel for all components in the robot.
  cedar::dev::ChannelPtr getChannel(const std::string& channel);

  //!@brief Tests whether an instance has been created for the given channel.
  bool hasChannelInstance(const std::string& channel) const;

  template <typename T>
  inline boost::shared_ptr<T> getComponent(const std::string& slotName) const
  {
    cedar::dev::ComponentPtr component = this->getComponent(slotName);
    return boost::dynamic_pointer_cast<T>(component);
  }

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  //!@brief Read a configuration for all registered parameters from a cedar::aux::ConfigurationNode.
  virtual void readDescription(const cedar::aux::ConfigurationNode& node);

  //!@brief Checks the robot description for consistency. Issues are written to the log as warnings and errors.
  void performConsistencyCheck() const;

  //! Reads and instantiates the slots given in the configuration.
  void readComponentSlotInstantiations(const cedar::aux::ConfigurationNode& node);

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
  //! Configurable object used for storing robot setup in a separate file.
  cedar::aux::ConfigurablePtr _mRobotDescription;

  //! mapping of all slot names to their component slots
  ComponentSlotParameter _mComponentSlots;

  //! mapping of all channel names to the channel
  ChannelParameterPtr _mChannels;

}; // class cedar::dev::Robot
#endif // CEDAR_DEV_ROBOT_H
