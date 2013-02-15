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

    File:        namespace.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 06 03

    Description: Namespace file for cedar::units.

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/units/namespace.h"

// SYSTEM INCLUDES
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/systems/si/velocity.hpp>
#include <boost/units/systems/si/acceleration.hpp>
#include <boost/units/systems/si/plane_angle.hpp>

using namespace boost::units;
using namespace boost::units::si;

const char cedar::unit::prefix_us[] = "µs";
const char cedar::unit::prefix_ms[] = "ms";
const char cedar::unit::prefix_s[] = "s";

const quantity<length> cedar::unit::DEFAULT_LENGTH_UNIT = 1 * meter;
const quantity<boost::units::si::time> cedar::unit::DEFAULT_TIME_UNIT = 1 * second;
const quantity<velocity> cedar::unit::DEFAULT_VELOCITY_UNIT = 1 * meter_per_second;
const quantity<acceleration> cedar::unit::DEFAULT_ACCELERATION_UNIT = 1 * meter_per_second_squared;
const quantity<plane_angle> cedar::unit::DEFAULT_PLANE_ANGLE_UNIT = 1 * radian;
