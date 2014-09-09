/*======================================================================================================================

    Copyright 2011, 2012, 2013, 2014 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        utilities.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2014 01 16

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/stringFunctions.h"
#include "cedar/version.h"

// SYSTEM INCLUDES
#include <boost/optional.hpp>
#include <iomanip>
#include <sstream>

std::string cedar::aux::versionNumberToString(unsigned int version)
{
  std::string res = cedar::aux::toString(CEDAR_GET_VERSION_MAJOR(version));
  res += "." + cedar::aux::toString(CEDAR_GET_VERSION_MINOR(version));
  res += "." + cedar::aux::toString(CEDAR_GET_VERSION_BUGFIX(version));
  return res;
}

std::string cedar::aux::removeWhiteSpaces(const std::string& stringFromWhichToRemoveWhiteSpaces)
{
  std::string result = cedar::aux::erase(stringFromWhichToRemoveWhiteSpaces, " ");
  result = cedar::aux::erase(result, "\n");
  result = cedar::aux::erase(result, "\r");
  return result;
}

std::string cedar::aux::formatDuration(const cedar::unit::Time& time)
{
  struct TimeSubdiv
  {
    TimeSubdiv(double max, const std::string& shortHandle, unsigned int precision)
    :
    max(max),
    precision(precision),
    short_handle(shortHandle)
    {
    }

    TimeSubdiv(double max, const std::string& shortHandle)
    :
    max(max),
    short_handle(shortHandle)
    {
    }

    TimeSubdiv(const std::string& shortHandle)
    :
    short_handle(shortHandle)
    {
    }

    boost::optional<double> max;
    boost::optional<unsigned int> precision;
    std::string short_handle;
  };

  double time_in_seconds = time / (1.0 * cedar::unit::seconds);

  static std::vector<TimeSubdiv> time_subdivs;
  if (time_subdivs.empty())
  {
    time_subdivs.push_back(TimeSubdiv(60.0, "s", 2));
    time_subdivs.push_back(TimeSubdiv(60.0, "m", 2));
    time_subdivs.push_back(TimeSubdiv(24.0, "h", 2));
    time_subdivs.push_back(TimeSubdiv(365, "d"));
    time_subdivs.push_back(TimeSubdiv("y"));
  }

  std::string time_str;

  double current_max = 1.0;
  double time_remaining = time_in_seconds;
  for (const auto& subdiv : time_subdivs)
  {
    double subtime;
    double old_max = current_max;
    if (subdiv.max)
    {
      current_max *= subdiv.max.get();
      subtime = std::fmod(time_remaining, current_max);
      time_remaining -= subtime;
    }
    else
    {
      subtime = time_remaining;
      time_remaining = 0.0;
    }
    std::stringstream stream;
    if (time_remaining > 0.0 && subdiv.precision)
    {
      // make the conversion zero-padded
      stream << std::setw(subdiv.precision.get()) << std::setfill('0');
    }

    stream << (subtime / old_max);
    stream << subdiv.short_handle;
    std::string subdiv_str = stream.str();
    if (!time_str.empty())
    {
      subdiv_str += " ";
    }
    time_str = subdiv_str + time_str;

    if (time_remaining <= 0.0)
    {
      break;
    }
  }

  return time_str;
}
