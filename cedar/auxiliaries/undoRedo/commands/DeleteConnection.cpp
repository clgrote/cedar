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

    File:        DeleteConnection.cpp

    Maintainer:  Yogeshwar Agnihotri
    Email:       yogeshwar.agnihotri@ini.ruhr-uni-bochum.de
    Date:        2020 07 04

    Description: Source file for the class cedar::aux::undoRedo::commands::DeleteConnection.

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/undoRedo/commands/DeleteConnection.h"
#include "cedar/processing/gui/GraphicsBase.h"
#include "cedar/processing/gui/Connection.h"
#include "cedar/processing/gui/Group.h"
#include "cedar/processing/gui/StepItem.h"
#include "cedar/processing/gui/Scene.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::undoRedo::commands::DeleteConnection::DeleteConnection(cedar::proc::gui::Connection* connection,
        cedar::aux::undoRedo::commands::DeleteConnection::Action action,
        bool createConnectorGroup)
:
mpSource(connection->getSource()),
mpTarget(connection->getTarget()),
mAction(action),
mCreateConnectorGroup(createConnectorGroup)
{
  this->mpScene = dynamic_cast<cedar::proc::gui::Scene *>(this->mpTarget->scene());
  CEDAR_DEBUG_ASSERT(this->mpScene)
}

cedar::aux::undoRedo::commands::DeleteConnection::DeleteConnection(cedar::proc::gui::GraphicsBase* source,
        cedar::proc::gui::GraphicsBase* target,
        cedar::aux::undoRedo::commands::DeleteConnection::Action action,
        bool createConnectorGroup)
:
mpSource(source),
mpTarget(target),
mAction(action),
mCreateConnectorGroup(createConnectorGroup)
{
  this->mpScene = dynamic_cast<cedar::proc::gui::Scene *>(this->mpTarget->scene());
  CEDAR_DEBUG_ASSERT(this->mpScene)
}

cedar::aux::undoRedo::commands::DeleteConnection::~DeleteConnection()
{
}

void cedar::aux::undoRedo::commands::DeleteConnection::undo()
{
  switch(mAction)
  {
    case Action::ADDED:
      removeConnection();
      break;
    case Action::REMOVED:
      createConnection();
      break;
  }
}

void cedar::aux::undoRedo::commands::DeleteConnection::redo()
{
  switch(mAction)
  {
    case Action::ADDED:
      createConnection();
      break;
    case Action::REMOVED:
      removeConnection();
      break;
  }
}

void cedar::aux::undoRedo::commands::DeleteConnection::removeConnection()
{
  std::vector<cedar::proc::gui::Connection*> connectionsFromSource = this->mpSource->getConnections();
  for(cedar::proc::gui::Connection* con : connectionsFromSource)
  {
    if(con->getTarget() == this->mpTarget)
    {
      con->disconnectUnderlying();
      break;
    }
  }
}

void cedar::aux::undoRedo::commands::DeleteConnection::createConnection()
{
  if(this->mpSource != nullptr && this->mpTarget != nullptr && this->mpScene != nullptr)
  {
    this->mpScene->createConnection(this->mpSource, this->mpTarget, mCreateConnectorGroup);
  }
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------
