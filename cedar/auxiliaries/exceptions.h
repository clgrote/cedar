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

    File:        exceptions.h

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 06 17

    Description: Header file for exceptions in the cedar::aux::comp namespace.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_EXCEPTIONS_H
#define CEDAR_AUX_EXCEPTIONS_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/namespace.h"
#include "cedar/auxiliaries/ExceptionBase.h"

// SYSTEM INCLUDES
#include <opencv2/core/mat.hpp>

/*!@brief Exception that occurs when a data type is not handled (e.g. by a generic plotter).
 */
class cedar::aux::UnhandledTypeException : public cedar::aux::ExceptionBase
{
public:
    UnhandledTypeException();
}; // class cedar::aux::UnhandledTypeException

/*!@brief Exception that occurs when a value is not handled (e.g. out of bounds).
 */
class cedar::aux::UnhandledValueException : public cedar::aux::ExceptionBase
{
public:
    UnhandledValueException();
}; // class cedar::aux::UnhandledValueException

/*!@brief Exception that occurs when a type is not known.
 */
class cedar::aux::UnknownTypeException : public cedar::aux::ExceptionBase
{
public:
    UnknownTypeException();
}; // class cedar::aux::UnknownTypeException

/*!@brief Exception that occurs when a name is not known.
 */
class cedar::aux::UnknownNameException : public cedar::aux::ExceptionBase
{
public:
    UnknownNameException();
}; // class cedar::aux::UnknownNameException

/*!@brief Exception that occurs when a parameter is not found.
 */
class cedar::aux::ParameterNotFoundException : public cedar::aux::ExceptionBase
{
public:
    ParameterNotFoundException();
}; // class cedar::aux::ParameterNotFoundException

/*!@brief Exception that occurs when a unique id appears twice.
 */
class cedar::aux::DuplicateIdException : public cedar::aux::ExceptionBase
{
public:
    DuplicateIdException();
}; // class cedar::aux::DuplicateIdException

/*!@brief Exception that occurs when a unique name appears twice.
 */
class cedar::aux::DuplicateNameException : public cedar::aux::ExceptionBase
{
public:
    DuplicateNameException();
}; // class cedar::aux::DuplicateNameException

/*!@brief Exception that occurs when a value leaves a certain range.
 */
class cedar::aux::RangeException : public cedar::aux::ExceptionBase
{
public:
  RangeException();
}; // class cedar::aux::RangeException

/*!@brief Exception that occurs when no default case is present but was reached somehow.
 */
class cedar::aux::NoDefaultException : public cedar::aux::ExceptionBase
{
public:
  NoDefaultException();
}; // class cedar::aux::NoDefaultException

/*!@brief Exception that occurs when a name is not valid (e.g. contains special characters).
 */
class cedar::aux::InvalidNameException : public cedar::aux::ExceptionBase
{
public:
    InvalidNameException();
};

/*!@brief Exception that occurs when an given type does not match an expected type.
 */
class cedar::aux::TypeMismatchException : public cedar::aux::ExceptionBase
{
public:
    TypeMismatchException();
};

/*!@brief Exception thrown when a connection (i.e. a network connection) is too bad to continue.
 */
class cedar::aux::BadConnectionException : public cedar::aux::ExceptionBase
{
public:
  BadConnectionException();
};

/*!@brief A failed assertion exception.
 */
class cedar::aux::FailedAssertionException : public cedar::aux::ExceptionBase
{
public:
  FailedAssertionException();
};

/*!@brief An exception that is thrown each time an index gets out of range.
 */
class cedar::aux::IndexOutOfRangeException : public cedar::aux::ExceptionBase
{
public:
  IndexOutOfRangeException();
};

/*!@brief A null pointer exception.
 *
 * Incidentally, this exception should be thrown when a null-pointer is accessed.
 */
class cedar::aux::NullPointerException : public cedar::aux::ExceptionBase
{
public:
  NullPointerException();
};

/*!@brief Exception for initialization errors.
 *
 * This exception should be thrown when an error occurs during initialization.
  */
class cedar::aux::InitializationException : public cedar::aux::ExceptionBase
{
public:
  InitializationException();
};

/*!@brief Exception that is thrown when unmangling of a name fails.
 */
class cedar::aux::UnmanglingFailedException : public cedar::aux::ExceptionBase
{
public:
  UnmanglingFailedException()
  {
    this->mType = "UnmanglingFailedException";
  }
};

/*!@brief Exception that is thrown when a singleton object does not exist any more.
 */
class cedar::aux::DeadReferenceException : public cedar::aux::ExceptionBase
{
public:
  DeadReferenceException();
};

/*!@brief Exception that is thrown when two matrices do not match in type or size.
 */
class cedar::aux::MatrixMismatchException : public cedar::aux::ExceptionBase
{
public:
  //!@brief constructor that takes the two mismatching matrices to generate detailed error information
  MatrixMismatchException(const cv::Mat& matA, const cv::Mat& matB);
};

/*!@brief Exception that is thrown when two matrices do not match in type or size.
 */
class cedar::aux::ResourceNotFoundException : public cedar::aux::ExceptionBase
{
public:
  //!@brief constructor that takes the two mismatching matrices to generate detailed error information
  ResourceNotFoundException();
};

#endif // CEDAR_AUX_EXCEPTIONS_H
