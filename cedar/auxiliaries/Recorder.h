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

    File:        Recorder.h

    Maintainer:  Christian Bodenstein
    Email:       christian.bodenstein@ini.ruhr-uni-bochum.de
    Date:        2013 07 01

    Description: Singleton class for recording and saving the data generated in a network.

    Credits:

======================================================================================================================*/


#ifndef CEDAR_AUX_RECORDER_H_
#define CEDAR_AUX_RECORDER_H_

// CEDAR INCLUDES
#include "cedar/processing/namespace.h"
#include "cedar/auxiliaries/Data.h"
#include "cedar/auxiliaries/LoopedThread.h"
#include "cedar/auxiliaries/ThreadCollection.h"
#include "DataSpectator.h"


// SYSTEM INCLUDES
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <QReadWriteLock>
#include <QTime>
#include <fstream>
#include <sys/stat.h>




  /*!@brief Singleton class for recording and saving the data generated in a network to disk.
   */
class cedar::aux::Recorder : public cedar::aux::LoopedThread
{

  //--------------------------------------------------------------------------------------------------------------------
  // friends
  //--------------------------------------------------------------------------------------------------------------------

  // uses singleton template.
  friend class cedar::aux::Singleton<Recorder>;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
  private:
    //!@brief The private constructor for singleton usage.
    Recorder();
  public:
    //!@brief The Destructor.
    ~Recorder();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
  public:
     /*!@brief Can be called to register a DataPtr to be recorded.
      *
      *             It will create a new thread which will record the data in the specified interval.
      *             Recording will only start when cedar::aux::Recorder::start() is called. If name is already
      *             exist in the Recorder, the DataPtr will not be registered.
      *
      */
    void registerData(cedar::aux::DataPtr toSpectate,int recordIntv,std::string name);
    /*!@brief Starts the recorder thread.
     *
     *              By calling start, every registered observer thread will automatically be started.
     *              Additionally all files will be created and filled with headers.
     */
    void start();

    /*!@brief Stops the recorder thread.
     *
     *              By calling stop, the calling thread waits until all recorded data has been written to disk.
     */
    void stop();

    //!@brief Sets the OutputDirectory
    void setOutputDirectory(std::string path);

    //!@brief Gets the OutputDirectory
    std::string getOutputDirectory();

    //!@brief Returns the elapsed time in ms since the REcorder thread has been started.
    int getTimeStamp();

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
  protected:

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
  private:
    //!@brief Calls write in the specified interval.
    void step(double time);

    //!@brief Creates a new Output directory
    void createOutputDirectory();

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------

  protected:
    // none yet

  private:
    //!@brief The registered DataSpectaors.
    cedar::aux::ThreadCollection mDataSpectatorCollection;

    //!@brief The output directory.
    std::string mOutputDirectory;

    //!@brief Counts the time from when the recorder is started.
    QTime mTime;
};



#include "cedar/auxiliaries/Singleton.h"

namespace cedar
{
  namespace aux
    {
      CEDAR_INSTANTIATE_PROC_TEMPLATE(cedar::aux::Singleton<Recorder>);
      typedef cedar::aux::Singleton<Recorder> RecorderSingleton;
    }
}

#endif // CEDAR_AUX_RECORDER_H
