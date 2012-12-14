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

    File:        ComponentSlot.cpp

    Maintainer:  Mathis Richter
    Email:       mathis.richter@ini.rub.de
    Date:        2012 11 23

    Description: Slot for components of a robot (e.g., a kinematic chain).

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/ObjectMapParameterTemplate.h"
#include "cedar/auxiliaries/StringParameter.h"
#include "cedar/devices/ComponentSlot.h"
#include "cedar/devices/Component.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::dev::ComponentSlot::ComponentSlot(cedar::dev::RobotPtr robot)
:
mRobot(robot)
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

std::ostream& cedar::dev::operator<<(std::ostream& stream, const cedar::dev::ComponentSlot& slot)
{
  stream << "slot";
  if (slot.mComponent)
  {
    stream << " with component instance of type " << cedar::aux::objectTypeToString(slot.mComponent);
  }
  else
  {
    stream << " currently holds no component instance";
  }
  stream << std::endl;

  stream << "available components:" << std::endl;
  for (auto iter = slot._mComponentConfigurations.begin(); iter != slot._mComponentConfigurations.end(); ++iter)
  {
    const std::string& type_name = iter->first;
    stream << "  " << type_name;
    stream << std::endl;
  }

  return stream;
}

std::ostream& cedar::dev::operator<<(std::ostream& stream, cedar::dev::ConstComponentSlotPtr slot)
{
  stream << "Component slot @ " << slot.get() << std::endl;
  stream << *slot;
  return stream;
}

std::ostream& cedar::dev::operator<<(std::ostream& stream, cedar::dev::ComponentSlotPtr slot)
{
  stream << cedar::dev::ConstComponentSlotPtr(slot);
  return stream;
}

std::vector<std::string> cedar::dev::ComponentSlot::listChannels() const
{
  std::vector<std::string> list;
  for (auto iter = this->mComponentTypeIds.begin(); iter != this->mComponentTypeIds.end(); ++iter)
  {
    list.push_back(iter->first);
  }
  return list;
}

bool cedar::dev::ComponentSlot::hasChannel(const std::string& name) const
{
  return this->mComponentTypeIds.find(name) != this->mComponentTypeIds.end();
}

void cedar::dev::ComponentSlot::setChannel(const std::string& channel)
{
  //TODO who is responsible for calling this?
  auto iter = mComponentTypeIds.find(channel);

  //TODO proper exception
  CEDAR_ASSERT(iter != mComponentTypeIds.end());

  const std::string& type_id = iter->second;
  this->mComponent = cedar::dev::ComponentManagerSingleton::getInstance()->allocate(type_id);

  cedar::aux::ConfigurationNode configuration;
  configuration.insert(configuration.end(), this->mCommonParameters.begin(), this->mCommonParameters.end());

  auto conf_iter = this->_mComponentConfigurations.find(type_id);

  if (conf_iter != this->_mComponentConfigurations.end())
  {
    const cedar::aux::ConfigurationNode& tree = conf_iter->second;
    configuration.insert(configuration.end(), tree.begin(), tree.end());
  }

  this->mComponent->readConfiguration(configuration);
}


void cedar::dev::ComponentSlot::readConfiguration(const cedar::aux::ConfigurationNode& node)
{
  cedar::aux::ConfigurationNode::const_assoc_iterator common = node.find("common component parameters");

  mCommonParameters = cedar::aux::ConfigurationNode();

  if (common != node.not_found())
  {
    mCommonParameters = common->second;
  }

  mComponentTypeIds.clear();

  cedar::aux::ConfigurationNode::const_assoc_iterator available_slots = node.find("available components");
  if (available_slots != node.not_found())
  {
    for (auto slot_iter = available_slots->second.begin(); slot_iter != available_slots->second.end(); ++slot_iter)
    {
      const std::string& channel_name = slot_iter->first;
      const cedar::aux::ConfigurationNode& channel_node = slot_iter->second;

      //TODO proper exception
      CEDAR_ASSERT(channel_node.size() == 1);

      cedar::aux::ConfigurationNode::const_iterator type_node = channel_node.begin();
      const std::string& type_str = type_node->first;

      const cedar::aux::ConfigurationNode& conf = type_node->second;

      //!@todo Actual exception
      CEDAR_ASSERT(this->_mComponentConfigurations.find(type_str) == this->_mComponentConfigurations.end());
      CEDAR_ASSERT(this->mComponentTypeIds.find(channel_name) == this->mComponentTypeIds.end());

      this->_mComponentConfigurations[type_str] = conf;
      this->mComponentTypeIds[channel_name] = type_str;
    }
  }

  this->cedar::aux::Configurable::readConfiguration(node);
}

cedar::dev::ComponentPtr cedar::dev::ComponentSlot::getComponent()
{
  if (!mComponent)
  {
    std::string channel_type = _mChannelType->getValue();
    std::string component_type_id = mComponentTypeIds[channel_type];
    mComponent = FactorySingleton::getInstance()->allocate(component_type_id);
  }

  return mComponent;
}
