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

    File:        NetworkTimer.h

    Maintainer:  Christian Bodenstein
    Email:       christian.bodenstein@ini.ruhr-uni-bochum.de
    Date:        2013 11 7

    Description: Singleton class for a central time in the network network.

    Credits:

======================================================================================================================*/

//CEDAR INCLUDES
#include "cedar/auxiliaries/GlobalClock.h"
#include "cedar/units/Time.h"
#include "cedar/units/prefixes.h"

cedar::aux::GlobalClock::GlobalClock()
:
mRunning(false)
{
  this->reset();
}

cedar::aux::GlobalClock::~GlobalClock()
{
}

bool cedar::aux::GlobalClock::isRunning() const
{
  return this->mRunning;
}

void cedar::aux::GlobalClock::start()
{
  if (!this->mRunning)
  {
    this->mTimer.start();
    this->mRunning = true;
  }
}

void cedar::aux::GlobalClock::reset()
{
  this->mAdditionalElapsedTime = 0;
  this->mTimer.restart();
}


void cedar::aux::GlobalClock::stop()
{
  if (this->mRunning)
  {
    this->mAdditionalElapsedTime += this->mTimer.elapsed();
    this->mTimer.restart();
    this->mRunning = false;
  }
}

cedar::unit::Time cedar::aux::GlobalClock::getTime()
{
  int time_msecs = this->mAdditionalElapsedTime;

  if (this->mRunning)
  {
    time_msecs += this->mTimer.elapsed();
  }

  return cedar::unit::Time(static_cast<double>(time_msecs) * cedar::unit::milli * cedar::unit::seconds);
}



