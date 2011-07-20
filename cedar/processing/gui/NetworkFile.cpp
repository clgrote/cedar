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

    File:        NetworkFile.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 19

    Description:

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "processing/gui/NetworkFile.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::gui::NetworkFile::NetworkFile(cedar::proc::gui::Scene* pScene)
:
mNetwork(new cedar::proc::Network()),
mpScene(pScene)
{
}

cedar::proc::gui::NetworkFile::~NetworkFile()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::NetworkPtr cedar::proc::gui::NetworkFile::network()
{
  return this->mNetwork;
}

void cedar::proc::gui::NetworkFile::save(const std::string& destination)
{
  cedar::aux::ConfigurationNode root;

  this->mNetwork->saveTo(root);
  this->saveScene(root);

  write_json(destination, root);
}

void cedar::proc::gui::NetworkFile::saveScene(cedar::aux::ConfigurationNode /* root */)
{
  // TODO implement me
}
