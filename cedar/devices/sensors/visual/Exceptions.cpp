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

    File:        Exceptions.cpp

    Maintainer:  Georg Hartinger
    Email:       georg.hartinger@ini.rub.de
    Date:        2011 08 01

    Description: This is the main file for all new exceptions introduced in
                 the @em cedar::dev::sensors::visual namespace.

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "Exceptions.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES




//----------------------------------------------------------------------------------------------------------------------
cedar::aux::exc::GrabberSnapshotException::GrabberSnapshotException()
{
  // Set the type name.
  this->mType = "GrabberSnapshotException";
}


//----------------------------------------------------------------------------------------------------------------------
cedar::aux::exc::GrabberRecordingException::GrabberRecordingException()
{
  // Set the type name.
  this->mType = "GrabberRecordingException";
}

//----------------------------------------------------------------------------------------------------------------------
cedar::aux::exc::GrabberGrabException::GrabberGrabException()
{
  // Set the type name.
  this->mType = "GrabberGrabException";
}

//----------------------------------------------------------------------------------------------------------------------

