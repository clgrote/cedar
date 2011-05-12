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

    File:        ControlThread.h

    Maintainer:  Bjoern Weghenkel
    Email:       bjoern.weghenkel@ini.ruhr-uni-bochum.de
    Date:        2011 05 10

    Description: Example of speed control of an single joint.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_CONTROLTHREAD_H
#define CEDAR_CONTROLTHREAD_H


// LOCAL INCLUDES

// PROJECT INCLUDES

#include "auxiliaries/LoopedThread.h"
#include "devices/robot/KinematicChain.h"

// SYSTEM INCLUDES


const unsigned int JOINT = 4;
const double TARGET = 1.5;


/*!@brief
 */
class ControlThread : public cedar::aux::LoopedThread
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------

public:

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------

  ControlThread(const cedar::dev::robot::KinematicChainPtr &kinematicChain, const std::string& configFileName);

  //!@brief Destructor
  //~ControlThread();

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

  void step(double stepSize);

//--------------------------------------------------------------------------------------------------------------------
// members
//--------------------------------------------------------------------------------------------------------------------

public:

  // none yet (hopefully never!)

protected:

  // none yet

private:

  cedar::dev::robot::KinematicChainPtr mpKinematicChain;


//--------------------------------------------------------------------------------------------------------------------
// parameters
//--------------------------------------------------------------------------------------------------------------------

public:

  // none yet (hopefully never!)

protected:

  // none yet

private:

  // none yet

}; // class ForwardInverseWidget


#endif /* CEDAR_CONTROLTHREAD_H */
