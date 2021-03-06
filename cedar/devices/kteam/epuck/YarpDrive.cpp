/*======================================================================================================================

    Copyright 2011, 2012, 2013, 2014, 2015, 2016, 2017 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        YarpDrive.cpp

    Maintainer:  Stephan Zibner
    Email:       stephan.zibner@ini.rub.de
    Date:        2013 03 06

    Description: Yarp implementation of an epuck drive.

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

#ifdef CEDAR_USE_YARP

// CEDAR INCLUDES
#include "cedar/auxiliaries/casts.h"
#include "cedar/auxiliaries/math/constants.h"
#include "cedar/auxiliaries/math/tools.h"
#include "cedar/devices/kteam/epuck/YarpDrive.h"
#include "cedar/devices/YarpChannel.h"
#include "cedar/devices/kteam/serialChannelHelperFunctions.h"
#include "cedar/devices/ComponentSlot.h"
#include "cedar/devices/Robot.h"
#include "cedar/devices/RobotManager.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// type registration
//----------------------------------------------------------------------------------------------------------------------

namespace
{
  bool registered()
  {
    cedar::dev::ComponentManagerSingleton::getInstance()->
        registerType<cedar::dev::kteam::epuck::YarpDrivePtr>();
    return true;
  }

  bool registerFnCall = registered();
}

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::dev::kteam::epuck::YarpDrive::YarpDrive()
:
_mMotorCommandsPort(new cedar::aux::StringParameter(this, "motor commands port", "motorCommands")),
_mEncoderValuesPort(new cedar::aux::StringParameter(this, "encoder values port", "encoderValues"))
{
  this->registerCommandHook(cedar::dev::DifferentialDrive::WHEEL_SPEED, boost::bind(&cedar::dev::kteam::epuck::YarpDrive::sendMovementCommand, this));
}

cedar::dev::kteam::epuck::YarpDrive::YarpDrive
(
    YarpMatChannelPtr channel
)
:
cedar::dev::kteam::Drive(channel),
mYarpChannel(channel),
_mMotorCommandsPort(new cedar::aux::StringParameter(this, "motor commands port", "motorCommands")),
_mEncoderValuesPort(new cedar::aux::StringParameter(this, "encoder values port", "encoderValues"))
{
  this->registerCommandHook(cedar::dev::DifferentialDrive::WHEEL_SPEED, boost::bind(&cedar::dev::kteam::epuck::YarpDrive::sendMovementCommand, this));
}

cedar::dev::kteam::epuck::YarpDrive::~YarpDrive()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::dev::kteam::epuck::YarpDrive::registerChannels()
{
  CEDAR_ASSERT(this->getChannel());
  mYarpChannel = boost::dynamic_pointer_cast<YarpMatChannel>(this->getChannel());
  std::string robot_name = cedar::dev::RobotManagerSingleton::getInstance()->getRobotName(this->getSlot().lock()->getRobot()) + "/";
  boost::replace_all(robot_name, " ", "_");
  mMotorCommandPortWithPrefix = robot_name + _mMotorCommandsPort->getValue();
  mEncoderValuesPortWithPrefix = robot_name + _mEncoderValuesPort->getValue();
  mYarpChannel->addWriterPort(mMotorCommandPortWithPrefix);
  mYarpChannel->addReaderPort(mEncoderValuesPortWithPrefix);
}

std::vector<int> cedar::dev::kteam::epuck::YarpDrive::getEncoders() const
{
  cv::Mat encoder_matrix;
  std::vector<int> encoders;
  try
  {
    encoder_matrix = mYarpChannel->read(mEncoderValuesPortWithPrefix);
    if (!encoder_matrix.empty())
    {
      encoders.push_back(encoder_matrix.at<float>(0,0));
      encoders.push_back(encoder_matrix.at<float>(1,0));
    }
  }
  catch (cedar::aux::net::NetWaitingForWriterException& exc)
  {
    // this can happen, just ignore it
  }
  return encoders;
}

void cedar::dev::kteam::epuck::YarpDrive::setEncoders(const std::vector<int>& /*encoders*/)
{

}

void cedar::dev::kteam::epuck::YarpDrive::sendMovementCommand()
{
  //retrieve matrix
  auto mat_data = cedar::aux::asserted_pointer_cast<cedar::aux::ConstMatData>(getUserSideCommandData(cedar::dev::DifferentialDrive::WHEEL_SPEED));
  QReadLocker data_locker(&(mat_data->getLock()));
  cv::Mat mat = cedar::aux::asserted_pointer_cast<cedar::aux::ConstMatData>(getUserSideCommandData(cedar::dev::DifferentialDrive::WHEEL_SPEED))->getData();

  CEDAR_DEBUG_ASSERT(mat.type() == CV_32F);
  CEDAR_DEBUG_ASSERT(mat.rows == 2);
  CEDAR_DEBUG_ASSERT(mat.cols == 1);

  cv::Mat velocities(2, 1, CV_32F);

  velocities.at<float>(0, 0) = (mat.at<float>(0, 0) / getNumberOfPulsesPerRevolution()) * 2.0f * cedar::aux::math::pi;
  velocities.at<float>(1, 0) = (mat.at<float>(1, 0) / getNumberOfPulsesPerRevolution()) * 2.0f * cedar::aux::math::pi;

  mYarpChannel->write(velocities, mMotorCommandPortWithPrefix);
}

void cedar::dev::kteam::epuck::YarpDrive::readConfiguration(const cedar::aux::ConfigurationNode& node)
{
  cedar::dev::kteam::Drive::readConfiguration(node);
  this->registerChannels();
}
#endif
