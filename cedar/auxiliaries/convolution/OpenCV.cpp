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

    File:        OpenCV.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2012 03 14

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/convolution/OpenCV.h"
#include "cedar/auxiliaries/convolution/BorderType.h"
#include "cedar/auxiliaries/kernel/Separable.h"
#include "cedar/auxiliaries/FactoryManager.h"
#include "cedar/auxiliaries/Singleton.h"
#include "cedar/auxiliaries/math/tools.h"
#include "cedar/auxiliaries/casts.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// register type with the factory
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool registered
    = cedar::aux::conv::ConvolutionManagerSingleton::getInstance()->registerType<cedar::aux::conv::OpenCVPtr>();
}

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::conv::OpenCV::OpenCV()
{
  this->getKernelList().connectToKernelAddedSignal(boost::bind(&cedar::aux::conv::OpenCV::updateKernelType, this, _1));
  this->getKernelList().connectToKernelChangedSignal(boost::bind(&cedar::aux::conv::OpenCV::updateKernelType, this, _1));
  this->getKernelList().connectToKernelRemovedSignal(boost::bind(&cedar::aux::conv::OpenCV::updateKernelType, this, _1));
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

cv::Mat cedar::aux::conv::OpenCV::convolve(const cv::Mat& matrix) const
{
  CEDAR_DEBUG_ASSERT(this->getKernelList().size() == this->mKernelTypes.size());
  //!@todo Thread safety!
  //!@todo Remove the kernel convolveWith method once this is done

  //!@todo Proper hot-spot handling
  cv::Point anchor = cv::Point(-1, -1);

  //!@todo Proper border handling
  int border_type;
  switch (this->getBorderType())
  {
    case cedar::aux::conv::BorderType::Cyclic:
      border_type = cv::BORDER_WRAP;
      break;

    case cedar::aux::conv::BorderType::Reflect:
      border_type = cv::BORDER_REFLECT;
      break;

    case cedar::aux::conv::BorderType::Replicate:
      border_type = cv::BORDER_REPLICATE;
      break;

    case cedar::aux::conv::BorderType::Zero:
      //!@todo This actually makes the border -1 rather than 0!
      border_type = cv::BORDER_CONSTANT;
      break;

    default:
      border_type = cv::BORDER_DEFAULT;
  }

  double delta = 0.0;

  int destination_depth = -1; // -1 == same as input

  cv::Mat result = 0.0 * matrix;
  for (size_t i = 0; i < this->getKernelList().size(); ++i)
  {
    cv::Mat convolved;

    switch (this->mKernelTypes.at(i))
    {
      //--------------------------------------------------------------------------------------
      case KERNEL_TYPE_SEPARABLE:
      //--------------------------------------------------------------------------------------
      {
        cedar::aux::kernel::ConstSeparablePtr kernel
          = cedar::aux::asserted_pointer_cast<const cedar::aux::kernel::Separable>(this->getKernelList().getKernel(i));

        switch (cedar::aux::math::getDimensionalityOf(matrix))
        {
          case 1:
          {
            const cv::Mat& kernel_mat = kernel->getKernelPart(0);
            cv::filter2D(matrix, convolved, destination_depth, kernel_mat, anchor, delta, border_type);
            break;
          }

          case 2:
          {
            CEDAR_DEBUG_ASSERT(kernel->kernelPartCount() == 2);
            const cv::Mat& kernel_mat_x = kernel->getKernelPart(1);
            const cv::Mat& kernel_mat_y = kernel->getKernelPart(0);

            cv::sepFilter2D
            (
              matrix,
              convolved,
              destination_depth,
              kernel_mat_x,
              kernel_mat_y,
              anchor,
              delta,
              border_type
            );

            break;
          }

          default:
            CEDAR_THROW(cedar::aux::UnhandledValueException, "Cannot convolve matrices of the given dimensionality.");
        }

        break;
      }

      //--------------------------------------------------------------------------------------
      case KERNEL_TYPE_FULL:
      //--------------------------------------------------------------------------------------
      {
        cedar::aux::kernel::ConstKernelPtr kernel = this->getKernelList().getKernel(i);
        cv::Mat kernel_mat = kernel->getKernel();
        switch (cedar::aux::math::getDimensionalityOf(matrix))
        {
          case 1:
          case 2:
            cv::filter2D(matrix, convolved, destination_depth, kernel_mat, anchor, delta, border_type);
            break;

          default:
            CEDAR_THROW(cedar::aux::UnhandledValueException, "Cannot convolve matrices of the given dimensionality.");
        }
        break;
      }

      //--------------------------------------------------------------------------------------
      case KERNEL_TYPE_UNKNOWN:
      //--------------------------------------------------------------------------------------
        CEDAR_THROW(cedar::aux::UnknownTypeException, "Unknown kernel type.");
        break;

      default:
        CEDAR_THROW(cedar::aux::UnhandledValueException, "Unhandled kernel-type enum value.");
    }

    result += convolved;
  }
  return result;
}

void cedar::aux::conv::OpenCV::updateKernelType(size_t index)
{
  if (this->mKernelTypes.size() <= index)
  {
    this->mKernelTypes.resize(index + 1, KERNEL_TYPE_UNKNOWN);
  }

  cedar::aux::kernel::ConstKernelPtr kernel = this->getKernelList().getKernel(index);
  if (boost::dynamic_pointer_cast<const cedar::aux::kernel::Separable>(kernel))
  {
    this->mKernelTypes.at(index) = KERNEL_TYPE_SEPARABLE;
  }
  else
  {
    this->mKernelTypes.at(index) = KERNEL_TYPE_FULL;
  }
}
