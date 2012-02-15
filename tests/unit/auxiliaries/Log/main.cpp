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

    File:        main.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2012 02 14

    Description: Implements all unit tests for the @em cedar::aux::LogFile class.

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "cedar/defines.h"
#include "cedar/auxiliaries/Log.h"
#include "cedar/auxiliaries/logFilter/Type.h"
#include "cedar/auxiliaries/LogInterface.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <boost/shared_ptr.hpp>
#include <string>

class CustomLogger : public cedar::aux::LogInterface
{
  public:
    void message
    (
      cedar::aux::LOG_LEVEL /* level */,
      const std::string& message,
      const std::string& /* title */
    )
    {
      mMessages.push_back(message);
      std::cout << "Intercepted message: " << message << std::endl;
    }
    
    std::vector<std::string> mMessages;
};

CEDAR_GENERATE_POINTER_TYPES(CustomLogger);

int main()
{
  int errors = 0;
  
  // test general logging capabilities
  cedar::aux::LogSingleton::getInstance()->message("This is a test message.", "SystemTest::main", "Title");
  cedar::aux::LogSingleton::getInstance()->warning("This is a test warning.", "SystemTest::main", "Title 2");
  cedar::aux::LogSingleton::getInstance()->error("This is a test error.", "SystemTest::main", "Title 42");
  cedar::aux::LogSingleton::getInstance()->systemInfo("This is a test system info.", "SystemTest::main", "Route 66");
  cedar::aux::LogSingleton::getInstance()->message("This is a test message without title.", "SystemTest::main");
  cedar::aux::LogSingleton::getInstance()->debug("This is a test debug message.", "SystemTest::main", "debug test");
  
  // test filtering
  CustomLoggerPtr debug_logger (new CustomLogger());
  cedar::aux::LogFilterPtr filter (new cedar::aux::logFilter::Type(cedar::aux::LOG_LEVEL_DEBUG));
  cedar::aux::LogSingleton::getInstance()->addLogger(debug_logger, filter);
  
  CustomLoggerPtr all_logger (new CustomLogger());
  cedar::aux::LogSingleton::getInstance()->addLogger(all_logger);

  cedar::aux::LogSingleton::getInstance()->message("message", "SystemTest::main");
  cedar::aux::LogSingleton::getInstance()->debug("debug", "SystemTest::main");
  cedar::aux::LogSingleton::getInstance()->message("message", "SystemTest::main");
  cedar::aux::LogSingleton::getInstance()->debug("debug", "SystemTest::main");
  
  if (debug_logger->mMessages.size() != 2)
  {
    std::cout << "The wrong number of messages was received by debug_logger." << std::endl;
    ++errors;
  }
  
  if (all_logger->mMessages.size() != 2)
  {
    std::cout << "The wrong number of messages was received by all_logger." << std::endl;
    ++errors;
  }
  
  return errors;
}
