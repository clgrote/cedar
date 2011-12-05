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

    File:        KinematicChain.h

    Maintainer:  Hendrik Reimann
    Email:       hendrik.reimann@ini.rub.de
    Date:        2010 08 30

    Description: Chain of joints (e.g., a robotic arm).

    Credits:

======================================================================================================================*/


#ifndef CEDAR_DEV_ROBOT_KINEMATIC_CHAIN_H
#define CEDAR_DEV_ROBOT_KINEMATIC_CHAIN_H

// LOCAL INCLUDES
#include "cedar/devices/robot/namespace.h"
#include "cedar/devices/robot/Component.h"
#include "cedar/auxiliaries/LoopedThread.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>


/*!@brief Interface for a chain of joints
 *
 * This interface lets you to set angle, velocity and acceleration values for
 * each joint of the kinematic chain. If your hardware (or driver) does not
 * allow you to control velocities or accelerations directly, you can start the
 * the KinematicChain as a thread to handle velocities and accelerations
 * "manually".
 */
class cedar::dev::robot::KinematicChain : public cedar::dev::robot::Component, public cedar::aux::LoopedThread
{
  //----------------------------------------------------------------------------
  // macros
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // parameters
  //----------------------------------------------------------------------------
public:
  //!@brief The different modes to operate the kinematic chain
  enum ActionType { ANGLE, VELOCITY, ACCELERATION };
protected:
  // none yet
private:
  // none yet

  //----------------------------------------------------------------------------
  // constructors and destructor
  //----------------------------------------------------------------------------
public:
  //!@brief constructor
  KinematicChain(const cedar::dev::robot::ReferenceGeometryPtr& rpReferenceGeometry);
  //!@brief constructor
  KinematicChain(const std::string& configFileName);
  //!@brief destructor
  virtual ~KinematicChain() = 0;

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------
public:
  /*!@brief get reference geometry for this kinematic chain
   *
   * @return    reference geometry
   */
  const ReferenceGeometryPtr& getReferenceGeometry() const;

  /*!@brief get number of joints in this kinematic chain
   *
   * @return    number of joints
   */
  unsigned int getNumberOfJoints() const;

  /*!@brief set a new reference geometry
   *
   * @param geometry    new reference geometry
   */
  void setReferenceGeometry(const ReferenceGeometryPtr& geometry);

  /*!@brief get current state of a single joint angle
   *
   * @param index    specifies the joint
   * @return    joint angle value
   */
  virtual double getJointAngle(unsigned int index) = 0;

  /*!@brief get current state of all joint angles
   *
   * @return    vector of joint angles
   */
  std::vector<double> getJointAngles();

  /*!@brief get current state of all joint angles
   *
   * @return    vector of joint angles
   */
  cv::Mat getJointAnglesMatrix();

  /*!@brief get current state of a single joint velocity
   *
   * @param index    specifies the joint
   * @return    joint velocity value
   */
  virtual double getJointVelocity(unsigned int index);

  /*!@brief get current state of all joint velocities
   *
   * @return    vector of joint velocities
   */
  std::vector<double> getJointVelocities();

  /*!@brief get current state of all joint velocities
   *
   * @return    vector of joint velocities
   */
  cv::Mat getJointVelocitiesMatrix();

  /*!@brief get current state of a single joint acceleration
   *
   * @param index    specifies the joint
   * @return    joint acceleration value
   */
  virtual double getJointAcceleration(unsigned int index);

  /*!@brief get current state of all joint accelerations
   *
   * @return    vector of joint accelerations
   */
  std::vector<double> getJointAccelerations();

  /*!@brief get current state of all joint accelerations
   *
   * @return    vector of joint accelerations
   */
  cv::Mat getJointAccelerationsMatrix();

  /*!@brief returns the mode in which the joints positions are set (angle/velocity/acceleration)
   *
   * @return current working mode
   */
  ActionType getWorkingMode();

  /*!@brief set current state of a single joint angle
   *
   * @param index    specifies the joint
   * @param angle    new joint angle value
   */
  virtual void setJointAngle(unsigned int index, double angle) = 0;

  /*!@brief set current state of all joint angles
   *
   * @param angles    Matrix of new joint angle values
   */
  void setJointAngles(const cv::Mat& angles);

  /*!@brief set current state of all joint angles
   *
   * @param angles    vector of new joint angle values
   */
  void setJointAngles(const std::vector<double>& angles);

  /*!@brief set current state of a single joint velocity
   *
   * The KinematicChain base class will always return false here. If your
   * device actually has its own velocity control you would probably like to
   * override this method and return true here. By returning true you indicate
   * to the KinematicChain base class that no integration is necessary.
   *
   * @param index    specifies the joint
   * @param velocity    new joint velocity value
   * @return true iff your subclass handles velocity itself
   */
  virtual bool setJointVelocity(unsigned int index, double velocity);

