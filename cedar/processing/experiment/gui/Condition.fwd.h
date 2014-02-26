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

    File:        Condition.fwd.h

    Maintainer:  Christian Bodenstein
    Email:       christian.bodenstein@ini.rub.de
    Date:        2014 02 06

    Description: Forward declaration file for the class cedar::proc::experiment::gui::Condition.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_PROC_EXPERIMENT_GUI_CONDITION_FWD_H
#define CEDAR_PROC_EXPERIMENT_GUI_CONDITION_FWD_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/processing/lib.h"

// FORWARD DECLARATIONS
#include "cedar/processing/experiment/Condition.fwd.h"
#include "cedar/auxiliaries/TypeBasedFactory.fwd.h"
#include "cedar/auxiliaries/Singleton.fwd.h"

// SYSTEM INCLUDES
#ifndef Q_MOC_RUN
  #include <boost/smart_ptr.hpp>
#endif // Q_MOC_RUN

//!@cond SKIPPED_DOCUMENTATION
namespace cedar
{
  namespace proc
  {
    namespace experiment
    {
      namespace gui
      {
        CEDAR_DECLARE_PROC_CLASS(Condition);
        typedef cedar::aux::TypeBasedFactory<cedar::proc::experiment::ConditionPtr,
            cedar::proc::experiment::gui::ConditionPtr> ConditionFactory;
        typedef cedar::aux::Singleton<ConditionFactory> ConditionFactorySingleton;
      }
    }
  }
}

//!@endcond

#endif // CEDAR_PROC_EXPERIMENT_GUI_CONDITION_FWD_H

