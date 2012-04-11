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

    File:        Sigmoid.h

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 05

    Description: Sigmoid functions

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_MATH_SIGMOID_H
#define CEDAR_AUX_MATH_SIGMOID_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/math/namespace.h"
#include "cedar/auxiliaries/Configurable.h"
#include "cedar/auxiliaries/DoubleParameter.h"

// SYSTEM INCLUDES

/*!@brief Basic interface for all sigmoid functions.
 */
class cedar::aux::math::Sigmoid : public Configurable
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  Sigmoid(double threshold = 0.0)
  :
  mThreshold(new DoubleParameter(this, "threshold", threshold, -1000.0, 1000.0))
  {
  }

  //!@brief Destructor
  virtual ~Sigmoid();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*!@brief this function calculates the sigmoid function for a given double value.
   * All inherited classes have to implement this function.
   */
  virtual double compute(double value) const = 0;

  /*!@brief this function calculates the sigmoid function for a given float value.
   * Included for backward-compatibility
   */
  virtual float compute(float value) const;

  /*!@brief this function calculates the sigmoid function for an n-dimensional matrix.
   *
   * @todo write a non-templated function, which checks the type flag of cv::Mat and calls the correct templated compute
   * function
   */
  template<typename T>
  cv::Mat compute(const cv::Mat& values) const
  {
    cv::Mat result = values.clone();
    cv::MatConstIterator_<T> iter_src = values.begin<T>();
    cv::MatIterator_<T> iter_dest = result.begin<T>();
    for ( ; iter_src != values.end<T>(); ++iter_src, ++iter_dest)
    {
      *iter_dest = static_cast<T>(compute(static_cast<double>(*iter_src)));
    }
    return result;
  }

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //!@brief threshold of the sigmoid
  cedar::aux::DoubleParameterPtr mThreshold;
private:
  // none yet
};

#include "cedar/auxiliaries/FactoryManager.h"

namespace cedar
{
  namespace aux
  {
    namespace math
    {
      //!@brief The manager of all sigmoind instances
      typedef cedar::aux::FactoryManager<SigmoidPtr> SigmoidManager;

#ifdef MSVC
#ifdef CEDAR_LIB_EXPORTS_AUX
      // dllexport
      template class __declspec(dllexport) cedar::aux::Singleton<SigmoidManager>;
#else // CEDAR_LIB_EXPORTS_AUX
    // dllimport
      extern template class __declspec(dllimport) cedar::aux::Singleton<SigmoidManager>;
#endif // CEDAR_LIB_EXPORTS_AUX
#endif // MSVC

      //!@brief The singleton object of the SigmoidFactory.
      typedef cedar::aux::Singleton<SigmoidManager> SigmoidManagerSingleton;
    }
  }
}
#endif  // CEDAR_AUX_MATH_SIGMOID_H
