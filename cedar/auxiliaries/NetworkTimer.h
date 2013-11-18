/*======================================================================================================================

    Copyright 2011, 2012, 2013 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany

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

    Description: Singleton class for a central time in the network.

    Credits:

======================================================================================================================*/
//CEDAR INCLUDES
#include "cedar/auxiliaries/namespace.h"

//SYSTEM INCLUDES
#include <QTime>

#ifndef CEDAR_AUX_NETWORKTIMER_H_
#define CEDAR_AUX_NETWORKTIMER_H_


//@!brief Can start, stop and reset the network time and should be used as a central time giver in a network.
class cedar::aux::NetworkTimer
{

  //--------------------------------------------------------------------------------------------------------------------
  // friends
  //--------------------------------------------------------------------------------------------------------------------

  // uses singleton template.
  friend class cedar::aux::Singleton<NetworkTimer>;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
private:
  //!@brief The private constructor for singleton usage.
  NetworkTimer();
public:
  //!@brief The destructor.
  ~NetworkTimer();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief Starts the timer
  void start();

  //!@brief Resets the timer
  void reset();

  //!@brief Stops the timer
  void stop();

  //!@brief Returns the elapsed time since timer has started
  unsigned int getTime();

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------

protected:
    // none yet

private:

  //!@brief Flag that is switched when the timer starts/stops
  bool mRunning;

  //!@brief The time when the timer has started
  QTime mStart;

  //!@brief The time when the timer has stoped
  QTime mStop;
};

#include "cedar/auxiliaries/Singleton.h"

namespace cedar
{
  namespace aux
  {
    CEDAR_INSTANTIATE_AUX_TEMPLATE(cedar::aux::Singleton<cedar::aux::NetworkTimer>);
    typedef cedar::aux::Singleton<cedar::aux::NetworkTimer> NetworkTimerSingleton;
  }
}

#endif /* CEDAR_AUX_NETWORKTIMER_H_ */
