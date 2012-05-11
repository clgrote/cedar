/*=============================================================================

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

===============================================================================

    Institute:   Ruhr-Universitaet Bochum
                 Institut fuer Neuroinformatik

    File:        CollatedTraits.h

    Maintainer:  Jean-Stephane Jokeit
    Email:       jean-stephane.jokeit@ini.ruhr-uni-bochum.de
    Date:        Wed 20 Jul 2011 04:43:49 PM CEST

    Description: CollatedTraits in the opencv specialization

    Credits:

=============================================================================*/

#ifndef CEDAR_COLLATEDTRAITS_OPENCV_H
#define CEDAR_COLLATEDTRAITS_OPENCV_H

// LOCAL INCLUDES
// traits specializations:
#include "cedar/auxiliaries/net/detail/namespace.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <opencv2/opencv.hpp>


namespace cedar {
  namespace aux {
    namespace net {
      namespace detail {
        template<> struct CollatedTraits<cv::Mat>;
        template<> struct CollatedTraits< cv::Mat_<float> >;
        template <typename CVT> class cvMatHelper;
        class cvMatNetHeader; // forward declaration
      }
    }
  }
}


//////////////// OPENCV SPECIALIZATION of our traits class

//!@brief CollatedTraits implementation for cv::Mat
template<>
struct cedar::aux::net::detail::CollatedTraits<cv::Mat>
{
  typedef cv::Mat                                        DataType;
  typedef cedar::aux::net::detail::cvMatHelper<cv::Mat>  HelperType;
  typedef cedar::aux::net::detail::cvMatNetHeader        HeaderType;
};

//!@brief CollatedTraits implementation for cv::Mat_<float> 
template<>
struct cedar::aux::net::detail::CollatedTraits< cv::Mat_<float> >
{
  typedef cv::Mat_<float>                                         DataType;
  typedef cedar::aux::net::detail::cvMatHelper< cv::Mat_<float> > HelperType;
  typedef cedar::aux::net::detail::cvMatNetHeader                 HeaderType;
};

#endif

