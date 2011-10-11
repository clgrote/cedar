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

    File:        sigmoids.h

    Maintainer:  Hendrik Reimann
    Email:       hendrik.reimann@ini.rub.de
    Date:        2010 11 11

    Description: Sigmoid functions

    Credits:

======================================================================================================================*/
#ifndef CEDAR_AUX_MATH_SIGMOIDS_H
#define CEDAR_AUX_MATH_SIGMOIDS_H

// LOCAL INCLUDES
#include "auxiliaries/math/namespace.h"
#include "auxiliaries/lib.h"
#include "auxiliaries/macros.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <math.h>
#include <vector>
#include <opencv2/opencv.hpp>

/*
 *  different sigmoid functions for different data formats
 *  default is the exponential sigmoid, just called sigmoid()
 *  other kinds of sigmoids have more specific names
 */
namespace cedar
{
  namespace aux
  {
    namespace math
    {
      //! an exponential-based sigmoid for a single double value
      CEDAR_AUX_LIB_EXPORT CEDAR_DECLARE_DEPRECATED(double sigmoid(const double x, const double beta, const double threshold = 0));

      //! an exponential-based sigmoid for a single double value
      CEDAR_AUX_LIB_EXPORT double sigmoidExp(const double x, const double beta, const double threshold = 0);

      //! an abs-based sigmoid for a single double value
      CEDAR_AUX_LIB_EXPORT double sigmoidAbs(const double x, const double beta, const double threshold = 0);

      //! Heavyside function for a single double value
      CEDAR_AUX_LIB_EXPORT double sigmoidHeavyside(const double x, const double threshold = 0);

      //! a sigmoid that rises smoothly in an interval and is exactly zero resp. one outside that interval
      CEDAR_AUX_LIB_EXPORT double sigmoidInterval(const double value, const double t1, const double t2, const bool decreasing = false);

      //! an exponential-based sigmoid for a cv::Mat
      template<typename T>
      CEDAR_AUX_LIB_EXPORT cv::Mat sigmoid(const cv::Mat& mat, const double beta, const double threshold = 0);

      //! an exponential-based sigmoid for a cv::Mat, that takes the result as an argument and does not allocate new memory
      template<typename T>
      CEDAR_AUX_LIB_EXPORT void sigmoid(const cv::Mat& mat, cv::Mat& result, const double beta, const double threshold = 0);

      //! an abs-based sigmoid for a cv::Mat
      template<typename T>
      CEDAR_AUX_LIB_EXPORT cv::Mat sigmoidAbs(const cv::Mat& mat, const double beta, const double threshold = 0);

      //! an abs-based sigmoid for a cv::Mat, that takes the result as an argument and does not allocate new memory
      template<typename T>
      CEDAR_AUX_LIB_EXPORT void sigmoidAbs(const cv::Mat& mat, cv::Mat& result, const double beta, const double threshold = 0);

      //! a sigmoid for a vector of doubles
      CEDAR_AUX_LIB_EXPORT std::vector<double> sigmoid(const std::vector<double>& x, const double beta, const double threshold = 0);
    };
  };
};
#endif  // CEDAR_AUX_MATH_SIGMOIDS_H