  /*!@brief set current state of all joint velocities
   *
   * The KinematicChain base class will always return false here. If your
   * device actually has its own velocity control and you want to override this
   * method, then you have to return true here. By returning true you indicate
   * to the KinematicChain base class that no integration is necessary.
   *
   * Note that it is sufficient to override setJointVelocity(unsigned int, double).
   *
   * @param velocities    vector of new joint velocity values
   * @return true iff your subclass handles velocity itself
   */
  bool setJointVelocities(const cv::Mat& velocities);

  /*!@brief set current state of all joint velocities
   *
   * The KinematicChain base class will always return false here. If your
   * device actually has its own velocity control and you want to override this
   * method, then you have to return true here. By returning true you indicate
   * to the KinematicChain base class that no integration is necessary.
   *
   * Note that it is sufficient to override setJointVelocity(unsigned int, double).
   *
   * @param velocities    vector of new joint velocity values
   * @return true iff your subclass handles velocity itself
   */
  bool setJointVelocities(const std::vector<double>& velocities);

  /*!@brief set current state of a single joint acceleration
   *
   * The KinematicChain base class will always return false here. If your
   * device actually has its own acceleration control you would probably like to
   * override this method and return true here. By returning true you indicate
   * to the KinematicChain base class that no integration is necessary.
   *
   * @param index    specifies the joint
   * @param acceleration    new joint acceleration value
   * @return true iff your subclass handles acceleration itself
   */
  virtual bool setJointAcceleration(unsigned int index, double acceleration);

  /*!@brief set current state of all joint velocities
   *
   * The KinematicChain base class will always return false here. If your
   * device actually has its own acceleration control and you want to override
   * this method, then you have to return true here. By returning true you
   * indicate to the KinematicChain base class that no integration is necessary.
   *
   * @param accelerations    vector of new joint velocity values
   * @return true iff your subclass handles acceleration itself
   */
  bool setJointAccelerations(const cv::Mat& accelerations);

  /*!@brief set current state of all joint velocities
   *
   * The KinematicChain base class will always return false here. If your
   * device actually has its own acceleration control and you want to override
   * this method, then you have to return true here. By returning true you
   * indicate to the KinematicChain base class that no integration is necessary.
   *
   * @param accelerations    vector of new joint velocity values
   * @return true iff your subclass handles acceleration itself
   */
  bool setJointAccelerations(const std::vector<double>& accelerations);

  /*!@brief Sets the mode in which the joints positions are set (angle/velocity/acceleration)
   *
   * Setting a working mode will also stop the KinematicChain to allow you to
   * set new target values for each joint. You have to restart the
   * KinematicChain afterwards if you want it to take care of velocities or
   * accelerations. If your hardware lets you set these values directly you
   * do not want to start the thread of KinematicChain.
   *
   * @param actionType new working mode
   */
  virtual void setWorkingMode(ActionType actionType);

  /*!@brief Controls if real hardware values are used when integrating velocity/acceleration.
   *
   * For hardware that does not support velocity or acceleration control you can
   * start the KinematicChain as a thread to simulate this behavior. For such a
   * simulation a vector of ideal angle/velocity values is kept. But if you
   * trust your hardware to be fast and reliable enough, you can also integrate
   * using the current hardware values.
   *
   * @param useCurrentHardwareValues
   */
  void useCurrentHardwareValues(bool useCurrentHardwareValues);


  /*!@brief Starts the kinematic chain as a thread
   *
   * If you want to use velocity or acceleration control but your hardware
   * does not support this, start the thread to "simulate" these values.
   */
  virtual void start(Priority priority = InheritPriority);


  //----------------------------------------------------------------------------
  // protected methods
  //----------------------------------------------------------------------------
protected:

  //----------------------------------------------------------------------------
  // private methods
  //----------------------------------------------------------------------------
private:
  void step(double time);
  void init();
  void applyAngleLimits(cv::Mat &angles);
  void applyVelocityLimits(cv::Mat &velocities);

  //----------------------------------------------------------------------------
  // members
  //----------------------------------------------------------------------------
public:
  // none yet (hopefully never!)
protected:
  //!@brief geometry in reference configuration
  cedar::dev::robot::ReferenceGeometryPtr mpReferenceGeometry;
private:
  bool mUseCurrentHardwareValues;
  cv::Mat mJointAngles;
  cv::Mat mJointVelocities;
  cv::Mat mJointAccelerations;
  ActionType mCurrentWorkingMode;

}; // class cedar::dev::robot::KinematicChain

#endif // CEDAR_DEV_ROBOT_KINEMATIC_CHAIN_H

