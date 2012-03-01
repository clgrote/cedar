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

    File:        exceptions.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 06 17

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/exceptions.h"

// SYSTEM INCLUDES
#include "cedar/auxiliaries/math/tools.h"

cedar::aux::UnhandledTypeException::UnhandledTypeException()
{
}

cedar::aux::UnknownTypeException::UnknownTypeException()
{
}

cedar::aux::ParameterNotFoundException::ParameterNotFoundException()
{
}

cedar::aux::DuplicateIdException::DuplicateIdException()
{
}

cedar::aux::DuplicateNameException::DuplicateNameException()
{
}

cedar::aux::UnknownNameException::UnknownNameException()
{
}

cedar::aux::UnhandledValueException::UnhandledValueException()
{
}

cedar::aux::RangeException::RangeException()
{
}

cedar::aux::NoDefaultException::NoDefaultException()
{
}

cedar::aux::InvalidNameException::InvalidNameException()
{
}

cedar::aux::TypeMismatchException::TypeMismatchException()
{
}

cedar::aux::BadConnectionException::BadConnectionException()
{
}

cedar::aux::FailedAssertionException::FailedAssertionException()
{
}

cedar::aux::IndexOutOfRangeException::IndexOutOfRangeException()
{
}

cedar::aux::NullPointerException::NullPointerException()
{
}

cedar::aux::InitializationException::InitializationException()
{
}

cedar::aux::DeadReferenceException::DeadReferenceException()
{
}

cedar::aux::ResourceNotFoundException::ResourceNotFoundException()
{
}

cedar::aux::MatrixMismatchException::MatrixMismatchException(const cv::Mat& matA, const cv::Mat& matB)
{
  unsigned int dim_a = cedar::aux::math::getDimensionalityOf(matA);
  unsigned int dim_b = cedar::aux::math::getDimensionalityOf(matA);

  std::string message = "The sizes or types of the matrices mismatch.\n";
  message += "First matrix is:\ndim: ";
  message += cedar::aux::toString(dim_a) + ", size: " + cedar::aux::math::matrixSizeToString(matA);
  message += "Second matrix is:\ndim: ";
  message += cedar::aux::toString(dim_b) + ", size: " + cedar::aux::math::matrixSizeToString(matB);
  this->setMessage(message);
}
