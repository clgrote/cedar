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

    File:        main.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 06 03

    Description:

    Credits:

======================================================================================================================*/


// LOCAL INCLUDES

// PROJECT INCLUDES
#include "auxiliaries/LogFile.h"
#include "auxiliaries/computation/Arguments.h"
#include "Neuron.h"

// SYSTEM INCLUDES
#include <iostream>

using namespace cedar::aux;

int main(int argc, char** argv)
{
  unsigned int errors = 0;

  LogFile log_file("Time.log");
  log_file.addTimeStamp();
  log_file << std::endl;

  cedar::Neuron neuron;
  for (unsigned int i = 0; i < 1000; i++)
  {
    neuron.compute(cedar::aux::comp::Arguments());
    if (i % 100 == 0)
    {
      log_file << neuron.getActivity() << std::endl;
    }
  }


  log_file << "Done. There were " << errors << " errors." << std::endl;
  return errors;
}
