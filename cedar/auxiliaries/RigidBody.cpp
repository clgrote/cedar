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

    File:        RigidBody.cpp

    Maintainer:  Hendrik Reimann
    Email:       hendrik.reimann@ini.rub.de
    Date:        2010 12 04

    Description: implementation of cedar::aux::RigidBody class, providing geometry of a rigid object

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/RigidBody.h"
#include "cedar/auxiliaries/math/tools.h"
#include "cedar/auxiliaries/assert.h"

// SYSTEM INCLUDES
#include <opencv2/opencv.hpp>
#include <QReadLocker>
#include <QWriteLocker>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::aux::RigidBody::RigidBody()
:
mTransformation(4, 4, CV_64FC1),
mPosition(4, 1, CV_64FC1),
mOrientationQuaternion(4, 1, CV_64FC1),
mTransformationTranspose(4, 4, CV_64FC1)
{
  init();
}

cedar::aux::RigidBody::RigidBody(const std::string& configFileName)
:
cedar::aux::ConfigurationInterface(configFileName),
mTransformation(4, 4, CV_64FC1),
mPosition(4, 1, CV_64FC1),
mOrientationQuaternion(4, 1, CV_64FC1),
mTransformationTranspose(4, 4, CV_64FC1)
{
  init();
  addParameter(&_mName, "Name", "<name>");
  addParameter(&_mPosition, "RigidBody.position", 1.0);
  addParameter(&_mOrientation, "RigidBody.orientation", 0.0);
  readOrDefaultConfiguration();
  mPosition.at<double>(0, 0) = _mPosition[0];
  mPosition.at<double>(1, 0) = _mPosition[1];
  mPosition.at<double>(2, 0) = _mPosition[2];
  // read out quaternion from orientation _mOrientationrix
  double r;
  if (IsZero(_mOrientation[0] + _mOrientation[4] + _mOrientation[8] - 3))
  {
    mOrientationQuaternion.at<double>(0, 0) = 1;
  }
  else
  { //!\todo: this doesn't really take care of negatives, should compare absolute values, really
    //!\todo: this should be moved to a general transformation matrix -> quaternion function
    if (_mOrientation[0] > _mOrientation[4] && _mOrientation[0] > _mOrientation[8])
    {      // Column 0:
      r  = sqrt(1.0 + _mOrientation[0] - _mOrientation[4] - _mOrientation[8]) * 2.0;
      mOrientationQuaternion.at<double>(0, 0) = (_mOrientation[7] - _mOrientation[5]) / r;
      mOrientationQuaternion.at<double>(1, 0) = 0.25 * r;
      mOrientationQuaternion.at<double>(2, 0) = (_mOrientation[3] + _mOrientation[1]) / r;
      mOrientationQuaternion.at<double>(3, 0) = (_mOrientation[2] + _mOrientation[6]) / r;
    }
    else if (_mOrientation[4] > _mOrientation[8])
    {      // Column 1:
      r  = sqrt(1.0 + _mOrientation[4] - _mOrientation[0] - _mOrientation[8]) * 2.0;
      mOrientationQuaternion.at<double>(0, 0) = (_mOrientation[2] - _mOrientation[6]) / r;
      mOrientationQuaternion.at<double>(1, 0) = (_mOrientation[3] + _mOrientation[1]) / r;
      mOrientationQuaternion.at<double>(2, 0) = 0.25 * r;
      mOrientationQuaternion.at<double>(3, 0) = (_mOrientation[7] + _mOrientation[5]) / r;
    }
    else
    {            // Column 2:
      r  = sqrt(1.0 + _mOrientation[8] - _mOrientation[0] - _mOrientation[4]) * 2.0;
      mOrientationQuaternion.at<double>(0, 0) = (_mOrientation[3] - _mOrientation[1]) / r;
      mOrientationQuaternion.at<double>(1, 0) = (_mOrientation[6] + _mOrientation[2]) / r;
      mOrientationQuaternion.at<double>(2, 0) = (_mOrientation[5] + _mOrientation[7]) / r;
      mOrientationQuaternion.at<double>(3, 0) = 0.25 * r;
    }
  }
  updateTransformation();
}

cedar::aux::RigidBody::~RigidBody()
{

}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

cv::Mat cedar::aux::RigidBody::getPosition() const
{
  QReadLocker locker(&mLock);
  return mPosition.clone();
}

double cedar::aux::RigidBody::getPositionX() const
{
  return mPosition.at<double>(0, 0);
}

double cedar::aux::RigidBody::getPositionY() const
{
  return mPosition.at<double>(1, 0);
}

double cedar::aux::RigidBody::getPositionZ() const
{
  return mPosition.at<double>(2, 0);
}

