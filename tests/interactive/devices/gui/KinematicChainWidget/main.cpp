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

    File:        KinematicChainWidget.cpp

    Maintainer:  Bjoern Weghenkel
    Email:       bjoern.weghenkel@ini.rub.de
    Date:        2011 01 06

    Description: Example for an @em cedar::dev::robot::KinematicChainWidget.

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES

// PROJECT INCLUDES

#include "cedar/devices/robot/gui/KinematicChainWidget.h"
#include "cedar/devices/robot/gui/KinematicChainMonitorWidget.h"
#include "cedar/devices/robot/SimulatedKinematicChain.h"

// SYSTEM INCLUDES

#include <iostream>
#include <QApplication>

//------------------------------------------------------------------------------
// methods
//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  cedar::dev::robot::KinematicChainPtr p_kinematic_chain(new cedar::dev::robot::SimulatedKinematicChain("../../../../tests/interactive/devices/gui/KinematicChainWidget/test_arm.conf"));
  QApplication app(argc, argv);
  cedar::dev::robot::gui::KinematicChainWidget widget(p_kinematic_chain);
  widget.getMonitorWidget()->setDecimals(10);
  widget.getCommandWidget()->setDecimals(10);
  widget.getCommandWidget()->setSingleStep(0.12345);
  widget.show();
  return app.exec();
}
