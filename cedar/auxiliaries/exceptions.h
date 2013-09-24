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

    File:        exceptions.h

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 06 17

    Description: Header file for exceptions in the cedar::aux namespace.

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
}; // class cedar::aux::UnhandledTypeException

/*!@brief Exception that occurs when a value is not handled (e.g. out of bounds).
 */
class cedar::aux::UnhandledValueException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::UnhandledValueException

/*!@brief Exception that occurs when a type is not known.
 */
class cedar::aux::UnknownTypeException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::UnknownTypeException

/*!@brief Exception that occurs when a name is not known.
 */
class cedar::aux::UnknownNameException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::UnknownNameException

/*!@brief Exception that signals that something could not be found.
 */
class cedar::aux::NotFoundException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::NotFoundException

/*!@brief Exception that signals that something could not be found.
 */
class cedar::aux::AnnotationNotFoundException : public cedar::aux::NotFoundException
{
}; // class cedar::aux::AnnotationNotFoundException

/*!@brief Exception that occurs when a parameter is not found.
 */
class cedar::aux::ParameterNotFoundException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::ParameterNotFoundException

/*!@brief Exception that occurs when a unique id appears twice.
 */
class cedar::aux::DuplicateIdException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::DuplicateIdException

/*!@brief Exception that occurs when a unique name appears twice.
 */
class cedar::aux::DuplicateNameException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::DuplicateNameException

/*!@brief Exception that occurs when a value leaves a certain range.
 */
class cedar::aux::RangeException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::RangeException

/*!@brief Exception that occurs when no default case is present but was reached somehow.
 */
class cedar::aux::NoDefaultException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::NoDefaultException

/*!@brief Exception that occurs when a name is not valid (e.g. contains special characters).
 */
class cedar::aux::InvalidNameException : public cedar::aux::ExceptionBase
{
};

/*!@brief Exception that occurs when an given type does not match an expected type.
 */
class cedar::aux::TypeMismatchException : public cedar::aux::ExceptionBase
{
};

/*!@brief Exception that occurs when an given dimensionalities do not match.
 */
class cedar::aux::DimensionalityMismatchException : public cedar::aux::ExceptionBase
{
};

/*!@brief Exception thrown when a connection (i.e. a network connection) is too bad to continue.
 */
class cedar::aux::BadConnectionException : public cedar::aux::ExceptionBase
{
};

/*!@brief A failed assertion exception.
 */
class cedar::aux::FailedAssertionException : public cedar::aux::ExceptionBase
{
};

/*!@brief An exception that is thrown each time an index gets out of range.
 */
class cedar::aux::IndexOutOfRangeException : public cedar::aux::ExceptionBase
{
};

/*!@brief A null pointer exception.
 *
 * Incidentally, this exception should be thrown when a null-pointer is accessed.
 */
class cedar::aux::NullPointerException : public cedar::aux::ExceptionBase
{
};

/*!@brief Exception for initialization errors.
 *
 * This exception should be thrown when an error occurs during initialization.
  */
class cedar::aux::InitializationException : public cedar::aux::ExceptionBase
{
};

/*!@brief Exception that is thrown when unmangling of a name fails.
 */
class cedar::aux::UnmanglingFailedException : public cedar::aux::ExceptionBase
{
};

/*!@brief Exception that is thrown when a singleton object does not exist any more.
 */
class cedar::aux::DeadReferenceException : public cedar::aux::ExceptionBase
{
};

/*!@brief Exception that is thrown when two matrices do not match in type or size.
 */
class cedar::aux::MatrixMismatchException : public cedar::aux::ExceptionBase
{
public:
  //!@brief constructor that takes the two mismatching matrices to generate detailed error information
  MatrixMismatchException(const cv::Mat& matA, const cv::Mat& matB);
};

/*!@brief Exception that is thrown when a resource cannot be located by standard means.
 */
class cedar::aux::ResourceNotFoundException : public cedar::aux::ExceptionBase
{
};

/*!@brief Exception that is thrown when unmangling of a name fails.
 */
class cedar::aux::FileNotFoundException : public cedar::aux::ExceptionBase
{
};

/*!@brief Exception that occurs when a string cannot be converted to a certain type.
 */
class cedar::aux::ConversionFailedException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::UnhandledTypeException

/*!@brief Exception that occurs when a parameter validation fails.
 */
class cedar::aux::ValidationFailedException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::ValidationFailedException

/*!@brief Exception that occurs when a parameter validation fails.
 */
class cedar::aux::UnknownUnitSuffixException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::UnknownUnitSuffixException

//! Exception that signals an invalid path.
class cedar::aux::InvalidPathException : public cedar::aux::ExceptionBase
{
};

/*!@brief An exception thrown when a method is not implemented.
 */
class cedar::aux::NotImplementedException : public cedar::aux::ExceptionBase
{
};

/*!@brief Exception that occurs when threading code is mis-used
 */
class cedar::aux::ThreadingErrorException : public cedar::aux::ExceptionBase
{
}; // class cedar::aux::ThreadingErrorException

/*!@brief An exception that is thrown, when reading or writing jsons encounters a parsing error.
 */
class cedar::aux::ParseException: public cedar::aux::ExceptionBase
{
}; // class cedar::proc::ParseException

/*!@brief An exception that is thrown, when a function is not implemented.
 */
class cedar::aux::NotImplementedException: public cedar::aux::ExceptionBase
{
}; // class cedar::aux::NotImplementedException

/*!@brief An exception that is thrown, when a thread is running and an operation
 *        is called which is not allowed.
 * @todo  This exception needs a better name
 */
class cedar::aux::ThreadRunningExeption: public cedar::aux::ExceptionBase
{
}; // class cedar::aux::ThreadRunningExeption


#endif // CEDAR_AUX_EXCEPTIONS_H
