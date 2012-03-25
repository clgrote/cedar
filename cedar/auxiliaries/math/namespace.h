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

    File:        namespace.h

    Maintainer:  Mathis Richter
    Email:       mathis.richter@ini.rub.de
    Date:        2010 11 11

    Description: Namespace file for cedar::aux::math.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_MATH_NAMESPACE_H
#define CEDAR_AUX_MATH_NAMESPACE_H

// CEDAR INCLUDES
#include "cedar/namespace.h"
#include "cedar/auxiliaries/namespace.h"
#include "cedar/defines.h"

// SYSTEM INCLUDES
#include <boost/smart_ptr.hpp>


namespace cedar
{
  namespace aux
  {
    /*!@brief Namespace for all math classes. */
    namespace math
    {
      //!@brief a templated class for representing limits (i.e. an interval) of some type
      template <typename T> struct Limits;

      //!@cond SKIPPED_DOCUMENTATION
      CEDAR_DECLARE_AUX_CLASS(Sigmoid);
      CEDAR_DECLARE_AUX_CLASS(AbsSigmoid);
      CEDAR_DECLARE_AUX_CLASS(ExpSigmoid);
      CEDAR_DECLARE_AUX_CLASS(HeavisideSigmoid);
      CEDAR_DECLARE_AUX_CLASS(SigmoidDeclaration);
      //!@endcond

      //!@brief a templated declaration for sigmoid function implementation
      template <class DerivedClass> class SigmoidDeclarationT;

      typedef cedar::aux::Factory<SigmoidPtr> SigmoidFactory;

      //!@brief The manager of all sigmoind instances
      typedef cedar::aux::FactoryManager<SigmoidPtr> SigmoidManager;

      CEDAR_GENERATE_POINTER_TYPES(SigmoidFactory);

      //!@brief The singleton object of the SigmoidFactory.
      typedef cedar::aux::Singleton<SigmoidManager> SigmoidManagerSingleton;
    }
  }
}

#endif // CEDAR_AUX_MATH_NAMESPACE_H
