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

    File:        DataRole.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 06 24

    Description:

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "processing/DataRole.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES

cedar::aux::EnumBase<cedar::proc::DataRole> cedar::proc::DataRole::mType("cedar::proc::DataRole::");

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::DataRole::construct()
{
  mType.def(cedar::aux::Enum(cedar::proc::DataRole::INPUT, "INPUT", "Input"));
  mType.def(cedar::aux::Enum(cedar::proc::DataRole::OUTPUT, "OUTPUT", "Output"));
  mType.def(cedar::aux::Enum(cedar::proc::DataRole::BUFFER, "BUFFER", "Buffer"));
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

const cedar::aux::EnumBase<cedar::proc::DataRole>& cedar::proc::DataRole::type()
{
  return cedar::proc::DataRole::mType;
}