cv::Mat cedar::aux::RigidBody::getOrientationQuaternion() const
{
  QReadLocker locker(&mLock);
  return mOrientationQuaternion.clone();
}

double cedar::aux::RigidBody::getOrientationQuaternion(unsigned int component) const
{
  QReadLocker locker(&mLock);
  return mOrientationQuaternion.at<double>(component, 0);
}

cv::Mat cedar::aux::RigidBody::getTransformation() const
{
  QReadLocker locker(&mLock);
  return mTransformation.clone();
}

void cedar::aux::RigidBody::setPosition(double x, double y, double z)
{
  mLock.lockForWrite();
  mPosition.at<double>(0, 0) = x;
  mPosition.at<double>(1, 0) = y;
  mPosition.at<double>(2, 0) = z;
  mLock.unlock();
  updateTransformation();
}

void cedar::aux::RigidBody::setPosition(const cv::Mat& position)
{
  mLock.lockForWrite();
  CEDAR_ASSERT(position.type() == mPosition.type());
  mPosition = position.clone();
  mLock.unlock();
  updateTransformation();
}

void cedar::aux::RigidBody::setOrientationQuaternion(const cv::Mat quaternion)
{
  mLock.lockForWrite();
  CEDAR_ASSERT(quaternion.type() == mOrientationQuaternion.type());
  mOrientationQuaternion = quaternion.clone();
  mLock.unlock();
  updateTransformation();
}

void cedar::aux::RigidBody::rotate(unsigned int axis, double angle)
{
  mLock.lockForWrite();
  // rotation quaternion
  cv::Mat q_rot = cv::Mat::zeros(4, 1, CV_64FC1);
  q_rot.at<double>(0, 0) = cos(angle/2.0);
  q_rot.at<double>(axis+1, 0) = sin(angle/2.0);

  // calculate new object orientation quaternion
  double a1 = mOrientationQuaternion.at<double>(0, 0);
  double b1 = mOrientationQuaternion.at<double>(1, 0);
  double c1 = mOrientationQuaternion.at<double>(2, 0);
  double d1 = mOrientationQuaternion.at<double>(3, 0);
  double a2 = q_rot.at<double>(0, 0);
  double b2 = q_rot.at<double>(1, 0);
  double c2 = q_rot.at<double>(2, 0);
  double d2 = q_rot.at<double>(3, 0);
  cv::Mat q_new = cv::Mat::zeros(4, 1, CV_64FC1);
  q_new.at<double>(0, 0) = a1*a2 - b1*b2 - c1*c2 - d1*d2;
  q_new.at<double>(1, 0) = a1*b2 + b1*a2 + c1*d2 - d1*c2;
  q_new.at<double>(2, 0) = a1*c2 - b1*d2 + c1*a2 + d1*b2;
  q_new.at<double>(3, 0) = a1*d2 + b1*c2 - c1*b2 + d1*a2;

  // set new quaternion
  mOrientationQuaternion = q_new;
  mLock.unlock();
  updateTransformation();
}

void cedar::aux::RigidBody::updateTransformation()
{
  QWriteLocker locker(&mLock);
  // calculate rotation matrix from orientation quaternion
  double a = mOrientationQuaternion.at<double>(0, 0);
  double b = mOrientationQuaternion.at<double>(1, 0);
  double c = mOrientationQuaternion.at<double>(2, 0);
  double d = mOrientationQuaternion.at<double>(3, 0);
  mTransformation.at<double>(0, 0) = a*a + b*b - c*c - d*d;
  mTransformation.at<double>(1, 0) = 2*b*c + 2*a*d;
  mTransformation.at<double>(2, 0) = 2*b*d - 2*a*c;

  mTransformation.at<double>(0, 1) = 2*b*c - 2*a*d;
  mTransformation.at<double>(1, 1) = a*a - b*b + c*c - d*d;
  mTransformation.at<double>(2, 1) = 2*c*d + 2*a*b;

  mTransformation.at<double>(0, 2) = 2*b*d + 2*a*c;
  mTransformation.at<double>(1, 2) = 2*c*d - 2*a*b;
  mTransformation.at<double>(2, 2) = a*a - b*b - c*c + d*d;

  // copy position
  mTransformation.at<double>(0, 3) = mPosition.at<double>(0, 0);
  mTransformation.at<double>(1, 3) = mPosition.at<double>(1, 0);
  mTransformation.at<double>(2, 3) = mPosition.at<double>(2, 0);

  mTransformation.at<double>(3, 3) = 1;
}

void cedar::aux::RigidBody::init()
{
  mPosition = 0.0;
  mPosition.at<double>(3, 0) = 1.0;
  mOrientationQuaternion = 0.0;
  mOrientationQuaternion.at<double>(0, 0) = 1.0;
  mTransformation = 0.0;
  mTransformationTranspose = 0.0;
  updateTransformation();
}
