/*----------------------------------------------------------------------------------------------------------------------
 ----- Institute:   Ruhr-Universitaet Bochum
                    Institut fuer Neuroinformatik

 ----- File:        KukaInterface.cpp

 ----- Author:      Guido Knips

 ----- Email:       guido.knips@ini.rub.de

 ----- Date:        2010 11 23

 ----- Description: cedar-Interface for the KUKA LBR

 ----- Credits:
 ---------------------------------------------------------------------------------------------------------------------*/
//Ready for some KukaIn?


#ifndef CEDAR_DEV_ROBOT_KUKA_KUKA_INTERFACE_H
#define CEDAR_DEV_ROBOT_KUKA_KUKA_INTERFACE_H

// LOCAL INCLUDES (includes from this project)
#include "namespace.h"

// PROJECT INCLUDES
#include "devices/robot/KinematicChain.h"
#include "auxiliaries/ConfigurationInterface.h"

// SYSTEM INCLUDES
#include <fri/friremote.h>
#include <QReadWriteLock>


/*!@brief cedar Interface for the KUKA LBR

 * This class wraps the KUKA Fast Research Interface (FRI)
 */
class cedar::dev::robot::kuka::KukaInterface :  public cedar::dev::robot::KinematicChain
{
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*!@brief Constructor that takes the name of the configuration file to use with the object.
   *
   * @param configFileName    Name of the configuration file containing the parameters
   */
  KukaInterface(const std::string& configFileName);

  /*!the Destructor*/
  virtual ~KukaInterface();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*! @brief returns angle for a specified joint
   *
   *  @param index  index of the joint, since the KUKA LBR has seven of them, it must be in the interval [0,6]
   *  @return joint angle for the given index
   */
  virtual double getJointAngle(unsigned int index);
  /*! @brief returns all joint angles
   *
   *  @return a vector filled with the joint angles
   *  \throws cedar::aux::exc::IndexOutOfRangeException if index is bigger than allowed
   */
  virtual void setJointAngle(unsigned int index, double angle);
  /*!@brief Sets the mode in which the joints positions are set (angle/velocity/acceleration)
   *
   * this function restarts the looped thread
   * @param actionType new working mode
   */
  virtual void setWorkingMode(cedar::dev::robot::KinematicChain::ActionType actionType);

  /*!@brief starts the looped thread
   *
   * the KinematicChain class does some things in this function that are not needed
   * @param priority thread priority
   */
  virtual void start(Priority priority = InheritPriority);

  /*Wrapping of some FRI-Functions that are needed for ensuring connection quality*/

  /*! @brief returns the state of the Interface.
   *
   * this can be FRI_STATE_OFF, FRI_STATE_MON and FRI_STATE_CMD
   * Commands can only be send if the state is FRI_STATE_CMD, which represents the command mode
   * @return current state of the interface
   */
  FRI_STATE getFriState();
  /*! @brief returns the quality of the connection.
   *
   * this can range from FRI_QUALITY_UNACCEPTABLE to FRI_QUALITY_PERFECT
   * if the Quality is worse (means: less) than FRI_QUALITY_GOOD, command mode switches to monitor mode automatically
   * @return current Quality of the connection
   */
  FRI_QUALITY getFriQuality();
  /*! @brief returns sample time of the FRI

   * The sample time is set on the FRI server. Each interval with the length of the sample time, data will be exchanged
   * @return FRI sample time
   */
  float getSampleTime();
  /*! @brief check if the robot is powered
   *
   * this especially means the dead man switch is in the right position and the robot is in command mode
   * @return true, if power is on
   */
  bool isPowerOn();

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  /*!@brief This method initializes the object.
   *
   * This method is called from all constructors of the class.
   * @param commandMode establish command mode if true
   */
  void init();
  /*!@brief every step is used to do communication between FRI and KUKA-RC
   *
   * if in velocity- or acceleration mode, every step will also change joint angles/velocity
   * @parameter time is not used, since the thread steps at fast as possible, FRI sample time is used instead.
   */
  void step(double time);
  //!@brief copies data from the FRI to member variables for access from outside the loop thread
  void copyFromFRI();

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet (hopefully never!)
protected:
  // none yet
private:
  //true, if the object has been initialized
  bool mIsInit;
  //KUKA Vendor-Interface, wrapped by this class
  friRemote *mpFriRemote;
  //locker for read/write protection
  QReadWriteLock mLock;
  //last commanded joint position
  std::vector<double> mCommandedJointPosition;
  //last measured joint Position
  std::vector<double> mMeasuredJointPosition;
  //Copy of the FRI state
  FRI_STATE mFriState;
  //Copy of the current FRI quality
  FRI_QUALITY mFriQuality;
  //Sample Time of the FRI connection
  float mSampleTime;
  //last known status if power is on on the KUKA RC
  bool mPowerOn;

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet
protected:
  // none yet
private:
  //!IP Address of the remote host
  std::string _mRemoteHost;
  //!local server port
  int _mServerPort;
};

#endif /* CEDAR_DEV_ROBOT_KUKA_KUKA_INTERFACE_H */
