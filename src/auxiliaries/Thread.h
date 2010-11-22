/*------------------------------------------------------------------------------
 ----- Institute:   Ruhr-Universitaet Bochum
                    Institut fuer Neuroinformatik

 ----- File:        Thread.h

 ----- Author:      Christian Faubel
                    Bjoern Weghenkel
 ----- Email:       christian.faubel@ini.rub.de
                    bjoern.weghenkel@ini.rub.de
 ----- Date:        2010 11 16

 ----- Description: Header for the @em cedar::aux::Thread class.

 ----- Credits:
 -----------------------------------------------------------------------------*/

#ifndef CEDAR_AUX_THREAD_H
#define CEDAR_AUX_THREAD_H

// LOCAL INCLUDES
#include "namespace.h"
#include "Base.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <string>
#include <iostream>
#include <QThread>


/*!@brief Cedar interface for threads
 *
 * Use this interface if your class should be executable as a Qt thread.
 * Just inherit from this class and implement a step function with the
 * given parameter.
 *
 * - to start your thread, call start()
 * - to stop your thread, call stop()
 * - before you delete your object, make sure that there was enough time
 *   to stop the current execution of step()
 *
 * Note that every child class must implement step(). If you design your own wrapper
 * thread with multiple objects that inherit the thread interface, you can call
 * all step functions consecutively and also pass measured time to each step function
 * to fulfill real-time constraints.
 */
class cedar::aux::Thread : public Base, public QThread
{
  //----------------------------------------------------------------------------
  // macros
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  // constructors and destructor
  //----------------------------------------------------------------------------
public:
  /*!@brief The standard constructor.
   *
   * The standard constructor sets the idle time between
   * two executions of step() to 1 millisecond
   */
  Thread(void);

  /*!@brief Constructor with idle time parameter.
   *
   * This constructor sets the idle time between two executions of
   * step() to a given value.
   *
   * @param idleTime the time in milliseconds used in msleep
   */
  Thread(unsigned idleTime);

  //!@brief Destructor
  virtual ~Thread(void) = 0;

  //----------------------------------------------------------------------------
  // public methods
  //----------------------------------------------------------------------------
public:
  /*!@brief Executes step() in a while loop in fixed time intervals.
   *
   * If the system was not fast enough to execute step(time) in the desired
   * time, step(time) is called with the proper multiple of mIdleTime to make
   * up for the lost step(s).
   */
  virtual void run();

  /*!@brief All calculations for each time step are put into step().
   *
   * @param time determines externally measured time since last step()
   */
  virtual void step( unsigned time ) = 0;

  /*!@brief Stops the thread.
   *
   * Since the thread might be busy executing step(), it makes sense to wait a
   * moment for the thread to finish its work.
   *
   * @param timeout the max. time to wait for the thread (in milliseconds).
   */
  void stop( unsigned timeout = 1000 );

  //----------------------------------------------------------------------------
  // protected methods
  //----------------------------------------------------------------------------
protected:
  // none yet

  //----------------------------------------------------------------------------
  // private methods
  //----------------------------------------------------------------------------
private:
  void initStatistics(void);
  inline void updateStatistics(unsigned stepsTaken);
  // none yet

  //----------------------------------------------------------------------------
  // members
  //----------------------------------------------------------------------------
public:
  // none yet (hopefully never!)
protected:
  int mIdleTime; //!< idle time in microseconds
private:
  bool mStop;
  // gather some statistics
  long unsigned mNumberOfSteps;
  long unsigned mSumOfStepsTaken;
  unsigned mMaxStepsTaken;

  //----------------------------------------------------------------------------
  // parameters
  //----------------------------------------------------------------------------
public:
  // none yet
protected:
  // none yet
private:
  // none yet

}; // class cedar::aux::Thread

#endif // CEDAR_AUX_THREAD_H
