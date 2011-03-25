/*----------------------------------------------------------------------------------------------------------------------
 ----- Institute:   Ruhr-Universitaet Bochum
                    Institut fuer Neuroinformatik

 ----- File:        KukaInterface.cpp

 ----- Author:      Guido Knips

 ----- Email:       guido.knips@ini.rub.de

 ----- Date:        2010 11 23

 ----- Description:

 ----- Credits:
 ---------------------------------------------------------------------------------------------------------------------*/

// LOCAL INCLUDES
#include "KukaInterface.h"

// PROJECT INCLUDES
#include "auxiliaries/exceptions/IndexOutOfRangeException.h"

// SYSTEM INCLUDES

using namespace cedar::dev::robot::kuka;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
KukaInterface::KukaInterface(const std::string& configFileName)
:
KinematicChain(configFileName),
mCommandedJointPosition(LBR_MNJ),
mMeasuredJointPosition(LBR_MNJ)
{
  mIsInit = false;
  mpFriRemote = 0;
  init();
}
KukaInterface::~KukaInterface()
{
  if(mIsInit)
  {
    //stop the looped Thread
    stop();
    //TODO The following line is not true at this point, the script "kukain.src" is not ready yet!
    //If the script "kukain.src" is started on the KUKA-LBR, the first boolean value means "Stop the FRI"
    //it won't throw an exception, because the index is 0 and therefore valid
    mpFriRemote->setToKRLBool(0, true);
    mpFriRemote->doDataExchange();
    //Free Memory
    if(mpFriRemote)
    {
      delete mpFriRemote;
    }
  }
}
//----------------------------------------------------------------------------------------------------------------------
// public member functions
//----------------------------------------------------------------------------------------------------------------------
void KukaInterface::init()
{
  //The number of joints the KUKA LBR has
  //Load Parameters from the configuration file
  //ServerPort: 0 means, FRI will use the default Port
  addParameter(&_mServerPort, "ServerPort", 0);
  /*RemoteHost: if the string is "NULL", the friRemote instance will be created with NULL,
   * else it will interpret it as IP-Address
   */
  addParameter(&_mRemoteHost, "RemoteHost", "NULL");

  //now read the configuration file
  readOrDefaultConfiguration();

  //create a new Instance of the friRemote
  if (_mRemoteHost != string("NULL"))
  {
    mpFriRemote = new friRemote(_mServerPort, _mRemoteHost.c_str());
  }
  else
  {
    mpFriRemote = new friRemote(_mServerPort);
  }
  //copy default values from the FRI
  copyFromFRI();

  //set step size and idle time for the looped thread
  setStepSize(0);
  setIdleTime(0);
  useFixedStepSize(false);
  //start the thread
  start();

  mIsInit = true;
}


double KukaInterface::getJointAngle(unsigned int index)
{
  mLock.lockForRead();
  double a = mMeasuredJointPosition.at(index);
  mLock.unlock();
  return a;
}


void KukaInterface::setJointAngle(unsigned int index, double angle)
{
  mLock.lockForWrite();
  mCommandedJointPosition.at(index) = angle;
  mLock.unlock();
}


void KukaInterface::setWorkingMode(cedar::dev::robot::KinematicChain::ActionType actionType)
{
  KinematicChain::setWorkingMode(actionType);
  this ->start();
}
/*
 * Overwritten start function of KinematicChain
 * the function inherited from KinematicChain does some things we do not want.
 */
void KukaInterface::start(Priority priority)
{
  if(isRunning())
  {
    return;
  }

  QThread::start(priority);
}
//----------------------------------------------------------------------------------------------------------------------
// private member functions
//----------------------------------------------------------------------------------------------------------------------
void KukaInterface::step(double time)
{
  //if the thread has not been initialized, do nothing
  if (mIsInit)
  {
    mLock.lockForWrite();
    //float array for copying joint position to fri
    float commanded_joint[LBR_MNJ];
    //update joint angle and joint velocity if necessary (and only if in command mode)
    //this will leave commanded_joint uninitialized, however, in this case it won't be used by doPositionControl()
    if (mpFriRemote->isPowerOn() && mpFriRemote->getState() == FRI_STATE_CMD)
    {
      switch (getWorkingMode())
      {
        case ACCELERATION:
        //increase speed for all joints
        setJointVelocities(getJointVelocitiesMatrix() + getJointAccelerationsMatrix() * mpFriRemote->getSampleTime());
        case VELOCITY:
          //change position for all joints
          for (unsigned i=0; i<LBR_MNJ; i++)
          {
            commanded_joint[i] = mMeasuredJointPosition.at(i) + getJointVelocity(i) * mpFriRemote->getSampleTime();
          }
          break;
          case ANGLE:
          //copy commanded joint position
          for(unsigned i=0; i<LBR_MNJ; i++)
          {
            commanded_joint[i] = float(mCommandedJointPosition[i]);
          }
          break;
        default:
          cerr << "Invalid working mode in KukaInterface::step(double). I'm afraid I can't do that." << endl;
      }
    }
    //copy position data, but don't do data exchange yet
    mpFriRemote->doPositionControl(commanded_joint, false);
    mLock.unlock();

    //now do the data exchange
    mpFriRemote->doDataExchange();

    //lock and copy data back
    mLock.lockForWrite();
    copyFromFRI();
    mLock.unlock();
  }
}


void KukaInterface::copyFromFRI()
{
  mFriState = mpFriRemote->getState();
  mFriQuality = mpFriRemote->getQuality();
  mSampleTime = mpFriRemote->getSampleTime();
  mPowerOn = mpFriRemote->isPowerOn();
  //Create a std::vector from the float-Array
  float *pJointPos = mpFriRemote->getMsrMsrJntPosition();
  for (unsigned i=0; i<LBR_MNJ; i++)
  {
    mMeasuredJointPosition[i] = double(pJointPos[i]);
  }
  //if not in command mode or Power is not on, reset commanded position to measured position
  if (mpFriRemote->getState() != FRI_STATE_CMD || !mpFriRemote->isPowerOn())
  {
    mCommandedJointPosition = mMeasuredJointPosition;
  }
}
//----------------------------------------------------------------------------------------------------------------------
// wrapped fri-functions
//----------------------------------------------------------------------------------------------------------------------
FRI_STATE KukaInterface::getFriState()
{
  mLock.lockForRead();
  FRI_STATE s = mFriState;
  mLock.unlock();
  return s;
}


FRI_QUALITY KukaInterface::getFriQuality()
{
  mLock.lockForRead();
  FRI_QUALITY q = mFriQuality;
  mLock.unlock();
  return q;
}


float KukaInterface::getSampleTime()
{
  mLock.lockForRead();
  float t = mSampleTime;
  mLock.unlock();
  return t;
}


bool KukaInterface::isPowerOn()
{
  mLock.lockForRead();
  bool on = mPowerOn;
  mLock.unlock();
  return on;
}

