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

    File:        DoubleVectorParameter.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 20

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/processing/auxiliaries/gui/DoubleVectorParameter.h"
#include "cedar/auxiliaries/DoubleVectorParameter.h"
#include "cedar/processing/auxiliaries/TypeBasedFactory.h"
#include "cedar/processing/auxiliaries/Singleton.h"
#include "cedar/auxiliaries/assert.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// associate aux::gui parameter with the aux parameter
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool registered = cedar::proc::aux::gui::ParameterFactorySingleton::getInstance()->add
      <
        cedar::aux::DoubleVectorParameter,
        cedar::proc::aux::gui::DoubleVectorParameter
      >();
}

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::aux::gui::DoubleVectorParameter::DoubleVectorParameter(QWidget *pParent)
:
cedar::proc::aux::gui::DoubleVectorParameter::Base(pParent)
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::aux::gui::DoubleVectorParameter::widgetValueChanged(double)
{
  this->Base::widgetValueChanged(cedar::aux::asserted_cast<cedar::proc::aux::gui::IgnoreLocaleDoubleSpinBox*>(QObject::sender()));
}

